#include "BondMarket.hpp"
#include "json.hpp"

namespace fmc {
    BondMarket::BondMarket(const nlohmann::json& config) {
        static_cast<void>(config);
    }
}