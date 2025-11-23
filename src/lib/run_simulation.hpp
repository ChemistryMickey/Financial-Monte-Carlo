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

namespace fmc {
    inline void run_simulation(const std::filesystem::path& out_dir, const SimConfig& config) {
        DEBUG("Executing simulation with config:\n{}", config);

        // Build simulation objects
        StockMarket stock_market{config.stock_market_config};
        // BondMarket bond_market{config.bond_market_config};
        AnnualInflation inflation{config.annual_inflation_config};
        Person person{stock_market, inflation, config.person_config};

        // Initialize timeseries logger
        std::chrono::sys_days cur_date = config.start_date;
        TimeseriesDataLogger ts_logger{
            cur_date,
            config.logging_channel_def_path,
            out_dir / "timeseries_data.csv",
            // The following key/value pairs must match EXACTLY
            {
                {"person", person},
                {"stock_market", stock_market},
                {"inflation", inflation}
            }
        };

        SimulationController sim_controller{cur_date, config.end_date,
            {stock_market, inflation, person}, ts_logger};

        sim_controller.run();
    }
}