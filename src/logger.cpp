#include "logger.hpp"
#include "common.hpp"
#include <iostream> // Hata ayıklama için konsola yazdırmak amacıyla eklendi

// info, error, vs. fonksiyonları aynı kalıyor
void Logger::info(const std::string &message) { getLogger()->info(message); }
void Logger::error(const std::string &message) { getLogger()->error(message); }
void Logger::warn(const std::string &message) { getLogger()->warn(message); }
void Logger::critical(const std::string &message) { getLogger()->critical(message); }
void Logger::debug(const std::string &message) { getLogger()->debug(message); }

std::shared_ptr<spdlog::logger> Logger::getLogger() {
    // "Construct on First Use" deyimi. Bu yapı thread-safe ve sağlamdır.
    static std::shared_ptr<spdlog::logger> logger = nullptr;
    
    // Eğer logger daha önce oluşturulmadıysa (ilk çağrı)
    if (!logger) {
        try {
            // 1. Log dosyasının yazılacağı tam yolu hesapla
            std::string log_file_path = Common::getExecutableDirectory() + "1553_Bus_Monitor.log";

            // 2. HATA AYIKLAMA: Bu yolu terminale yazdırarak doğru olduğundan emin ol
            std::cout << "[DEBUG] Logger: Attempting to create log file at: " << log_file_path << std::endl;

            // 3. spdlog ile log dosyasını oluşturmayı dene
            auto daily_logger = spdlog::daily_logger_mt("BusMonitor", log_file_path, 0, 0);
            
            // 4. Ayarları yap
            daily_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%f] [%l] %v");
            daily_logger->set_level(spdlog::level::trace);
            daily_logger->flush_on(spdlog::level::trace); // Her log mesajından sonra dosyaya yaz

            // 5. Oluşturulan logger'ı statik değişkene ata
            logger = daily_logger;

            // 6. Başarılı olduğunu hem konsola hem de log dosyasına yaz
            std::cout << "[DEBUG] Logger: Initialization successful." << std::endl;
            logger->info("Logger initialized successfully.");

        } catch (const spdlog::spdlog_ex& ex) {
            // 7. Eğer spdlog bir hata fırlatırsa (örn: izin yok), hatayı terminale yazdır
            std::cerr << "[ERROR] Logger: Initialization failed! spdlog exception: " << ex.what() << std::endl;
        }
    }
  
    return logger;
}