#pragma once

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <format>
inline const char* get_envvar(const char* envvar) {
    if (const char* var = std::getenv(envvar)) {
        return var;
    }

    std::string err_msg = std::format("Unable to find requested environment variable: {}", envvar);
    throw std::runtime_error(err_msg);
}