// fileName: rt.hpp
#pragma once

#include <atomic>
#include <string>
#include <memory>

// AIM Kütüphanesi
#include "Api1553.h"

// spdlog kütüphanesi için ileriye dönük bildirim (forward declaration)
namespace spdlog {
    class logger;
}

class TerminalRT {
public:
    // Yapıcı ve yıkıcı fonksiyonlar
    TerminalRT(int deviceId, int rtAddress, int transmitSa);
    ~TerminalRT();

    // Donanımı başlatır ve RT'yi yapılandırır
    bool initialize();

    // Veri tamponunu periyodik olarak güncelleyen ana döngü
    void run();

    // Simülasyonu güvenli bir şekilde durdurur
    void stop();

private:
    // Donanım kaynaklarını serbest bırakan fonksiyon
    void shutdown();

    // Hata kontrolü için yardımcı fonksiyon
    bool checkError(AiReturn ret, const std::string& functionName);

    // Sınıf üyeleri
    int m_deviceId;
    int m_rtAddress;
    int m_transmitSa;
    const int m_wordCount = 32;
    
    AiUInt32 m_boardHandle;
    const int m_biuId = 1;

    // Bu tek alt adres için sabit donanım ID'leri
    const AiUInt16 m_headerId = 1;
    const AiUInt16 m_bufferId = 1;

    std::atomic<bool> m_isRunning;
    
    // spdlog için logger nesnesi
    std::shared_ptr<spdlog::logger> m_logger;
};