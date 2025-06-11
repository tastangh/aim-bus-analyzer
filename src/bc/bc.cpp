#include "bc.hpp"
#include "logger.hpp"
#include <chrono>
#include <cstring>
#include <thread>
#include <iomanip>
#include <sstream>

// Bizim mantıksal ID'lerimiz. 
// Bunlar, transferleri ve buffer'ları ayırt etmek için kullanılır.
constexpr AiUInt16 XFER_ID_BC_TO_RT = 1;
constexpr AiUInt16 XFER_ID_RT_TO_BC = 2;
constexpr AiUInt16 XFER_ID_RT_TO_RT = 3;
constexpr AiUInt16 XFER_ID_MODE_CODE = 4;

constexpr AiUInt16 BUFFER_ID = 1; // Tüm operasyonlar için tek bir buffer header kullanacağız.

BC& BC::getInstance() {
    static BC instance;
    return instance;
}

BC::BC() {}

BC::~BC() {
    shutdown();
}

AiReturn BC::initialize(int devNum) {
    if (m_isInitialized.load() && m_devNum == devNum) return API_OK;
    if (m_isInitialized.load()) shutdown();
    
    m_devNum = devNum;
    Logger::info("Initializing Bus Controller for AIM device: " + std::to_string(m_devNum));

    AiReturn ret = ApiInit();
    if (ret < 0) { // Sadece gerçek hataları kontrol et (API_WARN_ALREADY_INIT'i yoksay)
        Logger::error("BC: ApiInit failed. " + getAIMError(ret));
        return ret;
    }

    TY_API_OPEN apiOpen;
    memset(&apiOpen, 0, sizeof(apiOpen));
    apiOpen.ul_Module = devNum;
    apiOpen.ul_Stream = 1; 
    strcpy(apiOpen.ac_SrvName, "local");
    
    ret = ApiOpenEx(&apiOpen, &m_ulModHandle);
    if (ret != API_OK) {
        Logger::error("BC: ApiOpenEx failed on device " + std::to_string(devNum) + ". " + getAIMError(ret));
        m_ulModHandle = 0;
        return ret;
    }

    ret = ApiCmdReset(m_ulModHandle, (AiUInt8)apiOpen.ul_Stream, API_RESET_ALL, nullptr);
    if (ret != API_OK) {
        Logger::error("BC: ApiCmdReset failed. " + getAIMError(ret));
        shutdown();
        return ret;
    }
    
    ret = setupBoardForBC();
    if (ret != API_OK) {
        shutdown();
        return ret;
    }

    m_isInitialized = true;
    Logger::info("BC Initialized successfully.");
    return API_OK;
}

void BC::shutdown() {
    if (m_ulModHandle != 0) {
        ApiCmdBCHalt(m_ulModHandle, 1);
        ApiClose(m_ulModHandle);
        Logger::info("BC Shutdown complete for device: " + std::to_string(m_devNum));
    }
    m_ulModHandle = 0;
    m_devNum = -1;
    m_isInitialized = false;
}

AiReturn BC::setupBoardForBC() {
    Logger::info("Configuring board for Bus Controller mode...");
    // Varsayılan parametrelerle (retry yok, global bus, vs.) BC'yi başlatıyoruz
    AiReturn ret = ApiCmdBCIni(m_ulModHandle, 1, 0, 0, 0, 0); 
    if (ret != API_OK) {
        Logger::error("BC: ApiCmdBCIni failed. " + getAIMError(ret));
    }
    return ret;
}

void BC::convertStringDataToU16(const std::array<std::string, RT_SA_MAX_COUNT>& str_data, AiUInt16* u16_data_buffer, int count) {
    for (int i = 0; i < count; ++i) {
        try {
            u16_data_buffer[i] = static_cast<AiUInt16>(std::stoul(str_data.at(i), nullptr, 16));
        } catch (...) {
            u16_data_buffer[i] = 0;
        }
    }
}

AiReturn BC::sendAcyclicFrame(BcMode mode, char bus, int rtTx, int saTx, int rtRx, int saRx, int wc, 
                              const std::array<std::string, RT_SA_MAX_COUNT>& data_in,
                              std::array<AiUInt16, RT_SA_MAX_COUNT>& data_out) {
    if (!m_isInitialized.load()) return API_ERR_NOT_INITIALIZED;

    TY_API_BC_XFER xfer;
    memset(&xfer, 0, sizeof(xfer));
    
    xfer.chn = (bus == 'A' ? API_BC_XFER_BUS_PRIMARY : API_BC_XFER_BUS_SECONDARY);
    xfer.wcnt = wc;
    xfer.hid = BUFFER_ID; // Tüm transferler için aynı buffer header'ı kullanıyoruz.

    // Mode'a göre transfer parametrelerini ayarla
    switch (mode) {
        case BcMode::BC_TO_RT:
            xfer.xid = XFER_ID_BC_TO_RT;
            xfer.type = API_BC_TYPE_BCRT;
            xfer.rcv_rt = rtRx; xfer.rcv_sa = saRx;
            break;
        case BcMode::RT_TO_BC:
            xfer.xid = XFER_ID_RT_TO_BC;
            xfer.type = API_BC_TYPE_RTBC;
            xfer.xmt_rt = rtTx; xfer.xmt_sa = saTx;
            break;
        case BcMode::RT_TO_RT:
            xfer.xid = XFER_ID_RT_TO_RT;
            xfer.type = API_BC_TYPE_RTRT;
            xfer.xmt_rt = rtTx; xfer.xmt_sa = saTx;
            xfer.rcv_rt = rtRx; xfer.rcv_sa = saRx;
            break;
        // Mode Code gönderme burada ele alınabilir veya ayrı bir fonksiyonda.
        // Şimdilik ana transfer tiplerine odaklanalım.
    }
    
    int word_count_to_process = (wc == 0) ? 32 : wc;
    AiUInt16 data_buffer[32] = {0};
    
    if (mode == BcMode::BC_TO_RT) {
        convertStringDataToU16(data_in, data_buffer, word_count_to_process);
    }

    // Acyclic transfer için:
    // 1. Buffer Header'ı tanımla (her ihtimale karşı)
    TY_API_BC_BH_INFO bh_info;
    AiReturn ret = ApiCmdBCBHDef(m_ulModHandle, 1, xfer.hid, 0, 0, 0, 1, 0, 0, 0, 0, 0, &bh_info);
    if(ret != API_OK) return ret;

    // 2. Transferi tanımla
    ret = ApiCmdBCXferDef(m_ulModHandle, 1, &xfer, nullptr);
    if(ret != API_OK) return ret;

    // 3. Veri buffer'ını yaz (eğer gerekiyorsa)
    if (mode == BcMode::BC_TO_RT) {
        ret = ApiCmdBufWriteBlock(m_ulModHandle, 1, API_BUF_BC_MSG, xfer.hid, 0, word_count_to_process, data_buffer);
        if(ret != API_OK) return ret;
    }

    // 4. Tek komutluk bir Instruction listesi oluştur
    TY_API_BC_FW_INSTR instr_list[1];
    memset(instr_list, 0, sizeof(instr_list));
    instr_list[0].op = API_BC_FWI_XFER;
    instr_list[0].par1 = xfer.xid;

    // 5. Hazırlanan instruction'ı çalıştır
    ret = ApiCmdBCStart(m_ulModHandle, 1, API_BC_START_INSTR_TABLE, 1, (AiUInt32)&instr_list, nullptr, nullptr);
    if(ret != API_OK) {
        Logger::error("BC: ApiCmdBCStart failed. " + getAIMError(ret));
        return ret;
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ApiCmdBCHalt(m_ulModHandle, 1);

    // 6. Gelen veriyi oku (eğer gerekiyorsa)
    if (mode == BcMode::RT_TO_BC || mode == BcMode::RT_TO_RT) {
        AiUInt16 rid; AiUInt32 raddr;
        ret = ApiCmdBufRead(m_ulModHandle, 1, API_BUF_RT_MSG, xfer.hid, 0, word_count_to_process, data_out.data(), &rid, &raddr);
        if (ret != API_OK) {
            Logger::error("BC: ApiCmdBufRead failed. " + getAIMError(ret));
            return ret;
        }
    }

    return API_OK;
}

std::string BC::getAIMError(AiReturn error_code) {
    const char* msg = ApiGetErrorMessage(error_code);
    return msg ? std::string(msg) : "Unknown AIM API Error";
}