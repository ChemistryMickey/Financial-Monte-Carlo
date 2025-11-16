#pragma once

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <format>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <sstream>

// Environment && Path
inline const char* get_envvar(const char* envvar) {
    if (const char* var = std::getenv(envvar)) {
        return var;
    }

    std::string err_msg = std::format("Unable to find requested environment variable: {}", envvar);
    throw std::runtime_error(err_msg);
}

inline std::filesystem::path project_path(const std::filesystem::path& filename) {
    return get_envvar("PROJECT_HOME") / filename;
}

// Time
inline std::string get_current_time() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm local_tm = *std::localtime(&now_time_t);

    std::stringstream ss;
    ss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");

    return ss.str();
}