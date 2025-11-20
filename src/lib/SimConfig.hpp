#pragma once
#include <string>
#include <filesystem>
#include <format>
#include <unordered_map>
#include <chrono>

#include "RandomVariable.hpp"
#include "file_io.hpp"
#include "Logger.hpp"
#include "utils.hpp"


namespace fmc {
    struct SimConfig {
        std::chrono::sys_days start_date;
        std::chrono::sys_days end_date;
        std::filesystem::path logging_channel_def_path;
        nlohmann::json person_config;
        nlohmann::json stock_market_config;
        // BondMarket bond_market;
        nlohmann::json annual_inflation_config;

        SimConfig() = default;
        SimConfig(const nlohmann::json& config);
    };

    void generate_dispersed_configs(const std::filesystem::path& out_directory, uint runs, const nlohmann::json& config);
}

namespace std {
    template <>
    struct formatter<fmc::SimConfig> {
        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }

        template <typename FormatContext>
        auto format(const fmc::SimConfig& config, FormatContext& ctx) const {
            return format_to(ctx.out(), "Sim Config:\n\tStart Date: {}\n\tEnd Date: {}\n\tLogging config path: {}\n\tPerson:\n{}\n\tAnnual Inflation: {}",
                config.start_date,
                config.end_date,
                config.logging_channel_def_path,
                config.person_config.dump(3),
                config.annual_inflation_config.dump(3)
            );
        }
    };
} // namespace std