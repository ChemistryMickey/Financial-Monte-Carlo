#include "Logger.hpp"
#include "utils.hpp"

#include <fstream>
#include <format>
#include <chrono>
#include <ctime>
#include <sstream>
#include <cstdlib>
#include <print>

namespace fmc {
    Logger::Logger(const std::filesystem::path& out_path_, size_t buffer_size) {
        this->out_path = get_envvar("PROJECT_HOME") / out_path_;

        this->buffer.reserve(buffer_size);

        if (std::filesystem::exists(this->out_path)) {
            std::filesystem::remove(this->out_path);
        }
    }

    void Logger::log(const std::string& msg, LoggingLevel logging_level, const std::string& origin_file, size_t line_number) {
        if (logging_level > Logger::cur_logging_level) {
            return;
        }

        auto now = std::chrono::system_clock::now();
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

        std::tm local_tm = *std::localtime(&now_time_t);

        std::stringstream ss;
        ss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
        std::string tagged_msg = std::format("[{}, {}, {}, {}]: {}",
            ss.str(),
            this->logging_level_to_string(logging_level),
            origin_file,
            line_number,
            msg
        );

#ifndef BENCHMARKING
        std::print("{}\n", tagged_msg);
#endif

        buffer.push_back(tagged_msg);
        if (buffer.size() < buffer.capacity()) {
            return;
        }

        this->flush();

    }

    void Logger::flush() {
        std::ofstream f{this->out_path, std::ios_base::app};
        for (auto& m : this->buffer) {
            f << m << "\n";
        }

        this->buffer.clear();
    }

    const char* Logger::logging_level_to_string(LoggingLevel ll) {
        switch (ll) {
        case LoggingLevel::DEBUG:
            return "DEBUG\033[0m";
        case LoggingLevel::INFO:
            return "\033[0;34mINFO\033[0m";
        case LoggingLevel::WARNING:
            return "\033[0;33mWARN\033[0m";
        case LoggingLevel::ERROR:
            return "\033[0;31mERROR\033[0m";
        }

        throw std::runtime_error("Unrecognized logging level!");
    }

}