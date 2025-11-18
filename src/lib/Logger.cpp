#include "Logger.hpp"
#include "utils.hpp"

#include <fstream>
#include <format>
#include <cstdlib>
#include <print>

namespace fmc {
    void Logger::initialize(const CliArgs& args, size_t buffer_size) {
        this->out_path = args.out_directory / "output.log";

        if (!std::filesystem::exists(args.out_directory)) {
            std::filesystem::create_directories(args.out_directory);
        }
        if (std::filesystem::exists(this->out_path)) {
            std::filesystem::remove(this->out_path);
        }

        if (args.verbose) {
            this->cur_logging_level = LoggingLevel::DEBUG;
            this->buffer.reserve(1); // dump to file as much as possible
        }
        else {
            this->buffer.reserve(buffer_size);
        }
        this->initialized = true;

        INFO("Logger successfully initialized, outputting to {}", this->out_path);
    }

    void Logger::initialize(const std::filesystem::path& out_path_, size_t buffer_size) {
        this->out_path = project_path(out_path_);

        this->buffer.reserve(buffer_size);

        if (std::filesystem::exists(this->out_path)) {
            std::filesystem::remove(this->out_path);
        }

        this->initialized = true;
    }

    void Logger::log(const std::string& msg, LoggingLevel logging_level, const std::string& origin_file, size_t line_number) {
        if (logging_level > Logger::cur_logging_level) {
            return;
        }

        std::string tagged_msg = std::format("[{}, {}, {}, {}]: {}",
            get_current_time(),
            this->logging_level_to_string(logging_level),
            origin_file,
            line_number,
            msg
        );

        if (!this->initialized) {
            std::string err_msg = std::format("Attempting to use the logger before it's initialized! {}", tagged_msg);
            throw std::runtime_error(err_msg);
        }


#ifndef BENCHMARKING
        std::println("{}", tagged_msg);
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