#ifndef AIM_BC_HPP
#define AIM_BC_HPP

#include "Api1553.h"
#include <array>
#include <string>
#include <atomic>
#include "common.hpp"

class BC {
public:
    static BC& getInstance();
    BC(const BC&) = delete;
    BC& operator=(const BC&) = delete;

    AiReturn initialize(int devNum);
    void shutdown();
    bool isInitialized() const { return m_isInitialized.load(); }
    
    AiReturn sendAcyclicFrame(BcMode mode, char bus, int rtTx, int saTx, int rtRx, int saRx, int wc, 
                              const std::array<std::string, RT_SA_MAX_COUNT>& data_in, 
                              std::array<AiUInt16, RT_SA_MAX_COUNT>& data_out);

private:
    BC();
    ~BC();

    int m_devNum = -1;
    AiUInt32 m_ulModHandle = 0;
    std::atomic<bool> m_isInitialized{false};
    
    AiReturn setupBoardForBC();
    std::string getAIMError(AiReturn error_code);
    void convertStringDataToU16(const std::array<std::string, RT_SA_MAX_COUNT>& str_data, AiUInt16* u16_data_buffer, int count);
};

#endif // AIM_BC_HPP