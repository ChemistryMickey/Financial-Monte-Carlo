#include "SimConfig.hpp"

namespace fmc {
    SimConfig::SimConfig(const nlohmann::json& config) :
        start_date{string2sys_days(config.at("start_date").get<std::string>())},
        end_date{string2sys_days(config.at("end_date").get<std::string>())},
        logging_channel_def_path{config.at("logging_channels").get<std::string>()},
        person_config(config.at("person")), // This is a "most vexing parse". This must be parentheses",
        stock_market_config(config.at("stock_market")),
        annual_inflation_config(config.at("annual_inflation").get<double>()) {}

    void generate_dispersed_configs(const std::filesystem::path& out_directory, uint runs, const nlohmann::json& config) {
        std::unordered_map<std::string, RandomVariable> monte_vars{};
        for (auto& [key, var] : config.at("person").items()) {
            DEBUG("Attempting to disperse {} with value {}", key, var.dump(3));
            if (key.contains("event")) {
                for (auto& [event_key, event_var] : var.items()) {
                    monte_vars.emplace(event_key, event_var);
                }
            }
            else {
                monte_vars.emplace(key, var);
            }
        }

        for (auto& [key, var] : config.at("stock_market").items()) {
            monte_vars.emplace(key, var);
        }
        monte_vars.emplace("annual_inflation", config.at("annual_inflation"));
        DEBUG("Created all Monte Carlo variables");

        for (uint run = 0; run < runs; ++run) {
            std::filesystem::path cur_out_dir = run_out_dir(out_directory, run);
            std::filesystem::create_directories(cur_out_dir);
            nlohmann::json out_config{
                {"start_date", config.at("start_date")},
                {"end_date", config.at("end_date")},
                {"logging_channels", config.at("logging_channels")}
            };

            for (auto& [key, var] : config.at("person").items()) {
                if (key.contains("event")) {
                    for (auto& [event_key, _] : var.items()) {
                        out_config["person"][key][event_key] = monte_vars.at(event_key).next_value();
                    }
                }
                else {
                    out_config["person"][key] = monte_vars.at(key).next_value();
                }
            }

            for (auto& [key, _] : config.at("stock_market").items()) {
                out_config["stock_market"][key] = monte_vars.at(key).next_value();
            }
            out_config["annual_inflation"] = monte_vars.at("annual_inflation").next_value();

            write_json(cur_out_dir / "monte_config.lock", out_config);
        }

        INFO("Created dispersions");
    }
}