#include "BondMarket.hpp"
#include "json.hpp"
#include "magic_enum/magic_enum.hpp"

namespace fmc {
    BondMarket::BondMarket(const nlohmann::json& config) {
        for (auto& [bill_type, subconfig] : config.items()) {
            this->security_interest_rates[magic_enum::enum_cast<SecurityType>(bill_type).value()] = subconfig["interest_rate"].get<double>();
        }
    }

    void BondMarket::environment(uint days_passed) {

    }
}