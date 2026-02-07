#pragma once

#include "json.hpp"
#include "Bond.hpp"

namespace fmc {
    struct BondMarket {
        BondMarket(nlohmann::json config) {
            static_cast<void>(config); // NOT YET IMPLEMENTED
        }
    };
}