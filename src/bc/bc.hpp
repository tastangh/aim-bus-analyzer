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
  
  std::string getAIMError(AiReturn error_code);
  AiReturn sendFrame(const FrameConfig &config, std::array<AiUInt16, BC_MAX_DATA_WORDS> &data_out);
  
private:
  BC();
  ~BC();

  AiReturn setupBoardForBC();
  void convertStringDataToU16(const std::array<std::string, BC_MAX_DATA_WORDS> &str_data, AiUInt16 *u16_data_buffer, int count);
  AiReturn waitForTransferCompletion(AiUInt16 xid);

  int m_devNum = -1;
  AiUInt32 m_ulModHandle = 0;
  std::atomic<bool> m_isInitialized{false};
  std::mutex m_apiMutex;
};

#endif // AIM_BC_HPP