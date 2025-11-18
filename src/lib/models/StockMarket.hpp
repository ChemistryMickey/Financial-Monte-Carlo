#pragma once
#include "json.hpp"
#include "Money.hpp"
#include "math.hpp"
#include "TimeseriesDataLogger.hpp"

#include <rttr/type>
#include <rttr/registration_friend>

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