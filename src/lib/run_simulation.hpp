#pragma once
#include <filesystem>
#include <chrono>

#include "Logger.hpp"
#include "SimConfig.hpp"
#include "SimulationController.hpp"
#include "TimeseriesDataLogger.hpp"

#include "models/Person.hpp"
#include "models/StockMarket.hpp"
#include "models/BondMarket.hpp"
#include "models/AnnualInflation.hpp"

#define VARIABLE_NAME_PAIR(var) {#var, var}

namespace fmc {
    inline void run_simulation(const std::filesystem::path& out_dir, const SimConfig& config) {
        DEBUG("Executing simulation with config:\n{}", config);

        // Build simulation objects
        StockMarket stock_market{config.stock_market_config};
        DEBUG("Constructed Stock Market");
        BondMarket bond_market{config.bond_market_config, config.start_date};
        DEBUG("Constructed Bond Market");
        AnnualInflation inflation{config.annual_inflation_config};
        DEBUG("Constructed Annual Inflation");
        Person person{stock_market, bond_market, inflation, config.person_config};
        DEBUG("Constructed Person");

        // Initialize timeseries logger
        std::chrono::sys_days cur_date = config.start_date;
        TimeseriesDataLogger ts_logger{
            cur_date,
            config.logging_channel_def_path,
            out_dir / "timeseries_data.csv",
            {
                VARIABLE_NAME_PAIR(person),
                VARIABLE_NAME_PAIR(stock_market),
                VARIABLE_NAME_PAIR(bond_market),
                VARIABLE_NAME_PAIR(inflation),
            }
        };

        SimulationController sim_controller{cur_date, config.end_date,
            {stock_market, inflation, person, bond_market}, ts_logger};

        sim_controller.run();
    }
}