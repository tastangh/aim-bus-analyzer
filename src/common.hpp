#pragma once

#include <string>
#include <filesystem>
#include <unistd.h>
#include <linux/limits.h>

/**
 * @brief Provides a collection of common utility functions and constants
 *        for the entire application, designed as a header-only library.
 */
namespace Common {

    /**
     * @brief Retrieves the absolute path to the directory containing the running executable.
     *
     * This function is declared 'inline' to prevent multiple definition errors when
     * this header is included in multiple source files. It allows the function body
     * to reside entirely within the header.
     *
     * It specifically uses the /proc filesystem, making it a Linux-specific implementation.
     *
     * @return A std::string containing the absolute path to the executable's directory,
     *         ending with a forward slash '/'. Returns "./" on failure.
     */
    inline std::string getExecutableDirectory() {
        char result[PATH_MAX] = {0};
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);

        if (count > 0 && count < PATH_MAX) {
            // Successfully read the link, now get the parent directory
            return std::filesystem::path(result).parent_path().string() + '/';
        }
        
        // In case of an error, return the current working directory as a fallback.
        return "./"; 
    }

    /**
     * @brief A globally accessible constant holding the full, absolute path to the config.json file.
     *
     * It is declared 'inline' (a C++17 feature for variables) to ensure it is
     * defined only once across the entire program, even though it's in a header file.
     * It is constructed at startup by combining the executable's directory with the
     * relative path to the config file ("../config.json").
     */
    inline const std::string CONFIG_PATH = getExecutableDirectory() + "../config.json";

} // namespace Common