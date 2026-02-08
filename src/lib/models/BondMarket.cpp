#include "BondMarket.hpp"
#include "json.hpp"
#include "magic_enum/magic_enum.hpp"
#include "utils.hpp"

namespace fmc {
    BondMarket::BondMarket(const nlohmann::json& config, const std::chrono::sys_days& start_date_) : cur_day{start_date_} {
        for (auto& [bill_type, subconfig] : config.items()) {
            this->security_interest_rates[magic_enum::enum_cast<SecurityType>(bill_type).value()] = subconfig["interest_rate"].get<double>();
        }
    }

    void BondMarket::environment(unsigned int days_passed) {
        cur_day += std::chrono::days{days_passed};

        /// TODO: Update interest rates based on volatility
    }


    void BondMarket::set_bonds_maturity(std::vector<Bond>& bond_vec) {
        for (auto& bond : bond_vec) {
            bond.should_mature = this->cur_day >= bond.maturation_date;
        }
    }

    Money BondMarket::calc_coupon_payments(std::vector<Bond>& bond_vec) {
        Money out{0.0};
        for (auto& bond : bond_vec) {
            if (cur_day >= bond.next_coupon_payment_date) {
                out += bond.face_value * bond.coupon_interest_rate;
                bond.next_coupon_payment_date = cur_day + bond.coupon_payment_interval_days;
            }
        }

        return out;
    }
}