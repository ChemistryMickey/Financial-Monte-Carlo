#pragma once
#include <functional>
#include <rttr/type>
#include <chrono>
#include <filesystem>
#include <string>
#include <map>
#include <vector>

#include "Money.hpp"

namespace fmc {
    /// @brief This is a "trait" that allows the user to log properties through inheritance.
    ///
    /// @details The inheriting class needs to do the following:
    ///
    ///     1. In their definition, have `RTTR_ENABLE(TimeseriesLoggable)` (requires <rttr/type>)
    ///
    ///     2. In their cpp file, implement `RTTR_REGISTRATION` with the properties you want to **expose** (requires <rttr/registration>)
    ///
    ///     3. (optional) if you want to log private variables, add `RTTR_REGISTRATION_FRIEND` to the definition (requires <rttr/registration_friend>)
    /// 
    ///     4. Add the object to the TimeseriesDataLogger constructor call in main.cpp
    struct TimeseriesLoggable {
        RTTR_ENABLE();
    };

    class TimeseriesDataLogger {
        static const uint BUFFER_SIZE = 100;

        std::chrono::sys_days& clock;
        std::filesystem::path log_out_path;
        std::vector<std::string> buffer;

        // Loggable types
        std::map<std::string, std::reference_wrapper<const Money>> money_signals_to_log;
        std::map<std::string, std::reference_wrapper<const double>> double_signals_to_log;

        template <typename T>
        std::string signals_to_log_to_string();

    public:
        TimeseriesDataLogger() = delete;
        TimeseriesDataLogger(
            std::chrono::sys_days& clock_,
            const std::filesystem::path& logging_json_path,
            const std::filesystem::path& logging_out_path,
            std::map<std::string, std::reference_wrapper<TimeseriesLoggable>> loggables,
            uint buffer_size = BUFFER_SIZE
        );
        ~TimeseriesDataLogger() {
            this->flush();
        }

        template <typename T>
        void log();
        void flush();
    };
}