#include <print>
#include "gtest/gtest.h"
#include "json.hpp"

#include "Money.hpp"
#include "models/StockMarket.hpp"
#include "Logger.hpp"

namespace fmc {
    struct Test_StockMarket : public ::testing::Test {
        nlohmann::json config = {
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

        StockMarket stock_market{config};

        Test_StockMarket() {
            Logger::instance().initialize(std::string(__FILE__) + ".log");
        }

        // void SetUp() override {} // Run before each test
        // void TearDown() override {} // Run after each test
        // static void SetUpTestSuite() {} // Shared between all tests, run at beginning of suite
        // static void TearDownTestSuite() {} // Shared between all tests, run at end of suite
    };

    TEST_F(Test_StockMarket, init) {
        EXPECT_EQ(stock_market.position_price, Money{100});
        EXPECT_EQ(stock_market.annual_time_scaling_factor, 1);
        EXPECT_EQ(stock_market.volatility, 0.1);
    }

    TEST_F(Test_StockMarket, update_without_volatility) {
        stock_market.volatility = 0;
        stock_market.update(1);

        // Doubles every year, so 100 / 365 per day
        double val = 100.0;
        Money expected = Money{val + (val / 365.0)};

        EXPECT_EQ(stock_market.position_price.dollars, expected.dollars);
        EXPECT_EQ(stock_market.position_price.cents, expected.cents);

        // value went up 27.5 cents

        // This updates based on the current value, not the initial value
        val = stock_market.position_price.to_double();
        expected = Money{val + (val / 365.0)};
        stock_market.update(1);

        EXPECT_EQ(stock_market.position_price.dollars, expected.dollars);
        EXPECT_EQ(stock_market.position_price.cents, expected.cents);
    }

    TEST_F(Test_StockMarket, update_without_scale) {
        // This one's empiracle because it involves the RNG but it feeds into the combined test.
        stock_market.annual_time_scaling_factor = 0;
        stock_market.update(1);

        Money expected = Money{94.50};

        EXPECT_EQ(stock_market.position_price.dollars, expected.dollars);
        EXPECT_EQ(stock_market.position_price.cents, expected.cents);

        // So the first tick of the RNG hits -5.50
    }

    TEST_F(Test_StockMarket, update) {
        stock_market.update(1);

        // The results from the two previous tests.
        Money expected = Money{100.0 - 5.50 + 0.27};
        EXPECT_EQ(stock_market.position_price.dollars, expected.dollars);
        EXPECT_EQ(stock_market.position_price.cents, expected.cents);
    }

    TEST_F(Test_StockMarket, boom_event) {
        stock_market.volatility = 0;
        stock_market.boom_scaling_event.probability.set_value(1);
        stock_market.bust_scaling_event.probability.set_value(0);

        // The Boom event will now happen for 30 days
        double effective_time_scaling_factor = 2.0 / 365.0;
        Money expected{100, 0};
        for (int i = 0; i < 30; ++i) {
            stock_market.update(1);
            Money scale_adjustment = expected * effective_time_scaling_factor;
            expected += scale_adjustment;

            EXPECT_EQ(stock_market.position_price.dollars, expected.dollars);
            EXPECT_EQ(stock_market.position_price.cents, expected.cents);
        }

        // Now it should be over.
        stock_market.update(1);
        expected *= (1.0 + effective_time_scaling_factor);
        EXPECT_EQ(stock_market.position_price.dollars, expected.dollars); // Dollars still match
        EXPECT_NE(stock_market.position_price.cents, expected.cents);
    }
}