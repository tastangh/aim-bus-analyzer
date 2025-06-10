#ifndef BM_HPP
#define BM_HPP

#include "Api1553.h"
#include <string>
#include <vector>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>

// UI tarafından BM sınıfına konfigürasyon geçirmek için
typedef struct ConfigBmUi
{
  AiUInt32 ulDevice;
  AiUInt32 ulStream;
  AiUInt8  ulCoupling;
} ConfigBmUi;

class BM {
public:
    static BM& getInstance();

    BM(const BM&) = delete;
    BM& operator=(const BM&) = delete;

    using UpdateMessagesCallback = std::function<void(const std::string& formattedMessages)>;
    using UpdateTreeItemCallback = std::function<void(char bus, int rt, int sa, bool isActive)>;

    AiReturn start(const ConfigBmUi& config);
    void stop();
    bool isMonitoring() const;

    void setUpdateMessagesCallback(UpdateMessagesCallback cb);
    void setUpdateTreeItemCallback(UpdateTreeItemCallback cb);

    void enableFilter(bool enable);
    bool isFilterEnabled() const;
    void setFilterCriteria(char bus, int rt, int sa);

private:
    BM();
    ~BM();

    // Veri işleme için dahili yardımcı yapı ve fonksiyonlar
    struct MessageTransaction {
        uint64_t full_timetag = 0;
        AiUInt32 last_timetag_l_data = 0;
        AiUInt32 last_timetag_h_data = 0;
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

    // Arka plan iş parçacığı ve API yönetimi
    void monitorThreadFunc();
    void processAndRelayData(const unsigned char* buffer, AiUInt32 bytesRead);
    AiReturn initializeBoard(const ConfigBmUi& config);
    void shutdownBoard();
    AiReturn configureBusMonitor(const ConfigBmUi& config);
    AiReturn openDataQueue();
    void closeDataQueue();

    // Üye değişkenler
    AiUInt32 m_ulModHandle;
    ConfigBmUi m_currentConfig;

    std::thread m_monitorThread;
    std::atomic<bool> m_monitoringActive;
    std::atomic<bool> m_shutdownRequested;

    UpdateMessagesCallback m_guiUpdateMessagesCb;
    UpdateTreeItemCallback m_guiUpdateTreeItemCb;

    std::atomic<bool> m_filterEnabled;
    std::atomic<char> m_filterBus;
    std::atomic<int>  m_filterRt;
    std::atomic<int>  m_filterSa;
    std::mutex        m_filterMutex;

    AiUInt32 m_dataQueueId;
    std::vector<unsigned char> m_rxDataBuffer;
    const AiUInt32 RX_BUFFER_CHUNK_SIZE = 16 * 1024;
};

std::string getAIMApiErrorMessage(AiReturn errorCode);

#endif // BM_HPP