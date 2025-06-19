// fileName: bc.hpp
#pragma once

#include "common.hpp"
#include "AiOs.h"
#include "Api1553.h"

#include <atomic>
#include <mutex>

class BusController {
public:
    static BusController& getInstance();

    BusController(const BusController&) = delete;
    void operator=(const BusController&) = delete;

    AiReturn initialize(int deviceId, int streamId = 1);
    void shutdown();
    bool isInitialized() const;
    AiReturn sendAcyclicFrame(const FrameConfig& config, std::array<AiUInt16, BC_MAX_DATA_WORDS>& receivedData);
    static const char* getAIMError(AiReturn ret);

private:
    BusController() = default;
    ~BusController();

    std::mutex m_apiMutex;
    std::atomic<bool> m_isInitialized{false};
    AiUInt32 m_boardHandle = 0;
    int m_deviceId = 0;
    int m_streamId = 0;
    const int m_biuId = 0;
    
    AiUInt32 m_nextTransferId = 1;
    AiUInt32 m_nextHeaderId = 1;
    AiUInt32 m_nextBufferId = 1;
};