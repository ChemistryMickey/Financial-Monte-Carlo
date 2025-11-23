#include <chrono>
#include <filesystem>
#include <string>
#include <functional>
#include <rttr/type>
#include <iostream>
#include <print>
#include <ranges>

#include "TimeseriesDataLogger.hpp"
#include "Logger.hpp"
#include "file_io.hpp"

namespace fmc {
    TimeseriesDataLogger::TimeseriesDataLogger(
        std::chrono::sys_days& clock_,
        const std::filesystem::path& logging_json_path,
        const std::filesystem::path& logging_out_path,
        std::map<std::string, std::reference_wrapper<TimeseriesLoggable>> loggables,
        uint buffer_size
    ) : clock{clock_}, log_out_path{logging_out_path} {
        this->buffer.reserve(buffer_size);

        auto logging_config = read_json(logging_json_path);

        // Iterate through the map of loggables and detect if it's in the logging config.
        // If so, get a reference to its property and add it to your signals_to_log map
        for (auto& [signal, mapped_name] : logging_config.items()) {
            // DEBUG("Attempting to register {}", signal);
            auto split_signal = split(signal, '.');
            if (!loggables.contains(split_signal.at(0))) {
                ERROR("Loggable {} not present in loggables. Please register {}",
                    split_signal[0], split_signal[0]
                );
            }

            auto& obj = loggables.at(split_signal[0]).get();
            rttr::instance inst{obj};
            rttr::type obj_type = inst.get_derived_type();

            // Kinda jank to just do this; it should probably be a map.
            auto val_variant{obj_type.get_property(split_signal[1]).get_value(obj)};
            auto val_type = val_variant.get_type();
            if (val_type == rttr::type::get<std::reference_wrapper<const Money>>()) {
                this->money_signals_to_log.emplace(mapped_name, val_variant.get_value<std::reference_wrapper<const Money>>());
            }
            else if (val_type == rttr::type::get<std::reference_wrapper<const double>>()) {
                this->double_signals_to_log.emplace(mapped_name, val_variant.get_value<std::reference_wrapper<const double>>());
            }
            else if (val_type == rttr::type::get<std::reference_wrapper<const long long int>>()) {
                this->int_signals_to_log.emplace(mapped_name, val_variant.get_value<std::reference_wrapper<const long long int>>());
            }
            else {
                ERROR("Type {} is not supported by the Timeseries Data Logger", val_type.get_name().to_string());
            }
        }

        // Write the header of the CSV so you don't need to do it again.
        // But write the name-mapped columns. It doesn't matter in the logging, the order is preserved.

        /// WARNING: This MUST be in the same order as the logging function or else columns won't line up!
        std::string csv_header = "Date";
        if (!this->money_signals_to_log.empty()) {
            csv_header += "," + this->csv_header_string<Money>(this->money_signals_to_log);
        }
        if (!this->double_signals_to_log.empty()) {
            csv_header += "," + this->csv_header_string<double>(this->double_signals_to_log);
        }
        if (!this->int_signals_to_log.empty()) {
            csv_header += "," + this->csv_header_string<long long int>(this->int_signals_to_log);
        }

        std::ofstream f{logging_out_path, std::fstream::out}; // Completely overwrites file
        std::println(f, "{}", csv_header);

        DEBUG("Initialized TimeseriesDataLogger outputting at {}", logging_out_path);
    }

    void TimeseriesDataLogger::flush() {
        std::string batched_out_str = std::ranges::to<std::string>(
            this->buffer
            | std::views::join_with('\n')
        );

        std::ofstream f{this->log_out_path, std::fstream::app}; // Just append
        std::println(f, "{}", batched_out_str);

        this->buffer.clear();
    }

    // Templated functions:
    template <typename T>
    std::string TimeseriesDataLogger::csv_header_string(const RefMap<T>& map) const {
        return std::format("{}",
            std::ranges::to<std::string>(
                map
                | std::views::transform([](auto& p) {
                    return std::format("{}", p.first);
                    })
                | std::views::join_with(',')
            )
        );
    }

    template <typename T>
    std::string TimeseriesDataLogger::signals_to_log_to_string(const RefMap<T>& signals_to_log) const {
        return std::format("{}",
            std::ranges::to<std::string>(
                signals_to_log
                | std::views::transform([](auto& p) {
                    return std::format("{}", p.second.get());
                    })
                | std::views::join_with(','))
        );
    }

    void TimeseriesDataLogger::log() {
        std::string out = std::format("{}", this->clock);

        /// WARNING: This MUST be in the same order as the header function or else columns won't line up!
        if (!this->money_signals_to_log.empty()) {
            out += "," + this->signals_to_log_to_string<Money>(this->money_signals_to_log);
        }
        if (!this->double_signals_to_log.empty()) {
            out += "," + this->signals_to_log_to_string<double>(this->double_signals_to_log);
        }
        if (!this->int_signals_to_log.empty()) {
            out += "," + this->signals_to_log_to_string<long long int>(this->int_signals_to_log);
        }
        this->buffer.push_back(out);

        if (this->buffer.size() < this->buffer.capacity()) {
            return;
        }

        this->flush();
    }
}