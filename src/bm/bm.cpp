#include "bm.hpp"
#include <stdio.h>
#include <cstring>
#include <chrono>
#include <cinttypes>
#include <sstream>

/**
 * @def AIM_CHECK_BM_ERROR
 * @brief A utility macro for robust error handling of AIM API calls.
 *        It checks the return value of an API function, prints a detailed
 *        error message to stderr if it's not API_OK, and returns the error code
 *        to propagate the failure up the call stack.
 * @param retVal The return value from the AIM API function.
 * @param funcName A string literal representing the name of the calling function for context.
 * @param bmInstancePtr A pointer to the BM instance, reserved for future use (e.g., error state handling).
 */
#define AIM_CHECK_BM_ERROR(retVal, funcName, bmInstancePtr) \
    if (retVal != API_OK) { \
        const char* errMsgPtr = ApiGetErrorMessage(retVal); \
        fprintf(stderr, "ERROR in BM::%s: %s (0x%04X) at %s:%d\n", funcName, errMsgPtr ? errMsgPtr : "Unknown Error", retVal, __FILE__, __LINE__); \
        if (bmInstancePtr) {} \
        return retVal; \
    }

/**
 * @brief Resets the internal state of a MessageTransaction struct.
 *        Called to prepare the struct for assembling the nexta message from the data stream.
 */
void BM::MessageTransaction::clear() {
    full_timetag = 0; last_timetag_l_data = 0; last_timetag_h_data = 0;
    cmd1 = 0; bus1 = 0; cmd1_valid = false;
    cmd2 = 0; bus2 = 0; cmd2_valid = false;
    stat1 = 0; stat1_bus = 0; stat1_valid = false;
    stat2 = 0; stat2_bus = 0; stat2_valid = false;
    data_words.clear();
    error_word = 0; error_valid = false;
}

/**
 * @brief Checks if the transaction object is empty.
 * @return True if no valid command, error, or data has been added, false otherwise.
 */
bool BM::MessageTransaction::isEmpty() const {
    return !cmd1_valid && !error_valid && full_timetag == 0 && data_words.empty();
}

/**
 * @brief Converts an AIM API error code into a human-readable string.
 * @param errorCode The AiReturn value from an API call.
 * @return A std::string containing the error message.
 */
std::string getAIMApiErrorMessage(AiReturn errorCode) {
    const char* errMsgPtr = ApiGetErrorMessage(errorCode);
    return errMsgPtr ? std::string(errMsgPtr) : "Unknown AIM API Error";
}

/**
 * @brief Provides access to the singleton instance of the BM class.
 *        Ensures a single, globally accessible point for Bus Monitor operations.
 * @return Reference to the singleton BM instance.
 */
BM& BM::getInstance() {
    static BM instance;
    return instance;
}

/**
 * @brief Constructor for the Bus Monitor (BM) singleton.
 *        Initializes member variables and resizes the data reception buffer.
 *        Private to enforce the singleton pattern.
 */
BM::BM() : m_ulModHandle(0), m_monitoringActive(false), m_shutdownRequested(false),
           m_dataLoggingEnabled(false), 
           m_guiUpdateMessagesCb(nullptr), m_guiUpdateTreeItemCb(nullptr),
           m_filterEnabled(false), m_filterBus(0), m_filterRt(-1), m_filterSa(-1),
           m_dataQueueId(0)
{
    m_rxDataBuffer.resize(RX_BUFFER_CHUNK_SIZE);
}

/**
 * @brief Destructor for the Bus Monitor (BM) singleton.
 *        Ensures that monitoring is stopped, the board is shut down, and the
 *        AIM API is properly exited to release all resources.
 */
BM::~BM() {
    if (isMonitoring()) { stop(); }
    shutdownBoard(); 
    ApiExit();
}

/**
 * @brief Initializes the AIM hardware board and resets it to a known state.
 *        This function performs the initial handshake with the API library and the hardware,
 *        making it ready for configuration.
 * @param config UI-provided configuration containing device and stream identifiers.
 * @return API_OK on success, or an AIM error code on failure.
 */
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

/**
 * @brief Closes the handle to the AIM board, releasing it for other applications.
 */
void BM::shutdownBoard() { if (m_ulModHandle != 0) { ApiClose(m_ulModHandle); m_ulModHandle = 0; } }

/**
 * @brief Configures the board specifically for Bus Monitor (BM) operations.
 *        Sets coupling, initializes the BM core, and sets the capture mode to continuous recording.
 * @param config UI-provided configuration specifying the coupling mode.
 * @return API_OK on success, or an AIM error code on failure.
 */
AiReturn BM::configureBusMonitor(const ConfigBmUi& config) {
    AiReturn ret = API_OK;
    ret = ApiCmdCalCplCon(m_ulModHandle, (AiUInt8)config.ulStream, API_CAL_BUS_PRIMARY, config.ulCoupling); AIM_CHECK_BM_ERROR(ret, "configureBusMonitor/ApiCmdCalCplCon Primary", this);
    ret = ApiCmdCalCplCon(m_ulModHandle, (AiUInt8)config.ulStream, API_CAL_BUS_SECONDARY, config.ulCoupling); AIM_CHECK_BM_ERROR(ret, "configureBusMonitor/ApiCmdCalCplCon Secondary", this);
    ret = ApiCmdBMIni(m_ulModHandle, (AiUInt8)config.ulStream); AIM_CHECK_BM_ERROR(ret, "configureBusMonitor/ApiCmdBMIni", this);
    TY_API_BM_CAP_SETUP bmCapSetup; memset(&bmCapSetup, 0, sizeof(bmCapSetup)); bmCapSetup.cap_mode = API_BM_CAPMODE_RECORDING; 
    ret = ApiCmdBMCapMode(m_ulModHandle, (AiUInt8)config.ulStream, &bmCapSetup); AIM_CHECK_BM_ERROR(ret, "configureBusMonitor/ApiCmdBMCapMode", this);
    return API_OK;
}

/**
 * @brief Opens and starts the hardware data queue for BM recording.
 *        This is the channel through which monitored data flows from the hardware to the host.
 * @return API_OK on success, or an AIM error code on failure.
 */
AiReturn BM::openDataQueue() {
    AiReturn ret = API_OK;
    m_dataQueueId = (m_currentConfig.ulStream == 1) ? API_DATA_QUEUE_ID_BM_REC_BIU1 : API_DATA_QUEUE_ID_BM_REC_BIU2;
    AiUInt32 queueSizeOnCard = 0;
    ret = ApiCmdDataQueueOpen(m_ulModHandle, m_dataQueueId, &queueSizeOnCard); AIM_CHECK_BM_ERROR(ret, "openDataQueue/ApiCmdDataQueueOpen", this);
    if (queueSizeOnCard == 0) return API_ERR_NAK;
    ret = ApiCmdDataQueueControl(m_ulModHandle, m_dataQueueId, API_DATA_QUEUE_CTRL_MODE_START); AIM_CHECK_BM_ERROR(ret, "openDataQueue/ApiCmdDataQueueControl START", this);
    return API_OK;
}

/**
 * @brief Stops and closes the hardware data queue.
 */
void BM::closeDataQueue() { if (m_ulModHandle != 0 && m_dataQueueId != 0) { ApiCmdDataQueueControl(m_ulModHandle, m_dataQueueId, API_DATA_QUEUE_CTRL_MODE_STOP); ApiCmdDataQueueClose(m_ulModHandle, m_dataQueueId); m_dataQueueId = 0; } }

/**
 * @brief Public entry point to start the entire monitoring process.
 *        Orchestrates board initialization, configuration, and starts the dedicated monitoring thread.
 * @param config The complete configuration from the user interface.
 * @return API_OK if monitoring started successfully, otherwise an error code.
 */
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

/**
 * @brief Public entry point to stop the monitoring process.
 *        Signals the monitoring thread to terminate, joins it, and de-initializes hardware resources.
 */
void BM::stop() {
    m_shutdownRequested.store(true); if (m_monitorThread.joinable()) { m_monitorThread.join(); }
    if (m_ulModHandle != 0) { ApiCmdBMHalt(m_ulModHandle, (AiUInt8)m_currentConfig.ulStream); closeDataQueue(); }
    shutdownBoard(); m_monitoringActive.store(false); 
}

/**
 * @brief Returns the current monitoring state.
 * @return True if the monitoring thread is active, false otherwise.
 */
bool BM::isMonitoring() const { return m_monitoringActive.load(); }

/**
 * @brief Registers a callback function for updating the UI message display.
 * @param cb A std::function to be called with formatted message strings.
 */
void BM::setUpdateMessagesCallback(UpdateMessagesCallback cb) { m_guiUpdateMessagesCb = cb; }

/**
 * @brief Registers a callback function for updating the UI tree view.
 * @param cb A std::function to be called with active bus/RT/SA information.
 */
void BM::setUpdateTreeItemCallback(UpdateTreeItemCallback cb) { m_guiUpdateTreeItemCb = cb; }

/**
 * @brief The main function for the dedicated monitoring thread.
 *        Continuously polls the data queue for new monitor data and triggers processing.
 *        This loop is the heart of the real-time data acquisition.
 */
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

/**
 * @brief Formats a completed message transaction into a human-readable string for the UI.
 *        This function is responsible for creating the final output format, including Time,
 *        Bus, Type, and Data Word sections. It also applies filtering and triggers UI tree updates.
 * @param trans The fully assembled message transaction to be processed.
 * @param outString The output string to which the formatted message will be appended.
 */
void BM::formatAndRelayTransaction(const MessageTransaction& trans, std::string& outString) {
    if (!trans.cmd1_valid) return;

    // Apply filtering criteria before any expensive formatting.
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

    // Signal the UI to update its tree view for the active terminal.
    if (m_guiUpdateTreeItemCb) {
        AiUInt8 rtAddr1 = (trans.cmd1 >> 11) & 0x1F;
        AiUInt8 sa_mc1  = (trans.cmd1 >> 5) & 0x1F;
        if (!(sa_mc1 == 0 || sa_mc1 == 31)) {
            m_guiUpdateTreeItemCb(trans.bus1, rtAddr1, sa_mc1, true);
        }
    }

    // Use a stringstream for efficient string building.
    std::stringstream ss;
    char tempBuf[256];

    // Format timestamp.
    if (trans.full_timetag != 0) {
        uint64_t total_us = trans.full_timetag * 1;
        snprintf(tempBuf, sizeof(tempBuf), "Time: %010" PRIu64 "us\n", total_us);
        ss << tempBuf;
    } else {
        ss << "Time: <no timestamp>\n";
    }

    // Decode command word to create a summary line.
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

    // Determine the expected number of data words and format them,
    // using placeholders if the actual data is missing.
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
            
            // Wrap data words every 8 words for readability.
            if ((i + 1) % 8 == 0 && (i + 1) < words_to_display) {
                ss << "\n      ";
            }
        }
        ss << "\n";
    }

    ss << "----------------------------------------\n";
    outString += ss.str();
}

/**
 * @brief Processes a raw chunk of data from the hardware queue.
 *        This function implements a state machine that iterates through the raw monitor words,
 *        assembles them into logical MessageTransaction objects, and dispatches them for formatting.
 * @param buffer Pointer to the raw data buffer.
 * @param bytesRead The number of bytes read into the buffer.
 */
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
        
        // A new transaction is typically demarcated by a timetag, error, or command word.
        // When one is encountered, the previously assembled transaction is finalized and processed.
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
            default: break; // Ignore unused or reserved types.
        }
    }

    // Process any remaining transaction at the end of the buffer.
    if (!currentTransaction.isEmpty()) {
        if (currentTransaction.full_timetag == 0) currentTransaction.full_timetag = last_full_timetag;
        formatAndRelayTransaction(currentTransaction, allMessagesForUi);
    }
    
    // Relay the complete chunk of formatted messages to the UI.
    if (m_guiUpdateMessagesCb && !allMessagesForUi.empty()) {
        m_guiUpdateMessagesCb(allMessagesForUi);
    }

    if (m_dataLoggingEnabled.load()) {
        Logger::info("\n---\n" + allMessagesForUi);
    }
}

/**
 * @brief Sets the state for enabling or disabling data logging to a file.
 */
void BM::enableDataLogging(bool enable) {
    m_dataLoggingEnabled.store(enable);
}
/**
 * @brief Enables or disables message filtering.
 * @param enable True to enable filtering, false to disable.
 */
void BM::enableFilter(bool enable) { std::lock_guard<std::mutex> lock(m_filterMutex); m_filterEnabled.store(enable); }

/**
 * @brief Checks if message filtering is currently enabled.
 * @return True if filtering is enabled.
 */
bool BM::isFilterEnabled() const { return m_filterEnabled.load(); }

/**
 * @brief Sets the criteria for message filtering.
 *        -1 for rt/sa or 0 for bus means 'any'.
 * @param bus The bus to filter ('A' or 'B').
 * @param rt The remote terminal address to filter (0-30).
 * @param sa The subaddress to filter (0-30).
 */
void BM::setFilterCriteria(char bus, int rt, int sa) { std::lock_guard<std::mutex> lock(m_filterMutex); m_filterBus.store(bus); m_filterRt.store(rt); m_filterSa.store(sa); }