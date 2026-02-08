#pragma once
#include "include/json.hpp" 
#include "Money.hpp"
#include "Event.hpp"
#include "Model.hpp"
#include "math.hpp"
#include "TimeseriesDataLogger.hpp"

#include <rttr/type>
#include <rttr/registration_friend>

/// TODO:
///     - Add events that shock the stock market. Ideally every 20 years or so.

namespace fmc {
    struct StockMarket : public TimeseriesLoggable, public Model {
        Money position_price;
        Event boom_scaling_event;
        Event bust_scaling_event;
        double volatility;
        double annual_time_scaling_factor;
        RNG rng;

        StockMarket(const nlohmann::json& config);
        void update(uint days_passed) override {
            static_cast<void>(days_passed);
        }
        void initialize() override;
        void yearly_update() override {}
        void environment(uint t) override;
        bool should_terminate() override { return false; }

        RTTR_ENABLE(TimeseriesLoggable);
        RTTR_REGISTRATION_FRIEND;
    };
}