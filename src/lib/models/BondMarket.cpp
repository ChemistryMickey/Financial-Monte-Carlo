#include "BondMarket.hpp"
#include "json.hpp"
#include "magic_enum/magic_enum.hpp"
#include "utils.hpp"

namespace fmc {
    BondMarket::BondMarket(const nlohmann::json& config, const std::chrono::sys_days& start_date_) :
        cur_day{start_date_},
        rng{config["rng_seed"].get<uint>()} {
        for (auto& [bill_type, subconfig] : config.items()) {
            if (!bill_type.contains("T_")) {
                // Then it's not a Treasury Bill type
                continue;
            }

            SecurityType security_type = magic_enum::enum_cast<SecurityType>(bill_type).value();
            this->security_interest_rates.at(security_type) = subconfig["interest_rate"].get<double>();
            this->security_volatility.at(security_type) = subconfig["volatility"].get<double>();
            this->security_face_value.at(security_type) = Money{subconfig["face_value"].get<double>()};
        }
    }

    void BondMarket::environment(unsigned int days_passed) {
        cur_day += std::chrono::days{days_passed};

        for (auto& [bill_type, interest_rate] : this->security_interest_rates) {
            interest_rate *= (1 + this->rng.normal(0, this->security_volatility[bill_type]));
            interest_rate = std::max(0.0, interest_rate); // Is this a good assumption? I think so
        }
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