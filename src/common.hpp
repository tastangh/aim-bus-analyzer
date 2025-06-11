#pragma once

#include <string>
#include <filesystem>
#include <unistd.h>
#include <linux/limits.h>

namespace Common {

    /**
     * @brief Returns the absolute path of the directory containing the running executable.
     * @return A std::string like ".../bin/", ending with a platform-appropriate separator.
     *         Returns "./" (current working directory) on error.
     */
    inline std::string getExecutableDirectory() {
        char result[PATH_MAX] = {0};
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);

        if (count > 0 && count < PATH_MAX) {
            return std::filesystem::path(result).parent_path().string() + '/';
        }
        
        return "./"; 
    }

    /**
     * @brief Returns the path to the project's root directory (one level above 'bin').
     * @return A std::string like ".../aim-bus-analyzer/", ending with a separator.
     */
    inline std::string getProjectRootDirectory() {
        std::filesystem::path binPath(getExecutableDirectory());
        // === THIS IS THE FIX ===
        // Call parent_path() twice to go up from ".../bin/" to ".../"
        return binPath.parent_path().parent_path().string() + '/';
    }

    /**
     * @brief Returns the full path to the config.json file.
     */
    inline std::string getConfigPath() {
        return getProjectRootDirectory() + "config.json";
    }

} // namespace Common