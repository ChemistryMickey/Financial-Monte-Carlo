#pragma once
#include "test_utils.hpp"
#include <string>
#include <filesystem>
#include <vector>

namespace fmc {
    enum class LoggingLevel {
        ERROR,
        WARNING,
        INFO,
        DEBUG
    };

    class Logger {
    public:
        Logger(const std::filesystem::path& out_path_, size_t buffer_size = 100);
        ~Logger() {
            this->flush();
        }


        /// @brief You should never have to call this function directly. Please use the DEBUG, INFO, WARN, and ERROR macros instead.
        /// @param msg 
        /// @param logging_level 
        /// @param origin_file 
        /// @param line_number 
        void log(const std::string& msg, LoggingLevel logging_level, const std::string& origin_file, size_t line_number);

        TESTING_PUBLIC;

        static const LoggingLevel cur_logging_level =
#ifdef UNIT_TESTING
            // All unit tests are executed at logging level debug
            LoggingLevel::DEBUG;
#else
            LoggingLevel::DEBUG;
#endif

        std::filesystem::path out_path;
        std::vector<std::string> buffer;
        const char* logging_level_to_string(LoggingLevel ll);
        void flush();
    };

    // This ensures there's only one logger defined across compilation units.
    // It's initialized in main.cpp and in any unit tests.
    extern Logger logger;
}
#define DEBUG(msg) logger.log(msg, fmc::LoggingLevel::DEBUG, __FILE__, __LINE__) 
#define INFO(msg) logger.log(msg, fmc::LoggingLevel::INFO, __FILE__, __LINE__) 
#define WARN(msg) logger.log(msg, fmc::LoggingLevel::WARNING, __FILE__, __LINE__) 
#define ERROR(msg) logger.log(msg, fmc::LoggingLevel::ERROR, __FILE__, __LINE__) 

