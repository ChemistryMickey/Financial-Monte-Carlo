#include <format>

#include "json.hpp"
#include "Logger.hpp"
#include "utils.hpp"
#include "file_io.hpp"
#include "CliArgs.hpp"
#include "RandomVariable.hpp"
#include "Person.hpp"
#include "SimConfig.hpp"

void run_simulation(const std::filesystem::path& out_dir, const fmc::SimConfig& config);

int main(int argc, char** argv) {
    fmc::CliArgs args{argc, argv};
    fmc::Logger::instance().initialize(args);

    DEBUG("\n{}", args);

    auto config_json = read_json(args.config_path);

    std::filesystem::remove_all(args.out_directory);
    if (args.runs == 1) {
        // Then this is a single run and the JSON should be for a single shot without dispersions
        std::filesystem::create_directories(args.out_directory);

        fmc::SimConfig config{config_json};
        run_simulation(args.out_directory, config);
    }
    else {
        fmc::generate_dispersed_configs(args, config_json);
        if (args.dry) {
            return 0;
        }

        // This should be parallelized at some point but this is pretty darn fast.
        for (uint run = 0; run < args.runs; ++run) {
            INFO("Beginning run {}", run);

            std::filesystem::path run_dir = run_out_dir(args.out_directory, run);
            fmc::SimConfig config{read_json(run_dir / "monte_config.lock")};
            run_simulation(run_dir, config);
        }
    }
}

void run_simulation(const std::filesystem::path& out_dir, const fmc::SimConfig& config) {
    DEBUG("Executing simulation with config:\n{}", config);

    // Copy out mutable simulation objects
    //fmc::StockMarket stock_market = config.stock_market
    //fmc::BondMarket bond_market = config.bond_market
    fmc::Person person = config.person;

    // Initialize objects
    //stock_market.initialize();
    //bond_market.initialize();
    // person.initialize();

    // Simulation loop
    uint dt = 1; // [days]
    for (
        std::chrono::sys_days cur_date = config.start_date;
        cur_date < config.end_date;
        cur_date += std::chrono::days{dt}
        ) {
        DEBUG("Starting day {}", cur_date);

        // Environment
        // stock_market.update()
        // bond_market.update()
        // inflation.update()
        person.update(dt);


        // Log state
    }
}