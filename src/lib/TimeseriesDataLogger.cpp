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
        std::vector<std::string> mapped_names{};
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

            this->signals_to_log.emplace(signal, obj_type.get_property(split_signal[1]).get_value(obj).get_value<std::reference_wrapper<Money>>());
            mapped_names.push_back(mapped_name);
        }

        // Write the header of the CSV so you don't need to do it again.
        // But write the name-mapped columns. It doesn't matter in the logging, the order is preserved.
        std::string csv_header = std::format("Date,{}",
            std::ranges::to<std::string>(
                mapped_names
                | std::views::transform([](auto& p) {
                    return std::format("{}", p);
                    })
                | std::views::join_with(','))
        );

        std::ofstream f{logging_out_path, std::fstream::out}; // Completely overwrites file
        std::println(f, "{}", csv_header);

        DEBUG("Initialized TimeseriesDataLogger outputting at {}", logging_out_path);
    }

    void TimeseriesDataLogger::log() {
        std::string out = std::format("{},{}",
            this->clock,
            std::ranges::to<std::string>(
                this->signals_to_log
                | std::views::transform([](auto& p) {
                    return std::format("{}.{:02}", p.second.get().dollars, p.second.get().cents);
                    })
                | std::views::join_with(','))
        );

        this->buffer.push_back(out);

        if (this->buffer.size() < this->buffer.capacity()) {
            return;
        }

        this->flush();
    }

    void TimeseriesDataLogger::flush() {
        std::string batched_out_str = std::ranges::to<std::string>(this->buffer | std::views::join_with('\n'));

        std::ofstream f{this->log_out_path, std::fstream::app}; // Just append
        std::println(f, "{}", batched_out_str);

        this->buffer.clear();
    }
}