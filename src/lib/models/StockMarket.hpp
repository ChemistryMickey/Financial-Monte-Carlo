#pragma once
#include "include/json.hpp" // dunno why this one needs the include/ in order for nlohmann to get properly found. :shrug:
#include "Money.hpp"
#include "math.hpp"
#include "TimeseriesDataLogger.hpp"

#include <rttr/type>
#include <rttr/registration_friend>

/// TODO:
///     - Add events that shock the stock market. Ideally every 20 years or so.

namespace fmc {
    struct StockMarket : public TimeseriesLoggable {
        Money position_price;
        double volatility;
        double annual_time_scaling_factor;
        RNG rng;

        StockMarket(const nlohmann::json& config);
        void update(uint days_passed);
        void initialize();

        RTTR_ENABLE(TimeseriesLoggable);
        RTTR_REGISTRATION_FRIEND;
    };
}