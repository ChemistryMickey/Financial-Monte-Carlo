#pragma once
#include <filesystem>
#include <chrono>

#include "Logger.hpp"
#include "SimConfig.hpp"
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

        // Initialize objects
        stock_market.initialize();
        //bond_market.initialize();
        inflation.initialize();
        person.initialize();

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


        // Simulation loop
        uint dt = 1; // [days]
        while (cur_date < config.end_date) {
            DEBUG("Starting day {}", cur_date);
            // January 1st updates
            if (is_jan_1st(cur_date)) {
                person.yearly_update();
            }

            // Environment
            stock_market.update(dt);
            // bond_market.update()
            inflation.update(dt);

            // Models
            person.update(dt);

            // Log state
            ts_logger.log();

            // Termination Conditions
            if (person.bankrupt()) {
                WARN("Went bankrupt on {} with a net worth of ${} and {} stocks ({} $/share)",
                    cur_date,
                    person.current_net_worth,
                    person.n_stocks,
                    stock_market.position_price
                );
                return;
            }

            if (person.dead()) {
                INFO("Died on {} with ${} net worth", cur_date, person.current_net_worth);
                return;
            }

            // Increment date
            cur_date += std::chrono::days{dt};
        }
    }
}