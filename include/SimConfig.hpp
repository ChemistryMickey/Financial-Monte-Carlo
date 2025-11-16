#pragma once
#include <string>
#include <filesystem>
#include <format>
#include <unordered_map>
#include <chrono>

#include "CliArgs.hpp"
#include "Person.hpp"
#include "RandomVariable.hpp"
#include "file_io.hpp"
#include "Logger.hpp"
#include "utils.hpp"


namespace fmc {
    struct SimConfig {
        std::chrono::sys_days start_date;
        std::chrono::sys_days end_date;
        Person person;
        // fmc::StockMarket<T> stock_market;
        // fmc::AnnualInflation<T> annual_inflation;

        SimConfig(const nlohmann::json& config);
    };

    void generate_dispersed_configs(
        const fmc::CliArgs& args,
        const nlohmann::json& config
    );
}

namespace std {
    template <>
    struct formatter<fmc::SimConfig> {
        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }

        template <typename FormatContext>
        auto format(const fmc::SimConfig& config, FormatContext& ctx) {
            return format_to(ctx.out(), "Start Date: {}\n\tEnd Date: {}\n\tPerson: {}",
                config.start_date,
                config.end_date,
                config.person
            );
        }
    };
} // namespace std