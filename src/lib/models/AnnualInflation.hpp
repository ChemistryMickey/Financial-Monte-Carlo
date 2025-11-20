#pragma once

#include "json.hpp"

namespace fmc {
    /// TODO: Make this more complex with fluctuating rates.
    ///     Inflation isn't constant and can be triggered by events.
    struct AnnualInflation {
        double annual_inflation_rate = 0.02;

        AnnualInflation() = default;
        AnnualInflation(double d) : annual_inflation_rate{d} {}
        // AnnualInflation(const nlohmann::json& config);    
    };
}