#include "bm.hpp"
#include <stdio.h>
#include <cstring>
#include <chrono>
#include <cinttypes>
#include <sstream>

#define AIM_CHECK_BM_ERROR(retVal, funcName, bmInstancePtr) \
    if (retVal != API_OK) { \
        const char* errMsgPtr = ApiGetErrorMessage(retVal); \
        fprintf(stderr, "ERROR in BM::%s: %s (0x%04X) at %s:%d\n", funcName, errMsgPtr ? errMsgPtr : "Unknown Error", retVal, __FILE__, __LINE__); \
        if (bmInstancePtr) {} \
        return retVal; \
    }

void BM::MessageTransaction::clear() { // Mode code'lar her zaman 1 veri kelimesi içerir.
    full_timetag = 0; last_timetag_l_data = 0; last_timetag_h_data = 0;
    cmd1 = 0; bus1 = 0; cmd1_valid = false;
    cmd2 = 0; bus2 = 0; cmd2_valid = false;
    stat1 = 0; stat1_bus = 0; stat1_valid = false;
    stat2 = 0; stat2_bus = 0; stat2_valid = false;
    data_words.clear();
    error_word = 0; error_valid = false;
}

bool BM::MessageTransaction::isEmpty() const {
    return !cmd1_valid && !error_valid && full_timetag == 0 && data_words.empty();
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
}

BM::~BM() {
    if (isMonitoring()) { stop(); }
    shutdownBoard(); 
    ApiExit();
}

AiReturn BM::initializeBoard(const ConfigBmUi& config) {
    AiReturn ret = API_OK; static bool apiLibraryInitialized = false;
    if (!apiLibraryInitialized) { ret = ApiInit(); if (ret <= 0) return ret < 0 ? (AiReturn)ret : API_ERR_NAK; apiLibraryInitialized = true; }
    TY_API_OPEN xApiOpen; memset(&xApiOpen, 0, sizeof(xApiOpen)); xApiOpen.ul_Module = config.ulDevice; xApiOpen.ul_Stream = config.ulStream; strcpy(xApiOpen.ac_SrvName, "local"); 
    ret = ApiOpenEx(&xApiOpen, &m_ulModHandle); if (ret != API_OK) { m_ulModHandle = 0; return ret; }
    TY_API_RESET_INFO xApiResetInfo; memset(&xApiResetInfo, 0, sizeof(xApiResetInfo));
    ret = ApiCmdReset(m_ulModHandle, (AiUInt8)config.ulStream, API_RESET_ALL, &xApiResetInfo);
    if (ret != API_OK) { ApiClose(m_ulModHandle); m_ulModHandle = 0; return ret; }
    return API_OK;
}
void BM::shutdownBoard() { if (m_ulModHandle != 0) { ApiClose(m_ulModHandle); m_ulModHandle = 0; } }
AiReturn BM::configureBusMonitor(const ConfigBmUi& config) {
    AiReturn ret = API_OK;
    ret = ApiCmdCalCplCon(m_ulModHandle, (AiUInt8)config.ulStream, API_CAL_BUS_PRIMARY, config.ulCoupling); AIM_CHECK_BM_ERROR(ret, "configureBusMonitor/ApiCmdCalCplCon Primary", this);
    ret = ApiCmdCalCplCon(m_ulModHandle, (AiUInt8)config.ulStream, API_CAL_BUS_SECONDARY, config.ulCoupling); AIM_CHECK_BM_ERROR(ret, "configureBusMonitor/ApiCmdCalCplCon Secondary", this);
    ret = ApiCmdBMIni(m_ulModHandle, (AiUInt8)config.ulStream); AIM_CHECK_BM_ERROR(ret, "configureBusMonitor/ApiCmdBMIni", this);
    TY_API_BM_CAP_SETUP bmCapSetup; memset(&bmCapSetup, 0, sizeof(bmCapSetup)); bmCapSetup.cap_mode = API_BM_CAPMODE_RECORDING; 
    ret = ApiCmdBMCapMode(m_ulModHandle, (AiUInt8)config.ulStream, &bmCapSetup); AIM_CHECK_BM_ERROR(ret, "configureBusMonitor/ApiCmdBMCapMode", this);
    return API_OK;
}
AiReturn BM::openDataQueue() {
    AiReturn ret = API_OK;
    m_dataQueueId = (m_currentConfig.ulStream == 1) ? API_DATA_QUEUE_ID_BM_REC_BIU1 : API_DATA_QUEUE_ID_BM_REC_BIU2;
    AiUInt32 queueSizeOnCard = 0;
    ret = ApiCmdDataQueueOpen(m_ulModHandle, m_dataQueueId, &queueSizeOnCard); AIM_CHECK_BM_ERROR(ret, "openDataQueue/ApiCmdDataQueueOpen", this);
    if (queueSizeOnCard == 0) return API_ERR_NAK;
    ret = ApiCmdDataQueueControl(m_ulModHandle, m_dataQueueId, API_DATA_QUEUE_CTRL_MODE_START); AIM_CHECK_BM_ERROR(ret, "openDataQueue/ApiCmdDataQueueControl START", this);
    return API_OK;
}
void BM::closeDataQueue() { if (m_ulModHandle != 0 && m_dataQueueId != 0) { ApiCmdDataQueueControl(m_ulModHandle, m_dataQueueId, API_DATA_QUEUE_CTRL_MODE_STOP); ApiCmdDataQueueClose(m_ulModHandle, m_dataQueueId); m_dataQueueId = 0; } }
AiReturn BM::start(const ConfigBmUi& config) {
    if (m_monitoringActive.load()) return API_OK;
    m_currentConfig = config; m_shutdownRequested.store(false);
    AiReturn ret = initializeBoard(m_currentConfig); if (ret != API_OK) return ret;
    ret = configureBusMonitor(m_currentConfig); if (ret != API_OK) { shutdownBoard(); return ret; }
    ret = openDataQueue(); if (ret != API_OK) { shutdownBoard(); return ret; }
    ret = ApiCmdBMStart(m_ulModHandle, (AiUInt8)m_currentConfig.ulStream);
    if (ret != API_OK) { closeDataQueue(); shutdownBoard(); return ret; }
    m_monitoringActive.store(true); m_monitorThread = std::thread(&BM::monitorThreadFunc, this);
    return API_OK;
}
void BM::stop() {
    m_shutdownRequested.store(true); if (m_monitorThread.joinable()) { m_monitorThread.join(); }
    if (m_ulModHandle != 0) { ApiCmdBMHalt(m_ulModHandle, (AiUInt8)m_currentConfig.ulStream); closeDataQueue(); }
    shutdownBoard(); m_monitoringActive.store(false); 
}
bool BM::isMonitoring() const { return m_monitoringActive.load(); }
void BM::setUpdateMessagesCallback(UpdateMessagesCallback cb) { m_guiUpdateMessagesCb = cb; }
void BM::setUpdateTreeItemCallback(UpdateTreeItemCallback cb) { m_guiUpdateTreeItemCb = cb; }
void BM::monitorThreadFunc() {
    TY_API_DATA_QUEUE_READ queueReadParams; TY_API_DATA_QUEUE_STATUS queueStatus; AiReturn ret;
    while (!m_shutdownRequested.load()) {
        if (m_ulModHandle == 0) break;
        memset(&queueReadParams, 0, sizeof(queueReadParams));
        queueReadParams.id = m_dataQueueId; queueReadParams.buffer = m_rxDataBuffer.data(); queueReadParams.bytes_to_read = RX_BUFFER_CHUNK_SIZE;
        memset(&queueStatus, 0, sizeof(queueStatus));
        ret = ApiCmdDataQueueRead(m_ulModHandle, &queueReadParams, &queueStatus);
        if (ret == API_ERR_TIMEOUT) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); continue; }
        if (ret != API_OK) break;
        if (queueStatus.bytes_transfered > 0) { processAndRelayData(m_rxDataBuffer.data(), queueStatus.bytes_transfered); } 
        else { std::this_thread::sleep_for(std::chrono::milliseconds(20)); }
    }
    m_monitoringActive.store(false); 
}

void BM::formatAndRelayTransaction(const MessageTransaction& trans, std::string& outString) {
    if (!trans.cmd1_valid) return;

    { 
        std::lock_guard<std::mutex> lock(m_filterMutex);
        if (m_filterEnabled.load()) {
            bool passed = true;
            char bus_to_check = toupper(m_filterBus.load());
            int rt_to_check = m_filterRt.load();
            int sa_to_check = m_filterSa.load();
            if (bus_to_check != 0 && toupper(trans.bus1) != bus_to_check) passed = false;
            if (passed && rt_to_check != -1 && rt_to_check != ((trans.cmd1 >> 11) & 0x1F)) passed = false;
            if (passed && sa_to_check != -1) {
                AiUInt8 sa_mc = (trans.cmd1 >> 5) & 0x1F;
                if (sa_mc == 0 || sa_mc == 31 || sa_to_check != sa_mc) passed = false;
            }
            if (!passed) return;
        }
    } 

    if (m_guiUpdateTreeItemCb) {
        AiUInt8 rtAddr1 = (trans.cmd1 >> 11) & 0x1F;
        AiUInt8 sa_mc1  = (trans.cmd1 >> 5) & 0x1F;
        if (!(sa_mc1 == 0 || sa_mc1 == 31)) {
            m_guiUpdateTreeItemCb(trans.bus1, rtAddr1, sa_mc1, true);
        }
    }

    std::stringstream ss;
    char tempBuf[256];

    if (trans.full_timetag != 0) {
        uint64_t total_us = trans.full_timetag * 1;
        snprintf(tempBuf, sizeof(tempBuf), "Time: %010" PRIu64 "us\n", total_us);
        ss << tempBuf;
    } else {
        ss << "Time: <no timestamp>\n";
    }

    AiUInt8 rt = (trans.cmd1 >> 11) & 0x1F;
    AiUInt8 tr = (trans.cmd1 >> 10) & 0x01;
    AiUInt8 sa = (trans.cmd1 >> 5) & 0x1F;
    AiUInt8 wc_field = trans.cmd1 & 0x1F;

    ss << "Bus: " << trans.bus1 << " Type: ";
    if (trans.cmd2_valid) { AiUInt8 rt2 = (trans.cmd2 >> 11) & 0x1F; ss << "RT " << (int)rt << " to RT " << (int)rt2; } 
    else if (tr == 0) { ss << "BC to RT " << (int)rt; } 
    else { ss << "RT " << (int)rt << " to BC"; }

    if (sa == 0 || sa == 31) { snprintf(tempBuf, sizeof(tempBuf), " MC: %d (Op %d)", (int)sa, (int)wc_field); } 
    else { snprintf(tempBuf, sizeof(tempBuf), " SA: %d WC: %d", (int)sa, (int)wc_field); }
    ss << tempBuf;

    if (!trans.stat1_valid) ss << " (No Response)";
    ss << "\n";

    int words_to_display = 0;
    bool is_mode_code = (sa == 0 || sa == 31);
    
    bool mc_has_data = (is_mode_code && ((tr == 1 && wc_field == 17) || (tr == 0 && wc_field == 16)));
    
    if (!is_mode_code || mc_has_data) {
        words_to_display = (wc_field == 0) ? 32 : wc_field;
        if (is_mode_code) words_to_display = 1;
    }

    if (words_to_display > 0) {
        ss << "Data: ";
        for (int i = 0; i < words_to_display; ++i) {
            if (i < trans.data_words.size()) {
                snprintf(tempBuf, sizeof(tempBuf), "%04X ", trans.data_words[i]);
                ss << tempBuf;
            } else {
                ss << "0000 ";
            }
            
            if ((i + 1) % 8 == 0 && (i + 1) < words_to_display) {
                ss << "\n      ";
            }
        }
        ss << "\n";
    }

    ss << "----------------------------------------\n";
    outString += ss.str();
}

void BM::processAndRelayData(const unsigned char* buffer, AiUInt32 bytesRead) {
    MessageTransaction currentTransaction;
    std::string allMessagesForUi;
    static uint64_t last_full_timetag = 0;
    AiUInt32 numMonitorWords = bytesRead / 4;
    const AiUInt32* pMonitorWords = reinterpret_cast<const AiUInt32*>(buffer);

    for (AiUInt32 i = 0; i < numMonitorWords; ++i) {
        AiUInt32 monitorWord = pMonitorWords[i];
        AiUInt8 type = (monitorWord >> 28) & 0x0F;
        AiUInt32 entryData = monitorWord & 0x07FFFFFF;
        AiUInt16 busWord = entryData & 0xFFFF;
        bool isNewMessageStart = (type == 0x1 || type == 0x2 || type == 0x3 || type == 0x8 || type == 0xC);

        if (isNewMessageStart && !currentTransaction.isEmpty()) {
            if (currentTransaction.full_timetag == 0) currentTransaction.full_timetag = last_full_timetag;
            formatAndRelayTransaction(currentTransaction, allMessagesForUi);
            currentTransaction.clear();
        }

        switch (type) {
            case 0x1: currentTransaction.error_valid = true; currentTransaction.error_word = entryData; break;
            case 0x2:
                currentTransaction.last_timetag_l_data = entryData & 0x03FFFFFF;
                if(currentTransaction.last_timetag_h_data != 0) {
                   currentTransaction.full_timetag = ((uint64_t)currentTransaction.last_timetag_h_data << 26) | currentTransaction.last_timetag_l_data;
                   last_full_timetag = currentTransaction.full_timetag;
                }
                break;
            case 0x3:
                currentTransaction.last_timetag_h_data = entryData & 0x000FFFFF;
                if(currentTransaction.last_timetag_l_data != 0) {
                   currentTransaction.full_timetag = ((uint64_t)currentTransaction.last_timetag_h_data << 26) | currentTransaction.last_timetag_l_data;
                   last_full_timetag = currentTransaction.full_timetag;
                }
                break;
            case 0x8: case 0x9: case 0xA: case 0xB: case 0xC: case 0xD: case 0xE: case 0xF: {
                char bus = (type <= 0xB) ? 'A' : 'B';
                switch(type & 0x3) {
                    case 0x0: currentTransaction.cmd1 = busWord; currentTransaction.bus1 = bus; currentTransaction.cmd1_valid = true; break;
                    case 0x1: currentTransaction.cmd2 = busWord; currentTransaction.bus2 = bus; currentTransaction.cmd2_valid = true; break;
                    case 0x2: currentTransaction.data_words.push_back(busWord); break;
                    case 0x3:
                        if (!currentTransaction.stat1_valid) {
                           currentTransaction.stat1 = busWord; currentTransaction.stat1_bus = bus; currentTransaction.stat1_valid = true;
                        } else {
                           currentTransaction.stat2 = busWord; currentTransaction.stat2_bus = bus; currentTransaction.stat2_valid = true;
                        }
                        break;
                }
                break;
            }
            default: break;
        }
    }

    if (!currentTransaction.isEmpty()) {
        if (currentTransaction.full_timetag == 0) currentTransaction.full_timetag = last_full_timetag;
        formatAndRelayTransaction(currentTransaction, allMessagesForUi);
    }
    if (m_guiUpdateMessagesCb && !allMessagesForUi.empty()) {
        m_guiUpdateMessagesCb(allMessagesForUi);
    }
}

void BM::enableFilter(bool enable) { std::lock_guard<std::mutex> lock(m_filterMutex); m_filterEnabled.store(enable); }
bool BM::isFilterEnabled() const { return m_filterEnabled.load(); }
void BM::setFilterCriteria(char bus, int rt, int sa) { std::lock_guard<std::mutex> lock(m_filterMutex); m_filterBus.store(bus); m_filterRt.store(rt); m_filterSa.store(sa); }