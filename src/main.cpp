#include <print>

#include "json.hpp"
#include "Logger.hpp"
#include "utils.hpp"
#include "file_io.hpp"
#include "Person.hpp"
#include "CliArgs.hpp"
#include "RandomVariable.hpp"

struct SimConfig {
    std::string start_date;
    std::string end_date;
    fmc::Person person;
    // fmc::StockMarket<T> stock_market;
    // fmc::AnnualInflation<T> annual_inflation;
    SimConfig(const nlohmann::json& config) : person{config["person"]} {}
};

void generate_dispersed_configs(const fmc::CliArgs& args, const nlohmann::json& config) {
    for (uint run = 0; run < args.runs; ++run) {
    }
}

int main(int argc, char** argv) {
    fmc::CliArgs args{argc, argv};

    static fmc::Logger logger{args};
    INFO("Logger successfully initialized");

    auto config_json = read_json(args.config_path);

    if (args.runs == 1) {
        // Then this is a single run and the JSON should be for a single shot without dispersions
        SimConfig config{config_json};
    }
    else {
        generate_dispersed_configs(args, config_json);
        if (args.dry) {
            return 0;
        }
    }


    // auto sim_config = read_json(project_path("data/sim_config.json"));
    // auto dispersed_sim_config =
    //     auto single_config_from_dispersion(sim_config["person"]);
    // fmc::Person person{sim_config["person"]};
    // INFO(std::format("Person starting money: {}", sim_config["person"]["starting_money"]["initial_undispersed_value"].get<double>()));
}