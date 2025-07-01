// fileName: bc_common.hpp
#pragma once

#include <string>
#include <array>

// Maksimum data word sayısını tanımla
constexpr int BC_MAX_DATA_WORDS = 32;

// BC transfer modlarını tanımlayan enum
enum class BcMode {
    BC_TO_RT,
    RT_TO_BC,
    RT_TO_RT,
    MODE_CODE_NO_DATA,
    MODE_CODE_WITH_DATA
};

// Bir BC çerçevesinin tüm yapılandırmasını tutan veri yapısı
struct FrameConfig {
    std::string label;
    char bus;
    int rt;
    int sa;
    int rt2;
    int sa2;
    int wc;
    BcMode mode;
    std::array<std::string, BC_MAX_DATA_WORDS> data;
};