#ifndef AIM_BC_HPP
#define AIM_BC_HPP

#include "../common.hpp"
#include "Api1553.h"
#include <array>
#include <atomic>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class BC {
public:
  static BC &getInstance();
  BC(const BC &) = delete;
  BC &operator=(const BC &) = delete;

  AiReturn initialize(int devNum);
  void shutdown();
  bool isInitialized() const;
  
  // DÜZELTME: Fonksiyon imzası tekrar 2 parametreli hale getirildi.
  AiReturn sendAcyclicFrame(const FrameConfig &config, std::array<AiUInt16, BC_MAX_DATA_WORDS> &data_out);

  void startCyclicSend(const std::vector<FrameConfig>& activeFrames, std::chrono::milliseconds interval);
  void stopCyclicSend();
  bool isCyclicSending() const;
  
  std::string getAIMError(AiReturn error_code);

private:
  BC();
  ~BC();

  AiReturn setupBoardForBC();
  AiReturn setupFrameSchedule(const std::vector<FrameConfig>& frames);
  AiReturn waitForTransferCompletion(AiUInt16 xid);
  void cyclicSendLoop();
  
  void convertStringDataToU16(const std::array<std::string, BC_MAX_DATA_WORDS> &str_data, AiUInt16 *u16_data_buffer, int count);
  
  int m_devNum = -1;
  AiUInt32 m_ulModHandle = 0;
  std::atomic<bool> m_isInitialized{false};
  std::mutex m_apiMutex;

  std::thread m_cyclicSendThread;
  std::atomic<bool> m_isCyclicSending{false};
  std::vector<FrameConfig> m_cyclicFrames;
  std::chrono::milliseconds m_cyclicInterval;
};

#endif // AIM_BC_HPP