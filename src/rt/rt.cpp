// fileName: rt.cpp
#include "rt.hpp"
#include <vector>
#include <array>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>
#include <cstring>
#include <csignal>

// spdlog kütüphanesi ve renkli konsol çıktısı için
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

// --- Global Nesneler ve Sinyal Yöneticisi ---

// Sinyal yakalayıcının erişebilmesi için global bir işaretçi
TerminalRT* g_rt_simulator_ptr = nullptr;

// Ctrl+C sinyalini yakalayıp uygulamayı durduran fonksiyon
void signal_handler(int signum) {
    if (g_rt_simulator_ptr) {
        spdlog::warn("\nKapatma sinyali (Ctrl+C) alındı. Simülasyon durduruluyor...");
        g_rt_simulator_ptr->stop();
    }
}


// --- TerminalRT Sınıfının Uygulaması ---

TerminalRT::TerminalRT(int deviceId, int rtAddress, int transmitSa)
    : m_deviceId(deviceId),
      m_rtAddress(rtAddress),
      m_transmitSa(transmitSa),
      m_boardHandle(0),
      m_isRunning(false) 
{
    // "console" adıyla renkli bir logger oluştur veya mevcut olanı kullan
    m_logger = spdlog::stdout_color_mt("console");
}

TerminalRT::~TerminalRT() {
    shutdown();
}

bool TerminalRT::checkError(AiReturn ret, const std::string& functionName) {
    if (ret != API_OK) {
        m_logger->error("{} fonksiyonu başarısız oldu. Hata Kodu: {} -> {}", 
                        functionName, ret, ApiGetErrorMessage(ret));
        return true;
    }
    return false;
}

bool TerminalRT::initialize() {
    m_logger->info("AIM kartı başlatılıyor...");
    if (ApiInit() < 1) {
        m_logger->critical("ApiInit basarisiz, kart bulunamadi.");
        return false;
    }

    TY_API_OPEN api_open_params;
    memset(&api_open_params, 0, sizeof(api_open_params));
    api_open_params.ul_Module = m_deviceId;
    api_open_params.ul_Stream = m_biuId;
    strncpy(api_open_params.ac_SrvName, "local", sizeof(api_open_params.ac_SrvName) - 1);
    
    if (checkError(ApiOpenEx(&api_open_params, &m_boardHandle), "ApiOpenEx")) return false;
    
    TY_API_RESET_INFO reset_info;
    if (checkError(ApiCmdReset(m_boardHandle, m_biuId, API_RESET_ALL, &reset_info), "ApiCmdReset")) return false;
    
    m_logger->info("RT Adresi {} için simülasyon ayarlanıyor...", m_rtAddress);
    if (checkError(ApiCmdRTIni(m_boardHandle, m_biuId, m_rtAddress, API_RT_ENABLE_SIMULATION, API_RT_RSP_BOTH_BUSSES, 12.0f, 0), "ApiCmdRTIni")) return false;

    m_logger->info("Transmit Subaddress {} için buffer (tampon) ayarlanıyor...", m_transmitSa);
    TY_API_RT_BH_INFO bh_info;
    if (checkError(ApiCmdRTBHDef(m_boardHandle, m_biuId, m_headerId, m_bufferId, 0, 0, API_QUEUE_SIZE_1, API_BQM_CYCLIC, 0, 0, 0, 0, &bh_info), "ApiCmdRTBHDef")) return false;

    m_logger->info("Subaddress {} 'Transmit' olarak aktif ediliyor...", m_transmitSa);
    if (checkError(ApiCmdRTSACon(m_boardHandle, m_biuId, m_rtAddress, m_transmitSa, m_headerId, API_RT_TYPE_TRANSMIT_SA, API_RT_ENABLE_SA, 0, 0, 0), "ApiCmdRTSACon")) return false;

    m_logger->info("RT simülasyonu başlatılıyor...");
    if (checkError(ApiCmdRTStart(m_boardHandle, m_biuId), "ApiCmdRTStart")) return false;

    m_isRunning = true;
    m_logger->info("--------------------------------------------------------------------");
    m_logger->info("Simülasyon Başarıyla Başlatıldı. RT Adresi: {}, Transmit SA: {}", m_rtAddress, m_transmitSa);
    m_logger->info("RT, harici BC simülatörünüzden komut bekliyor...");
    m_logger->info("Uygulamayı durdurmak için Ctrl+C'ye basın.");
    m_logger->info("--------------------------------------------------------------------");

    return true;
}

void TerminalRT::run() {
    if (!m_isRunning) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<AiUInt16> distrib(0, 0xFFFF);

    while (m_isRunning) {
        std::array<AiUInt16, 32> random_data;
        for (int i = 0; i < m_wordCount; ++i) {
            random_data[i] = distrib(gen);
        }

        AiUInt16 rid;
        AiUInt32 raddr;
        AiReturn ret = ApiCmdBufDef(m_boardHandle, m_biuId, API_BUF_RT_MSG, m_headerId, 0, m_wordCount, random_data.data(), &rid, &raddr);
        
        if (checkError(ret, "ApiCmdBufDef (RT data update)")) {
            stop();
            continue;
        }

        m_logger->info("RT-SA {} veri tamponu güncellendi. Hazırda bekleyen ilk kelime: 0x{:04X}", m_transmitSa, random_data[0]);

        // 5 saniye bekle
        for(int i = 0; i < 50 && m_isRunning; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void TerminalRT::stop() {
    m_isRunning = false;
}

void TerminalRT::shutdown() {
    if (m_boardHandle != 0) {
        m_logger->warn("Donanım kapatılıyor...");
        ApiCmdRTHalt(m_boardHandle, m_biuId);
        ApiClose(m_boardHandle);
        ApiExit();
        m_boardHandle = 0;
        m_logger->info("Kapatma işlemi tamamlandı.");
    }
}


// --- Uygulamanın Ana Giriş Noktası (main) ---

int main(int argc, char* argv[]) {
    signal(SIGINT, signal_handler);

    const int DEVICE_ID = 0;
    const int RT_ADDRESS = 2;
    const int TRANSMIT_SA = 3;

    TerminalRT rt_simulator(DEVICE_ID, RT_ADDRESS, TRANSMIT_SA);
    g_rt_simulator_ptr = &rt_simulator;

    if (!rt_simulator.initialize()) {
        spdlog::critical("Uygulama başlatılamadı.");
        return 1;
    }

    rt_simulator.run();

    spdlog::info("Uygulama sonlandırıldı.");
    
    return 0;
}