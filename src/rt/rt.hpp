// fileName: rt.hpp
#pragma once

#include "app.hpp"
#include "Api1553.h"
#include <atomic>
#include <mutex>

class FrameComponent;

class RemoteTerminal {
public:
    static RemoteTerminal& getInstance();
    RemoteTerminal(const RemoteTerminal&) = delete;
    void operator=(const RemoteTerminal&) = delete;

    AiReturn start(int deviceId, int rtAddress, int streamId = 1);
    void shutdown();
    bool isRunning() const;

    AiReturn defineFrameAsSubaddress(FrameComponent* component);
    AiReturn testTransmitSubaddress(const FrameComponent* component);

    static const char* getAIMError(AiReturn ret);

private:
    RemoteTerminal() = default;
    ~RemoteTerminal();

    std::mutex m_apiMutex;
    std::atomic<bool> m_isRunning{false};
    AiUInt32 m_boardHandle = 0;
    int m_deviceId = 0;
    int m_rtAddress = 0;
    const int m_biuId = 0;

    AiUInt32 m_nextHeaderId = 1;
    AiUInt32 m_nextBufferId = 1;
    
    // YENİ: Donanımdan okunan maksimum ID'leri saklamak için
    AiUInt32 m_maxRtHeaderId = 0;
    AiUInt32 m_maxBcHeaderId = 0;

    AiUInt16 m_testBcHeaderId = 0;
    AiUInt16 m_testBcBufferId = 0;
};