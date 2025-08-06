// =================================================================================
// FILE: bm.hpp
// =================================================================================
#ifndef BM_HPP
#define BM_HPP

#include "Api1553.h"
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <functional>
#include <iostream>
#include "logger.hpp" // Default logging library

// BM configuration provided by the UI
struct ConfigBmUi {
    AiUInt32 ulDevice;
    AiUInt32 ulStream;
    AiUInt32 ulCoupling;
};

// Bus Monitor class (Singleton)
class BM {
public:
    // Callback type for updating UI messages
    using UpdateMessagesCallback = std::function<void(const std::string&)>;
    // Callback type for updating tree view items
    using UpdateTreeItemCallback = std::function<void(char, int, int, bool)>;

    // Access to the singleton instance
    static BM& getInstance();

    // Starts the monitoring process
    AiReturn start(const ConfigBmUi& config);
    // Stops the monitoring process
    void stop();
    // Checks the monitoring status
    bool isMonitoring() const;

    // Sets the callback functions
    void setUpdateMessagesCallback(UpdateMessagesCallback cb);
    void setUpdateTreeItemCallback(UpdateTreeItemCallback cb);

    // Enables/disables data logging
    void enableDataLogging(bool enable);
    
    // Manages filtering
    void enableFilter(bool enable);
    bool isFilterEnabled() const;
    void setFilterCriteria(char bus, int rt, int sa, int mc);

private:
    // Private constructor and destructor (Singleton)
    BM();
    ~BM();

    // Copy and assignment operators are deleted
    BM(const BM&) = delete;
    BM& operator=(const BM&) = delete;

    // Board initialization and shutdown
    AiReturn initializeBoard(const ConfigBmUi& config);
    void shutdownBoard();

    // Bus Monitor configuration
    AiReturn configureBusMonitor(const ConfigBmUi& config);

    // Data queue management
    AiReturn openDataQueue();
    void closeDataQueue();

    // Main function for the monitoring thread
    void monitorThreadFunc();

    // Data processing and relaying
    void processAndRelayData(const unsigned char* buffer, AiUInt32 bytesRead);

    // Internal struct for parsing messages
    struct MessageTransaction {
        uint64_t full_timetag = 0;
        uint64_t last_timetag_l_data = 0;
        uint64_t last_timetag_h_data = 0;
        AiUInt16 cmd1 = 0; char bus1 = 0; bool cmd1_valid = false;
        AiUInt16 cmd2 = 0; char bus2 = 0; bool cmd2_valid = false;
        AiUInt16 stat1 = 0; char stat1_bus = 0; bool stat1_valid = false;
        AiUInt16 stat2 = 0; char stat2_bus = 0; bool stat2_valid = false;
        std::vector<AiUInt16> data_words;
        AiUInt32 error_word = 0; bool error_valid = false;

        void clear();
        bool isEmpty() const;
    };

    void formatAndRelayTransaction(const MessageTransaction& trans, std::string& outString);

    // Member variables
    AiUInt32 m_ulModHandle;
    std::atomic<bool> m_monitoringActive;
    std::atomic<bool> m_shutdownRequested;
    std::thread m_monitorThread;
    ConfigBmUi m_currentConfig;

    // Data reception buffer
    static constexpr AiUInt32 RX_BUFFER_CHUNK_SIZE = 131072; // 128KB
    std::vector<unsigned char> m_rxDataBuffer;

    // Data queue ID
    AiUInt32 m_dataQueueId;

    // UI Callbacks
    UpdateMessagesCallback m_guiUpdateMessagesCb;
    UpdateTreeItemCallback m_guiUpdateTreeItemCb;

    // Logging
    std::atomic<bool> m_dataLoggingEnabled;

    // Filtering
    std::atomic<bool> m_filterEnabled;
    std::atomic<char> m_filterBus;
    std::atomic<int> m_filterRt;
    std::atomic<int> m_filterSa;
    std::atomic<int> m_filterMc;
    std::mutex m_filterMutex;
};

// Helper functions
std::string getAIMApiErrorMessage(AiReturn errorCode);

#endif // BM_HPP