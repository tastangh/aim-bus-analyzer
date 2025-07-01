// fileName: bm.hpp
#pragma once

#include "bm_common.hpp"
#include <string>
#include <functional>

// Arayüzün derlenmesi için BusMonitor sınıfının temel tanımı.
class BM {
public:
    using UpdateMessagesCallback = std::function<void(const std::string&)>;
    using UpdateTreeItemCallback = std::function<void(char, int, int, bool)>;

    static BM& getInstance() {
        static BM instance;
        return instance;
    }

    bool isMonitoring() const { return m_isMonitoring; }
    bool isFilterEnabled() const { return m_isFilterEnabled; }
    
    // Backend yazıldığında doldurulacak fonksiyonlar
    void stop() { m_isMonitoring = false; }
    int start(const ConfigBmUi& config) { m_isMonitoring = true; return 0; }
    void setFilterCriteria(char bus, int rt, int sa, int mc) {}
    void enableFilter(bool enable) { m_isFilterEnabled = enable; }
    void enableDataLogging(bool enable) {}
    void setUpdateMessagesCallback(const UpdateMessagesCallback& cb) {}
    void setUpdateTreeItemCallback(const UpdateTreeItemCallback& cb) {}

private:
    BM() = default;
    bool m_isMonitoring = false;
    bool m_isFilterEnabled = false;
};