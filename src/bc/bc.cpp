// fileName: bc.cpp
#include "bc.hpp"
#include "frameComponent.hpp" 
#include <cstring>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <iostream>

BusController& BusController::getInstance() {
    static BusController instance;
    return instance;
}

BusController::~BusController() {
    shutdown();
}

AiReturn BusController::initialize(int deviceId, int streamId) {
    std::lock_guard<std::mutex> lock(m_apiMutex);
    if (m_isInitialized) {
        std::cout << "[BC] Zaten başlatılmış." << std::endl;
        return API_OK;
    }
    std::cout << "[BC] Başlatılıyor... Cihaz ID: " << deviceId << std::endl;
    m_deviceId = deviceId;
    m_streamId = streamId;

    if (ApiInit() < 1) {
        std::cerr << "[BC] HATA: ApiInit başarısız, kart bulunamadı." << std::endl;
        return API_ERR;
    }
    std::cout << "[BC] ApiInit başarılı." << std::endl;

    TY_API_OPEN api_open_params;
    memset(&api_open_params, 0, sizeof(api_open_params));
    api_open_params.ul_Module = m_deviceId;
    api_open_params.ul_Stream = m_streamId;
    strncpy(api_open_params.ac_SrvName, "local", sizeof(api_open_params.ac_SrvName) - 1);
    
    AiReturn ret = ApiOpenEx(&api_open_params, &m_boardHandle);
    if (ret != API_OK) { std::cerr << "[BC] HATA: ApiOpenEx başarısız." << std::endl; return ret; }
    std::cout << "[BC] ApiOpenEx başarılı. Handle: " << m_boardHandle << std::endl;
    
    TY_API_RESET_INFO reset_info;
    memset(&reset_info, 0, sizeof(reset_info));
    ret = ApiCmdReset(m_boardHandle, m_biuId, API_RESET_ALL, &reset_info);
    if (ret != API_OK) { std::cerr << "[BC] HATA: ApiCmdReset başarısız." << std::endl; return ret; }
    std::cout << "[BC] ApiCmdReset başarılı." << std::endl;

    ret = ApiCmdCalCplCon(m_boardHandle, m_biuId, API_CAL_BUS_PRIMARY, API_CAL_CPL_TRANSFORM);
    if (ret != API_OK) return ret;
    ret = ApiCmdCalCplCon(m_boardHandle, m_biuId, API_CAL_BUS_SECONDARY, API_CAL_CPL_TRANSFORM);
    if (ret != API_OK) return ret;
    std::cout << "[BC] Donanım kuplajı ayarlandı." << std::endl;

    ret = ApiCmdBCIni(m_boardHandle, m_biuId, API_DIS, API_ENA, API_TBM_TRANSFER, API_BC_XFER_BUS_PRIMARY);
    if (ret != API_OK) return ret;
    std::cout << "[BC] BC modu başlatıldı." << std::endl;

    m_isInitialized = true;
    return API_OK;
}

void BusController::shutdown() {
    std::lock_guard<std::mutex> lock(m_apiMutex);
    if (!m_isInitialized) return;
    std::cout << "[BC] Kapatılıyor..." << std::endl;
    if (m_boardHandle != 0) {
        ApiCmdBCHalt(m_boardHandle, m_biuId);
        ApiClose(m_boardHandle);
        m_boardHandle = 0;
    }
    ApiExit();
    m_isInitialized = false;
}

bool BusController::isInitialized() const { return m_isInitialized; }
const char* BusController::getAIMError(AiReturn ret) { return ApiGetErrorMessage(ret); }

AiReturn BusController::defineFrameResources(FrameComponent* frame) {
    std::lock_guard<std::mutex> lock(m_apiMutex);
    // DÜZELTME: Tanımlı olmayan hata kodu API_ERR ile değiştirildi.
    if (!frame) return API_ERR; 
    if (frame->getAimTransferId() != 0) {
        std::cout << "[BC::define] '" << frame->getFrameConfig().label << "' için kaynaklar zaten tanımlı." << std::endl;
        return API_OK;
    }
    const FrameConfig& config = frame->getFrameConfig();
    std::cout << "[BC::define] '" << config.label << "' için yeni kaynaklar tanımlanıyor..." << std::endl;
    
    AiUInt16 xferId = m_nextTransferId++;
    AiUInt16 hdrId = m_nextHeaderId++;
    AiUInt16 bufId = m_nextBufferId++;
    
    frame->setAimIds(xferId, hdrId, bufId);
    std::cout << "[BC::define] Atanan ID'ler -> XFER: " << xferId << ", HDR: " << hdrId << ", BUF: " << bufId << std::endl;
    
    TY_API_BC_BH_INFO bh_info;
    memset(&bh_info, 0, sizeof(bh_info));
    AiReturn ret = ApiCmdBCBHDef(m_boardHandle, m_biuId, hdrId, bufId, 0, 0, API_QUEUE_SIZE_1, API_BQM_CYCLIC, 0, 0, 0, 0, &bh_info);
    if (ret != API_OK) return ret;

    TY_API_BC_XFER xfer;
    memset(&xfer, 0, sizeof(xfer));
    xfer.xid = xferId;
    xfer.hid = hdrId;
    xfer.chn = (config.bus == 'A') ? API_BC_XFER_BUS_PRIMARY : API_BC_XFER_BUS_SECONDARY;
    xfer.tic = API_BC_TIC_NO_INT;
    xfer.hlt = API_BC_HLT_NO_HALT;

    switch (config.mode) {
        case BcMode::BC_TO_RT: xfer.type = API_BC_TYPE_BCRT; xfer.rcv_rt = config.rt; xfer.rcv_sa = config.sa; xfer.wcnt = config.wc; break;
        case BcMode::RT_TO_BC: xfer.type = API_BC_TYPE_RTBC; xfer.xmt_rt = config.rt; xfer.xmt_sa = config.sa; xfer.wcnt = config.wc; break;
        case BcMode::RT_TO_RT: xfer.type = API_BC_TYPE_RTRT; xfer.xmt_rt = config.rt; xfer.xmt_sa = config.sa; xfer.rcv_rt = config.rt2; xfer.rcv_sa = config.sa2; xfer.wcnt = config.wc; break;
        case BcMode::MODE_CODE_NO_DATA: xfer.type = API_BC_TYPE_BCRT; xfer.rcv_rt = config.rt; xfer.wcnt = config.sa; xfer.rcv_sa = 0; break;
        case BcMode::MODE_CODE_WITH_DATA: xfer.type = API_BC_TYPE_BCRT; xfer.rcv_rt = config.rt; xfer.wcnt = config.sa; xfer.rcv_sa = 31; break;
    }
    
    AiUInt32 desc_addr;
    ret = ApiCmdBCXferDef(m_boardHandle, m_biuId, &xfer, &desc_addr);
    std::cout << "[BC::define] Kaynak tanımlama sonucu: " << ret << std::endl;
    return ret;
}

AiReturn BusController::sendAcyclicFrame(const FrameComponent* frame, std::array<AiUInt16, BC_MAX_DATA_WORDS>& receivedData) {
    std::lock_guard<std::mutex> lock(m_apiMutex);
    if (!m_isInitialized || !frame) return API_ERR;
    if (frame->getAimTransferId() == 0) {
        std::cerr << "[BC::send] HATA: Çerçeve kaynakları tanımlanmamış!" << std::endl;
        // DÜZELTME: Tanımlı olmayan hata kodu API_ERR ile değiştirildi.
        return API_ERR; 
    }

    const FrameConfig& config = frame->getFrameConfig();
    const AiUInt16 transferId = frame->getAimTransferId();
    const AiUInt16 headerId = frame->getAimHeaderId();
    const AiUInt16 bufferId = frame->getAimBufferId();
    std::cout << "[BC::send] '"<< config.label <<"' gönderiliyor. Kullanılan ID'ler -> XFER: " << transferId << ", HDR: " << headerId << ", BUF: " << bufferId << std::endl;

    AiReturn ret;
    bool hasDataField = (config.mode == BcMode::BC_TO_RT || config.mode == BcMode::RT_TO_RT || config.mode == BcMode::MODE_CODE_WITH_DATA);
    int wc_to_process = (config.wc == 0 && config.mode != BcMode::MODE_CODE_NO_DATA) ? 32 : config.wc;
    if (config.mode == BcMode::MODE_CODE_WITH_DATA) wc_to_process = 1;

    if (hasDataField && wc_to_process > 0) {
        std::array<AiUInt16, BC_MAX_DATA_WORDS> dataWords;
        for (int i = 0; i < wc_to_process; ++i) {
            try { dataWords[i] = static_cast<AiUInt16>(std::stoul(config.data[i], nullptr, 16)); } catch(...) { dataWords[i] = 0; }
        }
        AiUInt16 outIndex; AiUInt32 outAddr;
        ret = ApiCmdBufDef(m_boardHandle, m_biuId, API_BUF_BC_MSG, headerId, bufferId, wc_to_process, dataWords.data(), &outIndex, &outAddr);
        if (ret != API_OK) { std::cerr << "[BC::send] HATA: ApiCmdBufDef başarısız." << std::endl; return ret; }
    }
    
    TY_API_BC_FRAME temp_minor_frame;
    memset(&temp_minor_frame, 0, sizeof(temp_minor_frame));
    temp_minor_frame.id = (AiUInt16)transferId; 
    temp_minor_frame.cnt = 1;
    temp_minor_frame.instr[0] = API_BC_INSTR_TRANSFER;
    temp_minor_frame.xid[0] = transferId;
    ret = ApiCmdBCFrameDef(m_boardHandle, m_biuId, &temp_minor_frame);
    if (ret != API_OK) { std::cerr << "[BC::send] HATA: ApiCmdBCFrameDef başarısız." << std::endl; return ret; }
    
    TY_API_BC_MFRAME_EX temp_major_frame;
    memset(&temp_major_frame, 0, sizeof(temp_major_frame));
    temp_major_frame.cnt = 1;
    temp_major_frame.fid[0] = temp_minor_frame.id;
    ret = ApiCmdBCMFrameDefEx(m_boardHandle, m_biuId, &temp_major_frame);
    if (ret != API_OK) { std::cerr << "[BC::send] HATA: ApiCmdBCMFrameDefEx başarısız." << std::endl; return ret; }
    
    AiUInt32 major_addr, minor_addr[64];
    ret = ApiCmdBCStart(m_boardHandle, m_biuId, API_BC_START_IMMEDIATELY, 1, 10.0f, 0, &major_addr, minor_addr);
    if (ret != API_OK) { std::cerr << "[BC::send] HATA: ApiCmdBCStart başarısız." << std::endl; return ret; }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(25)); 

    bool expectsData = (config.mode == BcMode::RT_TO_BC || config.mode == BcMode::RT_TO_RT);
    if (expectsData && wc_to_process > 0) {
        AiUInt16 outIndex; AiUInt32 outAddr;
        ret = ApiCmdBufRead(m_boardHandle, m_biuId, API_BUF_BC_MSG, headerId, bufferId, wc_to_process, receivedData.data(), &outIndex, &outAddr);
        if (ret != API_OK) return ret;
    }

    ret = ApiCmdBCHalt(m_boardHandle, m_biuId);
    if (ret != API_OK) return ret;

    std::cout << "[BC::send] Gönderim başarıyla tamamlandı." << std::endl;
    return API_OK;
}