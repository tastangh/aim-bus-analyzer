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

    void monitorThreadFunc();

    AiReturn initializeBoard(const ConfigBmUi& config);
    void shutdownBoard();
    AiReturn configureBusMonitor(const ConfigBmUi& config);
    AiReturn openDataQueue();
    void closeDataQueue();

    void processAndRelayData(const unsigned char* buffer, AiUInt32 bytesRead);
    bool applyFilterLogic(AiUInt8 type, AiUInt16 busWord); // Filtreleme mantığı için

    // Veri ayrıştırma yardımcı fonksiyonları (string döndüren versiyonlar)
    void helperParseCommandWord(AiUInt16 cmdWord, std::string& out);
    void helperParseStatusWord(AiUInt16 statusWord, std::string& out);
    void helperParseDataWord(AiUInt16 dataWord, std::string& out);
    void helperParseBusWordEntry(AiUInt32 entryData, AiUInt8 type, std::string& out);
    void helperParseErrorWordEntry(AiUInt32 entryData, std::string& out);
    void helperParseTimeTagLowEntry(AiUInt32 entryData, std::string& out);
    void helperParseTimeTagHighEntry(AiUInt32 entryData, std::string& out);

    AiUInt32 m_ulModHandle;
    ConfigBmUi m_currentConfig;

    std::thread m_monitorThread;
    std::atomic<bool> m_monitoringActive;
    std::atomic<bool> m_shutdownRequested;

    UpdateMessagesCallback m_guiUpdateMessagesCb;
    UpdateTreeItemCallback m_guiUpdateTreeItemCb;

    std::atomic<bool> m_filterEnabled;
    std::atomic<char> m_filterBus; // 'A' veya 'B', ya da 0 (filtre yok)
    std::atomic<int>  m_filterRt;  // 0-30, ya da -1 (filtre yok)
    std::atomic<int>  m_filterSa;  // 0-30, ya da -1 (filtre yok)
    std::mutex        m_filterMutex;

    AiUInt32 m_dataQueueId;
    std::vector<unsigned char> m_rxDataBuffer;
    const AiUInt32 RX_BUFFER_CHUNK_SIZE = 16 * 1024;
};

std::string getAIMApiErrorMessage(AiReturn errorCode);

#endif // BM_HPP