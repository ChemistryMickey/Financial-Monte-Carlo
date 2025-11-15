#pragma once

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <format>
#include <filesystem>
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