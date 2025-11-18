#pragma once
#include <filesystem>
#include <chrono>

#include "Logger.hpp"
#include "SimConfig.hpp"
#include "Person.hpp"
#include "TimeseriesDataLogger.hpp"

namespace fmc {
    inline void run_simulation(const std::filesystem::path& out_dir, const SimConfig& config) {
        DEBUG("Executing simulation with config:\n{}", config);

        // Copy out mutable simulation objects
        //fmc::StockMarket stock_market = config.stock_market
        //fmc::BondMarket bond_market = config.bond_market
        Person person{config.person_config};

        // Initialize objects
        //stock_market.initialize();
        //bond_market.initialize();
        person.initialize();

        // Initialize timeseries logger
        std::chrono::sys_days cur_date = config.start_date;
        TimeseriesDataLogger ts_logger{
            cur_date,
            config.logging_channel_def_path,
            out_dir / "timeseries_data.csv",
            // The following key/value pairs must match EXACTLY
            {{"person", person}}
        };


        // Simulation loop
        uint dt{1}; // [days]
        while (cur_date < config.end_date) {
            DEBUG("Starting day {}", cur_date);

            // Environment
            // stock_market.update()
            // bond_market.update()
            // inflation.update()

            // Models
            person.update(dt);

            // Log state
            ts_logger.log();

            // Increment date
            cur_date += std::chrono::days{dt};
        }
    }
}