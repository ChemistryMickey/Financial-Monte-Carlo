#pragma once

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <iostream>
#include <format>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <sstream>
#include <string>
#include <vector>

// Strings
inline std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(s);

    // Use getline to split by the delimiter
    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

inline std::pair<std::string, std::string> extract_first_entry(const std::string& s, char delimiter) {
    size_t first_delimiter = s.find(delimiter);

    return {s.substr(0, first_delimiter), s.substr(first_delimiter + 1, s.size())};
}

template <typename T>
inline void print_vector(const std::vector<T>& v) {
    size_t v_size = v.size();

    std::cout << "[";
    for (size_t i = 0; i < v_size; ++i) {
        std::cout << v.at(i);

        if (i != v_size - 1) {
            std::cout << ", ";
        }
        else {
            std::cout << "]";
        }
    }
    std::cout << "\n";
}

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

inline std::filesystem::path run_out_dir(const std::filesystem::path& top_out_dir, size_t run) {
    return top_out_dir / std::format("RUN_{:09}", run);
}

inline std::chrono::sys_days string2sys_days(const std::string& s) {
    std::istringstream iss{s};
    std::chrono::sys_days days;
    iss >> std::chrono::parse("%Y-%m-%d", days);

    if (iss.fail()) {
        std::string err_msg = std::format("Invalid date format: {}", s);
        throw std::runtime_error(err_msg);
    }

    return days;
}

inline std::chrono::year_month_day string2ymd(const std::string& s) {
    return string2sys_days(s); // implicitly converted
}

namespace std {
    template<>
    struct formatter<std::filesystem::path> : formatter<std::string> {
        auto format(const std::filesystem::path& p, auto& ctx) const {
            return formatter<std::string>::format(p.string(), ctx);
        }
    };
} // namespace std