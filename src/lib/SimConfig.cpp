#include "SimConfig.hpp"

namespace fmc {
    SimConfig::SimConfig(const nlohmann::json& config) :
        start_date{string2sys_days(config.at("start_date").get<std::string>())},
        end_date{string2sys_days(config.at("end_date").get<std::string>())},
        person{config.at("person")} {}

    void generate_dispersed_configs(const fmc::CliArgs& args, const nlohmann::json& config) {
        std::unordered_map<std::string, RandomVariable> monte_vars{};
        for (auto& [key, var] : config.at("person").items()) {
            monte_vars.emplace(key, var);
        }
        for (auto& [key, var] : config.at("stock_market").items()) {
            monte_vars.emplace(key, var);
        }
        monte_vars.emplace("annual_inflation", config.at("annual_inflation"));

        for (uint run = 0; run < args.runs; ++run) {
            std::filesystem::path cur_out_dir = run_out_dir(args.out_directory, run);
            std::filesystem::create_directories(cur_out_dir);
            nlohmann::json out_config{
                {"start_date", config["start_date"]},
                {"end_date", config["end_date"]}
            };

            for (auto& [key, _] : config.at("person").items()) {
                out_config["person"][key] = monte_vars.at(key).next_value();
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