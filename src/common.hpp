#pragma once

#include <string>
#include <filesystem>
#include <unistd.h>
#include <linux/limits.h>
#include <array>
#include <cstdint>
#include <vector>

constexpr int BC_MAX_DATA_WORDS = 32;
constexpr int TOP_BAR_COMP_HEIGHT = 28;
constexpr unsigned int BC_FRAME_TIME_MS = 100;

enum class BcMode {
    BC_TO_RT,
    RT_TO_BC,
    RT_TO_RT,
    MODE_CODE_NO_DATA,
    MODE_CODE_WITH_DATA
};

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

namespace Common {
    inline std::string getExecutableDirectory() {
        char result[PATH_MAX] = {0};
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        if (count > 0 && count < PATH_MAX) {
            return std::filesystem::path(result).parent_path().string() + '/';
        }
        return "./"; 
    }

    inline std::string getProjectRootDirectory() {
        std::filesystem::path execPath(getExecutableDirectory());
        if (!execPath.empty() && execPath.has_parent_path() && execPath.parent_path() != execPath) {
             return execPath.parent_path().string();
        }
        return "./";
    }

    inline std::string getConfigPath() {
        return getProjectRootDirectory() + "config.json";
    }

    inline std::string getLogPath() {
        return getExecutableDirectory() + "BusController.log";
    }
}