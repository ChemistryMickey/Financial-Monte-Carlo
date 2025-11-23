#pragma once

#include "Event.hpp"
#include "json.hpp"
#include "Model.hpp"
#include "TimeseriesDataLogger.hpp"

#include <rttr/type>

namespace fmc {
    /// TODO: Make this more complex with fluctuating rates.
    ///     Inflation isn't constant and can be triggered by events.
    struct AnnualInflation : public Model, public TimeseriesLoggable {
        double annual_inflation_rate = 0.02;
        double initial_inflation_rate = 0.02;
        Event inflation_event;
        Event deflation_event;

        AnnualInflation(const nlohmann::json& config);

        void initialize() override {}
        void update(uint) override {};
        void environment(uint) override;
        void yearly_update() override {}
        bool should_terminate() override { return false; }

        RTTR_ENABLE(TimeseriesLoggable);
    };
}