#pragma once
#include "test_utils.hpp"
#include <string>
#include <filesystem>
#include <vector>
#include <mutex>

namespace fmc {
    enum class LoggingLevel {
        ERROR,
        WARNING,
        INFO,
        DEBUG
    };

    class Logger {
    public:
        static constexpr size_t BUFFER_SIZE = 100;

        Logger() = default;
        ~Logger() {
            this->flush();
        }

        /// @brief Get the global instance of the logger. You should never use this function directly. Please use the provided macros.
        /// @return 
        static Logger& instance() {
            static Logger inst;
            return inst;
        }


        /// @brief You should never have to call this function directly. Please use the DEBUG, INFO, WARN, and ERROR macros instead.
        /// @param msg 
        /// @param logging_level 
        /// @param origin_file 
        /// @param line_number 
        void log(const std::string& msg, LoggingLevel logging_level, const std::string& origin_file, size_t line_number);
        void flush();
        static void set_logging_level(LoggingLevel l) {
            Logger::instance().cur_logging_level = l;
        }

        /// @brief Do late initialization of the logger such that there can be a global one that's configured in main
        /// @param args CLI arguments
        /// @param buffer_size The size of the buffer before dumping
        void initialize(const std::filesystem::path& out_path, size_t buffer_size = BUFFER_SIZE, bool verbose = false);

        TESTING_PUBLIC;

        LoggingLevel cur_logging_level =
#ifdef UNIT_TESTING
            // All unit tests are executed at logging level debug
            LoggingLevel::DEBUG;
#else
            LoggingLevel::INFO;
#endif

        std::filesystem::path out_path;
        std::vector<std::string> buffer;
        bool initialized = false;
        const char* logging_level_to_string(LoggingLevel ll);
        std::mutex mutex;
    };
}

#define DEBUG(pattern, ...) fmc::Logger::instance().log(std::format((pattern) __VA_OPT__(,) __VA_ARGS__), fmc::LoggingLevel::DEBUG, __FILE__, __LINE__) 
#define INFO(pattern, ...) fmc::Logger::instance().log(std::format((pattern) __VA_OPT__(,) __VA_ARGS__), fmc::LoggingLevel::INFO, __FILE__, __LINE__) 
#define WARN(pattern, ...) fmc::Logger::instance().log(std::format((pattern) __VA_OPT__(,) __VA_ARGS__), fmc::LoggingLevel::WARNING, __FILE__, __LINE__) 
#define ERROR(pattern, ...) {std::string err_msg = std::format((pattern) __VA_OPT__(,) __VA_ARGS__); fmc::Logger::instance().log(err_msg, fmc::LoggingLevel::ERROR, __FILE__, __LINE__); fmc::Logger::instance().flush(); throw std::runtime_error(err_msg);}

