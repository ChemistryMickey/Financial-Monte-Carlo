#pragma once

#include "json.hpp"
#include "Bond.hpp"

namespace fmc {
    struct BondMarket {
        BondMarket(const nlohmann::json& config);
    };
}