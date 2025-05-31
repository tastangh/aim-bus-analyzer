#include "bm.hpp"
#include <stdio.h>
#include <cstring>
#include <chrono>
#include <inttypes.h> 

#define AIM_CHECK_BM_ERROR(retVal, funcName, bmInstancePtr) \
    if (retVal != API_OK) { \
        const char* errMsgPtr = ApiGetErrorMessage(retVal); \
        fprintf(stderr, "ERROR in BM::%s: %s (0x%04X) at %s:%d\n", funcName, errMsgPtr ? errMsgPtr : "Unknown Error", retVal, __FILE__, __LINE__); \
        if (bmInstancePtr) {  \
              \
        } \
         \
        return retVal; \
    }


std::string getAIMApiErrorMessage(AiReturn errorCode) {
    const char* errMsgPtr = ApiGetErrorMessage(errorCode);
    return errMsgPtr ? std::string(errMsgPtr) : "Unknown AIM API Error";
}

BM& BM::getInstance() {
    static BM instance;
    return instance;
}

BM::BM() : m_ulModHandle(0), m_monitoringActive(false), m_shutdownRequested(false),
           m_guiUpdateMessagesCb(nullptr), m_guiUpdateTreeItemCb(nullptr),
           m_filterEnabled(false), m_filterBus(0), m_filterRt(-1), m_filterSa(-1),
           m_dataQueueId(0)
{
    m_rxDataBuffer.resize(RX_BUFFER_CHUNK_SIZE);
    printf("BM instance created.\n");
}

BM::~BM() {
    printf("BM instance destroying...\n");
    if (isMonitoring()) {
        stop();
    }
    shutdownBoard(); 

    printf("BM instance destroyed. Calling ApiExit.\n");
    ApiExit();
}

AiReturn BM::initializeBoard(const ConfigBmUi& config) {
    AiReturn ret = API_OK;
    static bool apiLibraryInitialized = false;

    if (!apiLibraryInitialized) {
        ret = ApiInit();
        if (ret <= 0) {
            fprintf(stderr, "BM::initializeBoard - ApiInit failed or no boards found. Ret: %d\n", ret);
            return ret; 
        }
        printf("BM::initializeBoard - ApiInit successful, found %d board(s).\n", ret);
        apiLibraryInitialized = true;
    }

    TY_API_OPEN xApiOpen;
    memset(&xApiOpen, 0, sizeof(xApiOpen));
    xApiOpen.ul_Module = config.ulDevice;
    xApiOpen.ul_Stream = config.ulStream;
    strcpy(xApiOpen.ac_SrvName, "local"); 

    ret = ApiOpenEx(&xApiOpen, &m_ulModHandle);
    if (ret != API_OK) {
        fprintf(stderr, "BM::initializeBoard - ApiOpenEx ERROR: %s\n", getAIMApiErrorMessage(ret).c_str());
        m_ulModHandle = 0;
        return ret;
    }
    printf("BM::initializeBoard - ApiOpenEx successful. ModuleHandle: 0x%X\n", m_ulModHandle);

    TY_API_RESET_INFO xApiResetInfo;
    memset(&xApiResetInfo, 0, sizeof(xApiResetInfo));
    ret = ApiCmdReset(m_ulModHandle, (AiUInt8)config.ulStream, API_RESET_ALL, &xApiResetInfo);
    if (ret != API_OK) {
        fprintf(stderr, "BM::initializeBoard - ApiCmdReset ERROR: %s\n", getAIMApiErrorMessage(ret).c_str());
        ApiClose(m_ulModHandle);
        m_ulModHandle = 0;
        return ret;
    }
    printf("BM::initializeBoard - ApiCmdReset successful.\n");
    return API_OK;
}

void BM::shutdownBoard() {
    if (m_ulModHandle != 0) {
        printf("BM::shutdownBoard - Closing ModuleHandle: 0x%X\n", m_ulModHandle);
        ApiClose(m_ulModHandle); 
        m_ulModHandle = 0;
    }
}

AiReturn BM::configureBusMonitor(const ConfigBmUi& config) {
    AiReturn ret = API_OK;

    // Coupling
    printf("BM::configureBusMonitor - Coupling mode: %d, Stream: %d\n", config.ulCoupling, config.ulStream);
    ret = ApiCmdCalCplCon(m_ulModHandle, (AiUInt8)config.ulStream, API_CAL_BUS_PRIMARY, config.ulCoupling);
    AIM_CHECK_BM_ERROR(ret, "configureBusMonitor/ApiCmdCalCplCon Primary", this);
    ret = ApiCmdCalCplCon(m_ulModHandle, (AiUInt8)config.ulStream, API_CAL_BUS_SECONDARY, config.ulCoupling);
    AIM_CHECK_BM_ERROR(ret, "configureBusMonitor/ApiCmdCalCplCon Secondary", this);

    // BM Init
    printf("BM::configureBusMonitor - Initializing BM...\n");
    ret = ApiCmdBMIni(m_ulModHandle, (AiUInt8)config.ulStream);
    AIM_CHECK_BM_ERROR(ret, "configureBusMonitor/ApiCmdBMIni", this);

    // BM Capture Mode
    TY_API_BM_CAP_SETUP bmCapSetup;
    memset(&bmCapSetup, 0, sizeof(bmCapSetup));
    bmCapSetup.cap_mode = API_BM_CAPMODE_RECORDING; 
    printf("BM::configureBusMonitor - Setting BM Capture Mode...\n");
    ret = ApiCmdBMCapMode(m_ulModHandle, (AiUInt8)config.ulStream, &bmCapSetup);
    AIM_CHECK_BM_ERROR(ret, "configureBusMonitor/ApiCmdBMCapMode", this);

    return API_OK;
}

AiReturn BM::openDataQueue() {
    AiReturn ret = API_OK;
    m_dataQueueId = (m_currentConfig.ulStream == 1) ? API_DATA_QUEUE_ID_BM_REC_BIU1 : API_DATA_QUEUE_ID_BM_REC_BIU2;
    AiUInt32 queueSizeOnCard = 0;
    printf("BM::openDataQueue - Opening Data Queue (ID: %u)...\n", m_dataQueueId);
    ret = ApiCmdDataQueueOpen(m_ulModHandle, m_dataQueueId, &queueSizeOnCard); 
    AIM_CHECK_BM_ERROR(ret, "openDataQueue/ApiCmdDataQueueOpen", this);
    if (queueSizeOnCard == 0) {
        fprintf(stderr, "BM::openDataQueue - Error: Data Queue ASP size is 0.\n");
        return API_ERR_NAK;
    }
    printf("BM::openDataQueue - Data Queue opened. Onboard ASP Queue Size: %u bytes\n", queueSizeOnCard);

    printf("BM::openDataQueue - Starting Data Queue Control...\n");
    ret = ApiCmdDataQueueControl(m_ulModHandle, m_dataQueueId, API_DATA_QUEUE_CTRL_MODE_START);
    AIM_CHECK_BM_ERROR(ret, "openDataQueue/ApiCmdDataQueueControl START", this);
    printf("BM::openDataQueue - Data Queue started.\n");
    return API_OK;
}

void BM::closeDataQueue() {
    if (m_ulModHandle != 0 && m_dataQueueId != 0) {
        printf("BM::closeDataQueue - Stopping Data Queue Control...\n");
        ApiCmdDataQueueControl(m_ulModHandle, m_dataQueueId, API_DATA_QUEUE_CTRL_MODE_STOP); 

        printf("BM::closeDataQueue - Closing Data Queue (ID: %u)...\n", m_dataQueueId);
        ApiCmdDataQueueClose(m_ulModHandle, m_dataQueueId); 
        m_dataQueueId = 0;
    }
}


AiReturn BM::start(const ConfigBmUi& config) {
    if (m_monitoringActive.load()) {
        printf("BM::start - Monitoring is already active.\n");
        return API_OK;
    }
    m_currentConfig = config;
    m_shutdownRequested.store(false);

    AiReturn ret = initializeBoard(m_currentConfig);
    if (ret != API_OK) return ret;

    ret = configureBusMonitor(m_currentConfig);
    if (ret != API_OK) { shutdownBoard(); return ret; }

    ret = openDataQueue();
    if (ret != API_OK) { shutdownBoard(); return ret; }

    printf("BM::start - Starting Bus Monitor HW...\n");
    ret = ApiCmdBMStart(m_ulModHandle, (AiUInt8)m_currentConfig.ulStream);
    if (ret != API_OK) {
        fprintf(stderr, "BM::start - ApiCmdBMStart ERROR: %s\n", getAIMApiErrorMessage(ret).c_str());
        closeDataQueue();
        shutdownBoard();
        return ret;
    }

    m_monitoringActive.store(true);
    m_monitorThread = std::thread(&BM::monitorThreadFunc, this);
    printf("BM::start - Bus Monitor thread started.\n");
    return API_OK;
}

void BM::stop() {
    printf("BM::stop - Stop requested.\n");
    m_shutdownRequested.store(true);

    if (m_monitorThread.joinable()) {
        m_monitorThread.join();
        printf("BM::stop - Monitor thread joined.\n");
    }

    if (m_ulModHandle != 0) {
        printf("BM::stop - Halting Bus Monitor HW...\n");
        ApiCmdBMHalt(m_ulModHandle, (AiUInt8)m_currentConfig.ulStream);
        closeDataQueue(); 
    }
    m_monitoringActive.store(false); 
    printf("BM::stop - Monitoring stopped.\n");
}

bool BM::isMonitoring() const {
    return m_monitoringActive.load();
}

void BM::setUpdateMessagesCallback(UpdateMessagesCallback cb) {
    m_guiUpdateMessagesCb = cb;
}

void BM::setUpdateTreeItemCallback(UpdateTreeItemCallback cb) {
    m_guiUpdateTreeItemCb = cb;
}

void BM::monitorThreadFunc() {
    printf("BM::monitorThreadFunc - Thread started.\n");
    TY_API_DATA_QUEUE_READ queueReadParams;
    TY_API_DATA_QUEUE_STATUS queueStatus;
    AiReturn ret;

    while (!m_shutdownRequested.load()) {
        if (m_ulModHandle == 0) {
            fprintf(stderr, "BM::monitorThreadFunc - Module handle is 0. Exiting thread.\n");
            break;
        }

        memset(&queueReadParams, 0, sizeof(queueReadParams));
        queueReadParams.id = m_dataQueueId;
        queueReadParams.buffer = m_rxDataBuffer.data();
        queueReadParams.bytes_to_read = RX_BUFFER_CHUNK_SIZE;
        memset(&queueStatus, 0, sizeof(queueStatus));

        ret = ApiCmdDataQueueRead(m_ulModHandle, &queueReadParams, &queueStatus);
        if (ret != API_OK) {
            fprintf(stderr, "BM::monitorThreadFunc - ApiCmdDataQueueRead ERROR: %s (0x%X). Exiting thread.\n",
                   getAIMApiErrorMessage(ret).c_str(), ret);
            break; 
        }

        if (queueStatus.bytes_transfered > 0) {
            processAndRelayData(m_rxDataBuffer.data(), queueStatus.bytes_transfered);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(20)); 
        }
    }
    m_monitoringActive.store(false); 
    printf("BM::monitorThreadFunc - Thread finished.\n");
}

void BM::processAndRelayData(const unsigned char* buffer, AiUInt32 bytesRead) {
    std::string chunkStringForUi;
    char tempLineBuffer[1024];

    AiUInt32 numMonitorWords = bytesRead / 4;
    const AiUInt32* pMonitorWords = reinterpret_cast<const AiUInt32*>(buffer);

    for (AiUInt32 i = 0; i < numMonitorWords; ++i) {
        AiUInt32 monitorWord = pMonitorWords[i];
        AiUInt8 type  = (monitorWord >> 28) & 0x0F;
        AiUInt32 entryData = monitorWord & 0x07FFFFFF;

        snprintf(tempLineBuffer, sizeof(tempLineBuffer), "Idx %3u: Raw=0x%08X, Type=0x%X\n", i, monitorWord, type);
        chunkStringForUi += tempLineBuffer;

        std::string parsedEntryDetails;
        switch (type) {
            case 0x0: parsedEntryDetails = "    Entry Not Updated\n"; break;
            case 0x1: helperParseErrorWordEntry(entryData, parsedEntryDetails); break;
            case 0x2: helperParseTimeTagLowEntry(entryData, parsedEntryDetails); break;
            case 0x3: helperParseTimeTagHighEntry(entryData, parsedEntryDetails); break;
            case 0x8: case 0x9: case 0xA: case 0xB:
            case 0xC: case 0xD: case 0xE: case 0xF:
                helperParseBusWordEntry(entryData, type, parsedEntryDetails);
                break;
            default:
                snprintf(tempLineBuffer, sizeof(tempLineBuffer), "    Reserved/Unknown Monitor Word Type: 0x%X\n", type);
                parsedEntryDetails = tempLineBuffer;
                break;
        }
        chunkStringForUi += parsedEntryDetails;
    }

    if (m_guiUpdateMessagesCb && !chunkStringForUi.empty()) {
        m_guiUpdateMessagesCb(chunkStringForUi);
    }
}

void BM::helperParseCommandWord(AiUInt16 cmdWord, std::string& out) {
    char buf[128];
    AiUInt8 rtAddr = (cmdWord >> 11) & 0x1F;
    AiUInt8 tr     = (cmdWord >> 10) & 0x01;
    AiUInt8 sa_mc  = (cmdWord >> 5)  & 0x1F;
    AiUInt8 wc_mc  = cmdWord & 0x1F;
    snprintf(buf, sizeof(buf), "      CMD: RT=%02u, T/R=%u, ", rtAddr, tr);
    out += buf;
    if (sa_mc == 0 || sa_mc == 31) {
        snprintf(buf, sizeof(buf), "MC=%02u (Op %u)\n", sa_mc, wc_mc);
    } else {
        snprintf(buf, sizeof(buf), "SA=%02u, WC=%02u\n", sa_mc, wc_mc);
    }
    out += buf;
}

void BM::helperParseStatusWord(AiUInt16 statusWord, std::string& out) {
    char buf[256];
    AiUInt8 rtAddr        = (statusWord >> 11) & 0x1F;
    AiUInt8 msgError      = (statusWord >> 10) & 0x01;
    AiUInt8 instBit       = (statusWord >> 9)  & 0x01;
    AiUInt8 srvReqBit     = (statusWord >> 8)  & 0x01;
    AiUInt8 bcastRcvdBit  = (statusWord >> 3)  & 0x01;
    AiUInt8 busyBit       = (statusWord >> 2)  & 0x01;
    AiUInt8 subSysFlagBit = (statusWord >> 1)  & 0x01;
    AiUInt8 termFlagBit   = statusWord & 0x01;
    snprintf(buf, sizeof(buf), "      STA: RT=%02u, ME=%u, INST=%u, SRQ=%u, BCST_RX=%u, BUSY=%u, SSF=%u, TF=%u (0x%04X)\n",
           rtAddr, msgError, instBit, srvReqBit, bcastRcvdBit, busyBit, subSysFlagBit, termFlagBit, statusWord);
    out += buf;
}

void BM::helperParseDataWord(AiUInt16 dataWord, std::string& out) {
    char buf[64];
    snprintf(buf, sizeof(buf), "      DAT: 0x%04X (%u)\n", dataWord, dataWord);
    out += buf;
}

void BM::helperParseBusWordEntry(AiUInt32 entryData, AiUInt8 type, std::string& out) {
    char buf[64];
    AiUInt16 busWord = entryData & 0xFFFF;
    switch (type) {
        case 0x8: snprintf(buf, sizeof(buf), "    CW1 (Pri): "); out+=buf; helperParseCommandWord(busWord, out); break;
        case 0x9: snprintf(buf, sizeof(buf), "    CW2 (Pri): "); out+=buf; helperParseCommandWord(busWord, out); break;
        case 0xA: snprintf(buf, sizeof(buf), "    DW  (Pri): "); out+=buf; helperParseDataWord(busWord, out);    break;
        case 0xB: snprintf(buf, sizeof(buf), "    SW  (Pri): "); out+=buf; helperParseStatusWord(busWord, out);  break;
        case 0xC: snprintf(buf, sizeof(buf), "    CW1 (Sec): "); out+=buf; helperParseCommandWord(busWord, out); break;
        case 0xD: snprintf(buf, sizeof(buf), "    CW2 (Sec): "); out+=buf; helperParseCommandWord(busWord, out); break;
        case 0xE: snprintf(buf, sizeof(buf), "    DW  (Sec): "); out+=buf; helperParseDataWord(busWord, out);    break;
        case 0xF: snprintf(buf, sizeof(buf), "    SW  (Sec): "); out+=buf; helperParseStatusWord(busWord, out);  break;
        default:  snprintf(buf, sizeof(buf), "    Unknown BusWord subtype for parsing\n"); out+=buf; break;
    }
}
void BM::helperParseErrorWordEntry(AiUInt32 entryData, std::string& out) {
    char buf[256]; 
    AiUInt8 transmitRTFlag   = (entryData >> 17) & 0x01;
    AiUInt8 receiveRTFlag    = (entryData >> 16) & 0x01;
    AiUInt16 errorType       = entryData & 0xFFFF;
    snprintf(buf, sizeof(buf), "    ERR: T_RT=%u, R_RT=%u, Mask=0x%04X\n", transmitRTFlag, receiveRTFlag, errorType);
    out += buf;
}

void BM::helperParseTimeTagLowEntry(AiUInt32 entryData, std::string& out) {
    char buf[128];
    AiUInt32 seconds = (entryData >> 20) & 0x3F;
    AiUInt32 microseconds = entryData & 0xFFFFF;
    snprintf(buf, sizeof(buf), "    TT_L: Seconds=%02u, Microseconds=%06u\n", seconds, microseconds);
    out += buf;
}

void BM::helperParseTimeTagHighEntry(AiUInt32 entryData, std::string& out) {
    char buf[128];
    AiUInt32 days    = (entryData >> 11) & 0x1FF;
    AiUInt32 hours   = (entryData >> 6)  & 0x1F;
    AiUInt32 minutes = entryData & 0x3F;
    snprintf(buf, sizeof(buf), "    TT_H: DayOfYear=%03u, Hours=%02u, Minutes=%02u\n", days, hours, minutes);
    out += buf;
}

// Filtreleme metotları
void BM::enableFilter(bool enable) {
    std::lock_guard<std::mutex> lock(m_filterMutex);
    m_filterEnabled.store(enable);
    printf("BM::enableFilter - Filter %s.\n", enable ? "enabled" : "disabled");
}

bool BM::isFilterEnabled() const {
    return m_filterEnabled.load();
}

void BM::setFilterCriteria(char bus, int rt, int sa) {
    std::lock_guard<std::mutex> lock(m_filterMutex);
    m_filterBus.store(bus);
    m_filterRt.store(rt);
    m_filterSa.store(sa);
    printf("BM::setFilterCriteria - Bus: %c, RT: %d, SA: %d\n", bus, rt, sa);
}

bool BM::applyFilterLogic(AiUInt8 type, AiUInt16 busWord) {
    if (!m_filterEnabled.load()) {
        return true; 
    }

    std::lock_guard<std::mutex> lock(m_filterMutex); 
    char currentBusChar = 0;
    if (type >= 0x8 && type <= 0xB) currentBusChar = 'A'; // Primary
    else if (type >= 0xC && type <= 0xF) currentBusChar = 'B'; // Secondary

    if (m_filterBus.load() != 0 && currentBusChar != 0 && toupper(m_filterBus.load()) != currentBusChar) {
        return false;
    }

    // RT ve SA filtresi (sadece Komut ve Durum kelimeleri için anlamlı)
    if (type == 0x8 || type == 0x9 || type == 0xB || type == 0xC || type == 0xD || type == 0xF) {
        AiUInt8 rtAddr = (busWord >> 11) & 0x1F;
        if (m_filterRt.load() != -1 && m_filterRt.load() != rtAddr) {
            return false;
        }

        if (type == 0x8 || type == 0xC) { // Sadece Komut Kelimeleri SA içerir
            AiUInt8 sa_mc = (busWord >> 5) & 0x1F;
            if (!(sa_mc == 0 || sa_mc == 31)) { // Eğer Mode Code değilse (SA ise)
                if (m_filterSa.load() != -1 && m_filterSa.load() != sa_mc) {
                    return false;
                }
            } else { // Mode Code ise ve SA filtresi aktifse gösterme (veya tam tersi, ihtiyaca göre)
                if (m_filterSa.load() != -1) return false;
            }
        }
    }
    return true;
}