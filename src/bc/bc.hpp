#ifndef AIM_BC_HPP
#define AIM_BC_HPP

#include "Api1553.h"
#include <string>
#include <stdexcept>
#include <vector>
#include <chrono>

// Hata kontrolü için bir exception sınıfı
class AimException : public std::runtime_error {
public:
    AimException(const std::string& message) : std::runtime_error(message) {}
};

class BC {
public:
    // Singleton pattern'i ile tek bir BC nesnesi olmasını sağlıyoruz
    static BC& getInstance() {
        static BC instance;
        return instance;
    }

    // Initialize ve Shutdown fonksiyonları
    void initialize(int devNum);
    void shutdown();

    // Veri gönderme fonksiyonu
    void sendDataPeriodically(const std::vector<AiUInt16>& initial_data, int period_ms, int number_of_sends);

    // Kopyalama ve atama operatörlerini siliyoruz
    BC(const BC &) = delete;
    BC& operator=(const BC &) = delete;

private:
    BC();  // Kurucu fonksiyon (constructor) özel
    ~BC(); // Yıkıcı fonksiyon (destructor) özel

    void checkApiReturn(AiReturn ret, const std::string& function_name);

    int m_devNum = -1;
    AiUInt32 m_boardHandle = 0;
    bool m_isInitialized = false;
    bool m_isBoardOpen = false;

    // Sabitler
    const int BIU_NUMBER = 1;
    const AiUInt16 XFER_ID_BC_TO_RT = 1;
    const AiUInt16 BUFFER_ID = 1;
};

#endif // AIM_BC_HPP