#include "SimConfig.hpp"

namespace fmc {
    SimConfig::SimConfig(const nlohmann::json& config) :
        start_date{string2sys_days(config.at("start_date").get<std::string>())},
        end_date{string2sys_days(config.at("end_date").get<std::string>())},
        logging_channel_def_path{config.at("logging_channels").get<std::string>()},
        person_config(config.at("person")), // This is a "most vexing parse". This must be parentheses
        stock_market_config(config.at("stock_market")),
        bond_market_config(config.at("bond_market")),
        annual_inflation_config(config.at("annual_inflation")) {}

    void generate_dispersed_configs(const std::filesystem::path& out_directory, uint runs, const nlohmann::json& config) {
        std::unordered_map<std::string, RandomVariable> monte_vars{};
        auto append_monte_var = [&](const char* root_key) {
            for (auto& [key, var] : config.at(root_key).items()) {
                DEBUG("Attempting to disperse {} with value {}", key, var.dump(3));
                if (key.contains("event")) {
                    for (auto& [event_key, event_var] : var.items()) {
                        monte_vars.emplace(key + "_" + event_key, event_var);
                    }
                }
                else {
                    monte_vars.emplace(key, var);
                }
            }
            };

        append_monte_var("person");
        append_monte_var("stock_market");
        append_monte_var("annual_inflation");

        DEBUG("Created all Monte Carlo variables");

        auto output_monte_var = [&](const char* root_key, nlohmann::json& out_config) {
            for (auto& [key, var] : config.at(root_key).items()) {
                if (key.contains("event")) {
                    for (auto& [event_key, _] : var.items()) {
                        out_config[root_key][key][event_key] = monte_vars.at(key + "_" + event_key).next_value();
                    }
                }
                else {
                    out_config[root_key][key] = monte_vars.at(key).next_value();
                }
            }

            };

        for (uint run = 0; run < runs; ++run) {
            std::filesystem::path cur_out_dir = run_out_dir(out_directory, run);
            std::filesystem::create_directories(cur_out_dir);
            nlohmann::json out_config{
                {"start_date", config.at("start_date")},
                {"end_date", config.at("end_date")},
                {"logging_channels", config.at("logging_channels")}
            };

            output_monte_var("person", out_config);
            output_monte_var("stock_market", out_config);
            output_monte_var("annual_inflation", out_config);

            write_json(cur_out_dir / "monte_config.lock", out_config);
        }

        INFO("Created dispersions");
    }
}