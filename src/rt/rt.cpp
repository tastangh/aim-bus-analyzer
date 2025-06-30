// fileName: rt.cpp
#include "rt.hpp"
#include "frameComponent.hpp" 
#include <cstring>
#include <iostream>
#include <array>

RemoteTerminal& RemoteTerminal::getInstance() {
    static RemoteTerminal instance;
    return instance;
}

RemoteTerminal::~RemoteTerminal() {
    shutdown();
}

AiReturn RemoteTerminal::start(int deviceId, int rtAddress, int streamId) {
    std::lock_guard<std::mutex> lock(m_apiMutex);
    if (m_isRunning) return API_OK;
    
    m_deviceId = deviceId;
    m_rtAddress = rtAddress;

    if (ApiInit() < 1) return API_ERR;

    TY_API_OPEN api_open_params;
    memset(&api_open_params, 0, sizeof(api_open_params));
    api_open_params.ul_Module = m_deviceId;
    api_open_params.ul_Stream = streamId;
    strncpy(api_open_params.ac_SrvName, "local", sizeof(api_open_params.ac_SrvName) - 1);
    
    AiReturn ret = ApiOpenEx(&api_open_params, &m_boardHandle);
    if (ret != API_OK) return ret;
    
    TY_API_RESET_INFO reset_info;
    ret = ApiCmdReset(m_boardHandle, m_biuId, API_RESET_ALL, &reset_info);
    if (ret != API_OK) return ret;

    // Donanımın hafıza yapılandırmasını ve limitlerini oku
    TY_API_GET_MEM_INFO memInfo;
    memset(&memInfo, 0, sizeof(memInfo));
    ret = ApiCmdSysGetMemPartition(m_boardHandle, 0, &memInfo);
    if (ret != API_OK) {
        std::cerr << "Error: Could not get memory partition info from the board." << std::endl;
        return ret;
    }
    
    // DÜZELTME: Hatalı `` işaretçisi kaldırıldı.
    // Kullanabileceğimiz maksimum RT ve BC başlık ID'lerini sakla
    m_maxRtHeaderId = memInfo.ax_BiuCnt[m_biuId].ul_RtBhArea;
    m_maxBcHeaderId = memInfo.ax_BiuCnt[m_biuId].ul_BcBhArea;
    std::cout << "[RT] Max RT HIDs: " << m_maxRtHeaderId << ", Max BC HIDs: " << m_maxBcHeaderId << std::endl;

    if (m_maxBcHeaderId < 1 || m_maxRtHeaderId < 1) {
        std::cerr << "Error: Board configuration does not allow for any buffer headers." << std::endl;
        return API_ERR;
    }

    ret = ApiCmdRTIni(m_boardHandle, m_biuId, m_rtAddress, API_RT_ENABLE_SIMULATION, API_RT_RSP_BOTH_BUSSES, 12.0f, 0);
    if (ret != API_OK) return ret;
    
    ret = ApiCmdBCIni(m_boardHandle, m_biuId, API_DIS, API_DIS, API_TBM_TRANSFER, 0);
    if (ret != API_OK) return ret;

    ret = ApiCmdRTStart(m_boardHandle, m_biuId);
    if (ret != API_OK) return ret;
    
    m_isRunning = true;
    return API_OK;
}

void RemoteTerminal::shutdown() {
    std::lock_guard<std::mutex> lock(m_apiMutex);
    if (!m_isRunning) return;
    if (m_boardHandle != 0) {
        ApiCmdBCHalt(m_boardHandle, m_biuId);
        ApiCmdRTHalt(m_boardHandle, m_biuId);
        ApiClose(m_boardHandle);
        m_boardHandle = 0;
    }
    ApiExit();
    m_isRunning = false;
}

bool RemoteTerminal::isRunning() const { return m_isRunning; }
const char* RemoteTerminal::getAIMError(AiReturn ret) { return ApiGetErrorMessage(ret); }

AiReturn RemoteTerminal::defineFrameAsSubaddress(FrameComponent* component) {
    std::lock_guard<std::mutex> lock(m_apiMutex);
    if (!m_isRunning || !component) return API_ERR;

    // Yeni ID atamadan önce limitleri kontrol et
    if (component->getAimHeaderId() == 0 && m_nextHeaderId >= m_maxRtHeaderId) {
        // DÜZELTME: Hatalı sabit kod, derleyicinin önerdiği doğru kod ile değiştirildi.
        return API_ERR_MAXIMUM; // Maksimum enstans/kaynak sayısına ulaşıldı.
    }
    
    const FrameConfig& config = component->getConfig();

    if (component->getAimHeaderId() == 0) {
        AiUInt16 hid = m_nextHeaderId++;
        AiUInt16 bid = m_nextBufferId++;
        component->setAimIds(hid, bid);

        TY_API_RT_BH_INFO bh_info;
        AiReturn ret = ApiCmdRTBHDef(m_boardHandle, m_biuId, hid, bid, 0, 0, API_QUEUE_SIZE_1, API_BQM_CYCLIC, 0, 0, 0, 0, &bh_info);
        if (ret != API_OK) return ret;
    }

    AiUInt16 dataWords[32];
    int wordCount = (config.wc == 0) ? 32 : config.wc;
    for (int i = 0; i < wordCount; ++i) {
        try { dataWords[i] = static_cast<AiUInt16>(std::stoul(config.data[i], nullptr, 16)); } catch (...) { dataWords[i] = 0; }
    }
    
    AiUInt16 rid;
    AiUInt32 raddr;
    AiReturn ret = ApiCmdBufDef(m_boardHandle, m_biuId, API_BUF_RT_MSG, component->getAimHeaderId(), 0, wordCount, dataWords, &rid, &raddr);
    if (ret != API_OK) return ret;

    AiUInt8 sa_con = config.enabled ? API_RT_ENABLE_SA : API_RT_DISABLE_SA;
    ret = ApiCmdRTSACon(m_boardHandle, m_biuId, m_rtAddress, config.sa, component->getAimHeaderId(), API_RT_TYPE_TRANSMIT_SA, sa_con, 0, 0, 0);

    return ret;
}

AiReturn RemoteTerminal::testTransmitSubaddress(const FrameComponent* component) {
    std::lock_guard<std::mutex> lock(m_apiMutex);
    if (!m_isRunning || !component) return API_ERR;

    if (m_testBcHeaderId == 0) {
        m_testBcHeaderId = m_maxBcHeaderId - 1;
        m_testBcBufferId = m_maxBcHeaderId - 1;
        TY_API_BC_BH_INFO test_bh_info;
        AiReturn ret = ApiCmdBCBHDef(m_boardHandle, m_biuId, m_testBcHeaderId, m_testBcBufferId, 0, 0, API_QUEUE_SIZE_1, API_BQM_CYCLIC, 0, 0, 0, 0, &test_bh_info);
        if (ret != API_OK) {
            m_testBcHeaderId = 0; 
            return ret;
        }
    }

    const auto& config = component->getConfig();

    TY_API_BC_XFER xfer;
    memset(&xfer, 0, sizeof(xfer));
    xfer.xid = m_testBcHeaderId; 
    xfer.hid = m_testBcHeaderId;
    xfer.type = API_BC_TYPE_RTBC;
    xfer.chn = (config.bus == 'A') ? API_BC_XFER_BUS_PRIMARY : API_BC_XFER_BUS_SECONDARY;
    xfer.xmt_rt = config.rt;
    xfer.xmt_sa = config.sa;
    xfer.wcnt = config.wc;

    std::array<AiUInt16, 32> receivedData;
    TY_API_BC_XFER_DSP transfer_status;
    return ApiCmdBCAcycPrepAndSendTransferBlocking(m_boardHandle, m_biuId, &xfer, receivedData.data(), &transfer_status);
}