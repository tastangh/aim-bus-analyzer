// fileName: bc.cpp
#include "bc.hpp"
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
    if (m_isInitialized) return API_OK;
    
    std::cout << "[BC] Başlatılıyor... Cihaz ID: " << deviceId << std::endl;
    m_deviceId = deviceId;
    m_streamId = streamId;

    if (ApiInit() < 1) return API_ERR;
    std::cout << "[BC] ApiInit başarılı." << std::endl;

    TY_API_OPEN api_open_params;
    memset(&api_open_params, 0, sizeof(api_open_params));
    api_open_params.ul_Module = m_deviceId;
    api_open_params.ul_Stream = m_streamId;
    strncpy(api_open_params.ac_SrvName, "local", sizeof(api_open_params.ac_SrvName) - 1);
    
    AiReturn ret = ApiOpenEx(&api_open_params, &m_boardHandle);
    if (ret != API_OK) return ret;
    std::cout << "[BC] ApiOpenEx başarılı." << std::endl;
    
    TY_API_RESET_INFO reset_info;
    memset(&reset_info, 0, sizeof(reset_info));
    ret = ApiCmdReset(m_boardHandle, m_biuId, API_RESET_ALL, &reset_info);
    if (ret != API_OK) return ret;
    std::cout << "[BC] ApiCmdReset başarılı." << std::endl;

    ret = ApiCmdCalCplCon(m_boardHandle, m_biuId, API_CAL_BUS_PRIMARY, API_CAL_CPL_TRANSFORM);
    if (ret != API_OK) return ret;
    ret = ApiCmdCalCplCon(m_boardHandle, m_biuId, API_CAL_BUS_SECONDARY, API_CAL_CPL_TRANSFORM);
    if (ret != API_OK) return ret;
    std::cout << "[BC] Donanım kuplajı ayarlandı." << std::endl;

    // DÜZELTME: ApiCmdBCIni çağrısı yeni örnekteki gibi daha detaylı parametrelerle güncellendi.
    ret = ApiCmdBCIni(m_boardHandle, m_biuId, API_DIS, API_ENA, API_TBM_TRANSFER, API_BC_XFER_BUS_PRIMARY);
    if (ret != API_OK) return ret;
    std::cout << "[BC] BC modu başlatıldı." << std::endl;

    m_isInitialized = true;
    return API_OK;
}

void BusController::shutdown() {
    std::lock_guard<std::mutex> lock(m_apiMutex);
    if (!m_isInitialized) return;
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

AiReturn BusController::sendAcyclicFrame(const FrameConfig& config, std::array<AiUInt16, BC_MAX_DATA_WORDS>& receivedData) {
    std::lock_guard<std::mutex> lock(m_apiMutex);
    std::cout << "[BC::sendAcyclicFrame] '" << config.label << "' çerçevesi gönderiliyor..." << std::endl;
    if (!m_isInitialized) return API_ERR;

    AiReturn ret;
    const AiUInt16 transferId = m_nextTransferId++;
    const AiUInt16 headerId = m_nextHeaderId++;
    const AiUInt16 bufferId = m_nextBufferId++;
    
    TY_API_BC_BH_INFO bh_info;
    memset(&bh_info, 0, sizeof(bh_info));
    ret = ApiCmdBCBHDef(m_boardHandle, m_biuId, headerId, bufferId, 0, 0, API_QUEUE_SIZE_1, API_BQM_CYCLIC, 0, 0, 0, 0, &bh_info);
    if (ret != API_OK) return ret;

    bool hasDataField = (config.mode == BcMode::BC_TO_RT || config.mode == BcMode::MODE_CODE_WITH_DATA);
    int wc_to_send = (config.wc == 0 && hasDataField) ? 32 : config.wc;
    if (config.mode == BcMode::MODE_CODE_WITH_DATA) wc_to_send = 1;

    if (hasDataField) {
        std::array<AiUInt16, BC_MAX_DATA_WORDS> dataWords;
        for (int i = 0; i < wc_to_send; ++i) {
            try { dataWords[i] = static_cast<AiUInt16>(std::stoul(config.data[i], nullptr, 16)); } catch(...) { dataWords[i] = 0; }
        }
        AiUInt16 outIndex; AiUInt32 outAddr;
        ret = ApiCmdBufDef(m_boardHandle, m_biuId, API_BUF_BC_MSG, headerId, bufferId, wc_to_send, dataWords.data(), &outIndex, &outAddr);
        if (ret != API_OK) return ret;
    }

    TY_API_BC_XFER xfer;
    memset(&xfer, 0, sizeof(xfer));
    xfer.xid = transferId;
    xfer.hid = headerId;
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
    if (ret != API_OK) return ret;
    
    TY_API_BC_FRAME temp_minor_frame;
    memset(&temp_minor_frame, 0, sizeof(temp_minor_frame));
    temp_minor_frame.id = (AiUInt16)transferId;
    temp_minor_frame.cnt = 1;
    temp_minor_frame.instr[0] = API_BC_INSTR_TRANSFER;
    temp_minor_frame.xid[0] = transferId;
    ret = ApiCmdBCFrameDef(m_boardHandle, m_biuId, &temp_minor_frame);
    if (ret != API_OK) return ret;
    
    TY_API_BC_MFRAME_EX temp_major_frame;
    memset(&temp_major_frame, 0, sizeof(temp_major_frame));
    temp_major_frame.cnt = 1;
    temp_major_frame.fid[0] = temp_minor_frame.id;
    ret = ApiCmdBCMFrameDefEx(m_boardHandle, m_biuId, &temp_major_frame);
    if (ret != API_OK) return ret;
    
    AiUInt32 major_addr, minor_addr[64];
    // DÜZELTME: ApiCmdBCStart'ın 5. parametresi (frame_time) 10.0f olarak ayarlandı.
    ret = ApiCmdBCStart(m_boardHandle, m_biuId, API_BC_START_IMMEDIATELY, 1, 10.0f, 0, &major_addr, minor_addr);
    if (ret != API_OK) return ret;

    // Transferin tamamlanması için çok kısa bir bekleme
    std::this_thread::sleep_for(std::chrono::milliseconds(25)); 

    bool expectsData = (config.mode == BcMode::RT_TO_BC || config.mode == BcMode::RT_TO_RT);
    if (expectsData) {
        AiUInt16 outIndex; AiUInt32 outAddr;
        int wc_to_read = (config.wc == 0) ? 32 : config.wc;
        ret = ApiCmdBufRead(m_boardHandle, m_biuId, API_BUF_BC_MSG, headerId, bufferId, wc_to_read, receivedData.data(), &outIndex, &outAddr);
        if (ret != API_OK) return ret;
    }

    // DÜZELTME: Tek seferlik gönderimden sonra BC'yi durdur.
    ret = ApiCmdBCHalt(m_boardHandle, m_biuId);
    if (ret != API_OK) return ret;

    std::cout << "[BC::sendAcyclicFrame] Gönderim başarılı." << std::endl;
    return API_OK;
}