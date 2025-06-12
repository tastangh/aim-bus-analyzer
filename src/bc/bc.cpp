#include "bc.hpp"
#include "logger.hpp"
#include <chrono>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <thread>

constexpr AiUInt16 XFER_ID_BC_TO_RT = 1;
constexpr AiUInt16 XFER_ID_RT_TO_BC = 2;
constexpr AiUInt16 XFER_ID_RT_TO_RT = 3;
constexpr AiUInt16 XFER_ID_MODE_CODE = 4;
constexpr AiUInt16 SHARED_BUFFER_ID = 1;
constexpr int POLLING_TIMEOUT_MS = 200;
constexpr AiReturn API_ERR_NOT_INITIALIZED = API_ERR_NAK;

BC &BC::getInstance() {
  static BC instance;
  return instance;
}

BC::BC() {}
BC::~BC() { shutdown(); }

AiReturn BC::initialize(int devNum) {
  std::lock_guard<std::mutex> lock(m_apiMutex);
  if (m_isInitialized.load() && m_devNum == devNum) return API_OK;
  if (m_isInitialized.load()) { ApiClose(m_ulModHandle); }

  m_devNum = devNum;
  Logger::info("Initializing Bus Controller for AIM device: " + std::to_string(m_devNum));

  AiReturn ret = ApiInit();
  if (ret < 0) { Logger::error("BC: ApiInit failed. " + getAIMError(ret)); return ret; }

  TY_API_OPEN apiOpen;
  memset(&apiOpen, 0, sizeof(apiOpen));
  apiOpen.ul_Module = devNum; apiOpen.ul_Stream = 1; strcpy(apiOpen.ac_SrvName, "local");
  
  ret = ApiOpenEx(&apiOpen, &m_ulModHandle);
  if (ret != API_OK) { Logger::error("BC: ApiOpenEx failed. " + getAIMError(ret)); m_ulModHandle = 0; return ret; }

  ret = ApiCmdReset(m_ulModHandle, (AiUInt8)apiOpen.ul_Stream, API_RESET_ALL, nullptr);
  if (ret != API_OK) { Logger::error("BC: ApiCmdReset failed. " + getAIMError(ret)); ApiClose(m_ulModHandle); m_ulModHandle = 0; return ret; }

  ret = setupBoardForBC();
  if (ret != API_OK) { ApiClose(m_ulModHandle); m_ulModHandle = 0; return ret; }

  m_isInitialized = true;
  Logger::info("BC Initialized successfully for device " + std::to_string(devNum));
  return API_OK;
}

void BC::shutdown() {
  std::lock_guard<std::mutex> lock(m_apiMutex);
  if (!m_isInitialized.load()) return;

  if (m_ulModHandle != 0) {
    ApiCmdBCHalt(m_ulModHandle, 1);
    ApiClose(m_ulModHandle);
    Logger::info("BC Shutdown complete for device: " + std::to_string(m_devNum));
  }
  m_ulModHandle = 0; m_devNum = -1; m_isInitialized = false;
}

bool BC::isInitialized() const { return m_isInitialized.load(); }

AiReturn BC::setupBoardForBC() {
  Logger::debug("Configuring board for Bus Controller mode...");
  AiReturn ret = ApiCmdBCIni(m_ulModHandle, 1, 0, 0, 0, 0);
  if (ret != API_OK) { Logger::error("BC: ApiCmdBCIni failed. " + getAIMError(ret)); }
  return ret;
}

AiReturn BC::sendFrame(const FrameConfig &config, std::array<AiUInt16, BC_MAX_DATA_WORDS> &data_out) {
  if (!m_isInitialized.load()) return API_ERR_NOT_INITIALIZED;
  std::lock_guard<std::mutex> lock(m_apiMutex);

  TY_API_BC_XFER xfer;
  memset(&xfer, 0, sizeof(xfer));
  xfer.chn = (config.bus == 'A' ? API_BC_XFER_BUS_PRIMARY : API_BC_XFER_BUS_SECONDARY);
  xfer.hid = SHARED_BUFFER_ID;

  switch (config.mode) {
    case BcMode::BC_TO_RT: xfer.xid = XFER_ID_BC_TO_RT; xfer.type = API_BC_TYPE_BCRT; xfer.rcv_rt = config.rt; xfer.rcv_sa = config.sa; xfer.wcnt = config.wc; break;
    case BcMode::RT_TO_BC: xfer.xid = XFER_ID_RT_TO_BC; xfer.type = API_BC_TYPE_RTBC; xfer.xmt_rt = config.rt; xfer.xmt_sa = config.sa; xfer.wcnt = config.wc; break;
    case BcMode::RT_TO_RT: xfer.xid = XFER_ID_RT_TO_RT; xfer.type = API_BC_TYPE_RTRT; xfer.xmt_rt = config.rt; xfer.xmt_sa = config.sa; xfer.rcv_rt = config.rt2; xfer.rcv_sa = config.sa2; xfer.wcnt = config.wc; break;
    case BcMode::MODE_CODE_NO_DATA: case BcMode::MODE_CODE_WITH_DATA:
      xfer.xid = XFER_ID_MODE_CODE; xfer.type = API_BC_TYPE_BCRT; xfer.rcv_rt = config.rt; xfer.rcv_sa = (config.sa == 31) ? 31 : 0; xfer.wcnt = config.sa; break;
  }

  AiReturn ret = ApiCmdBCBHDef(m_ulModHandle, 1, xfer.hid, 0, 0, 0, 1, 0, 0, 0, 0, 0, nullptr);
  if (ret != API_OK) return ret;
  
  ret = ApiCmdBCXferDef(m_ulModHandle, 1, &xfer, nullptr);
  if (ret != API_OK) return ret;

  int word_count_to_send = (config.mode == BcMode::BC_TO_RT) ? ((config.wc == 0) ? 32 : config.wc) : (config.mode == BcMode::MODE_CODE_WITH_DATA) ? 1 : 0;
  if (word_count_to_send > 0) {
    AiUInt16 data_buffer[BC_MAX_DATA_WORDS] = {0};
    convertStringDataToU16(config.data, data_buffer, word_count_to_send);
    ret = ApiCmdBufWriteBlock(m_ulModHandle, 1, API_BUF_BC_MSG, xfer.hid, 0, 0, word_count_to_send, data_buffer);
    if (ret != API_OK) return ret;
  }

  TY_API_BC_XFER_DSP xfer_status;
  ApiCmdBCXferRead(m_ulModHandle, 1, xfer.xid, 0x7, &xfer_status);

  TY_API_BC_FW_INSTR instr_list[1] = {0};
  instr_list[0].op = API_BC_FWI_XFER;
  instr_list[0].par1 = xfer.xid;
  
  uintptr_t instr_addr_val = reinterpret_cast<uintptr_t>(instr_list);
  ret = ApiCmdBCStart(m_ulModHandle, 1, API_BC_START_INSTR_TABLE, 1, 0.0f,
    static_cast<AiUInt32>(instr_addr_val), nullptr, nullptr);


  
  if (ret != API_OK) return ret;

  ret = waitForTransferCompletion(xfer.xid);
  ApiCmdBCHalt(m_ulModHandle, 1);
  if (ret != API_OK) return ret;

  int word_count_to_read = (config.mode == BcMode::RT_TO_BC || config.mode == BcMode::RT_TO_RT) ? ((config.wc == 0) ? 32 : config.wc) : 0;
  if (word_count_to_read > 0) {
    ret = ApiCmdBufRead(m_ulModHandle, 1, API_BUF_BC_MSG, xfer.hid, 0, word_count_to_read, data_out.data(), nullptr, nullptr);
    if (ret != API_OK) return ret;
  }
  return API_OK;
}

AiReturn BC::waitForTransferCompletion(AiUInt16 xid) {
    auto startTime = std::chrono::steady_clock::now();
    TY_API_BC_XFER_DSP xfer_status;
    while(true) {
        AiReturn ret = ApiCmdBCXferRead(m_ulModHandle, 1, xid, 0, &xfer_status);
        if (ret != API_OK) { Logger::error("BC: ApiCmdBCXferRead failed during polling. " + getAIMError(ret)); return ret; }
        if (xfer_status.brw & 0x01) { 
            if (xfer_status.brw & 0x80) { Logger::warn("Transfer XID " + std::to_string(xid) + " completed with an error. Report Word: 0x" + std::to_string(xfer_status.brw)); }
            return API_OK;
        }
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() > POLLING_TIMEOUT_MS) {
            Logger::error("Transfer XID " + std::to_string(xid) + " timed out after " + std::to_string(POLLING_TIMEOUT_MS) + " ms.");
            return API_ERR_TIMEOUT;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(200));
    }
}

void BC::convertStringDataToU16(const std::array<std::string, BC_MAX_DATA_WORDS>& str_data, AiUInt16* u16_data_buffer, int count) {
    for (int i = 0; i < count; ++i) {
        try { u16_data_buffer[i] = static_cast<AiUInt16>(std::stoul(str_data.at(i), nullptr, 16)); } catch (...) { u16_data_buffer[i] = 0; }
    }
}

std::string BC::getAIMError(AiReturn error_code) {
    const char* msg = ApiGetErrorMessage(error_code);
    return msg ? std::string(msg) : "Unknown AIM API Error";
}