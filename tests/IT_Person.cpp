#include <string>

#include "gtest/gtest.h"
#include "models/Person.hpp"
#include "Logger.hpp"

namespace fmc {
    struct Test_Person : public ::testing::Test {
        nlohmann::json inflation_config = {
            {"deflation_event", {
                {"cooldown", 30},
                {"duration", 0},
                {"event_val", -0.1},
                {"knockdown", -0.05},
                {"probability", 0},
                {"rng_seed", 42},
                {"scaling_factor", 1.00001}
            }},
            {"inflation_event", {
                {"cooldown", 30},
                {"duration", 0},
                {"event_val", -0.1},
                {"knockdown", -0.05},
                {"probability", 0},
                {"rng_seed", 42},
                {"scaling_factor", 1.00001}
            }},
            {"rate", 0.02}
        };
        nlohmann::json stock_market_config = {
            {"annual_time_scaling_factor", 1.0},
            {"position_price", 100.00},
            {"volatility", 0.1},
            {"rng_seed", 42},
            {"boom_scaling_event", {
                {"event_val", 1.0},
                {"probability" , 0.0},
                {"rng_seed" , 42},
                {"scaling_factor" , 1.1},
                {"duration" , 30},
                {"cooldown" , 1800},
                {"knockdown" , -0.05}
            }},
            {"bust_scaling_event", {
                {"event_val", -1.0},
                {"probability" , 0.0},
                {"rng_seed" , 42},
                {"scaling_factor" , 1.1},
                {"duration" , 30},
                {"cooldown" , 1800},
                {"knockdown" , -0.05}
            }}
        };
        nlohmann::json person_config = {
            {"current_age", 30},
            {"death_age", 90},
            {"desired_cash_on_hand", 50000},
            {"income_until_retirement", 120000},
            {"initial_yearly_expenses", 24000},
            {"medical_event", {
                {"cooldown", 30},
                {"duration", 0},
                {"event_val", 500},
                {"knockdown", -0.05},
                {"probability", 0},
                {"rng_seed", 42},
                {"scaling_factor", 1.00001}
            }},
            {"retirement_age", 75},
            {"starting_money", 250000},
            {"stock_bond_ratio", 1},
            {"yearly_raise_percent", 0.0}
        };

        AnnualInflation inflation;
        StockMarket stock_market;

        Person person;
        Test_Person() : inflation{inflation_config}, stock_market{stock_market_config}, person{stock_market, inflation, person_config} {
            Logger::instance().initialize(std::string(__FILE__) + ".log", 100, false);
        }
    };

    TEST_F(Test_Person, init) {
        person.initialize();
    }
}