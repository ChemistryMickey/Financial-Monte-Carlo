#include <rttr/registration.h>
#include "BondMarket.hpp"
#include "json.hpp"
#include "magic_enum/magic_enum.hpp"
#include "utils.hpp"
#include "Logger.hpp"

RTTR_REGISTRATION{
    rttr::registration::class_<fmc::BondMarket>("BondMarket")
        .property_readonly("T_Bill_interest_rate", &fmc::BondMarket::T_Bill_interest_rate) (
            // This means the value will be returned as a reference wrapper such that I can keep it in a table for logging! Slick.
            rttr::policy::prop::as_reference_wrapper
        )
        .property_readonly("T_Note_interest_rate", &fmc::BondMarket::T_Note_interest_rate) (
            // This means the value will be returned as a reference wrapper such that I can keep it in a table for logging! Slick.
            rttr::policy::prop::as_reference_wrapper
        )
        .property_readonly("T_IPS_interest_rate", &fmc::BondMarket::T_IPS_interest_rate) (
            // This means the value will be returned as a reference wrapper such that I can keep it in a table for logging! Slick.
            rttr::policy::prop::as_reference_wrapper
        );
}

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
            this->security_volatility.at(security_type) = subconfig["volatility"].get<double>();
            this->security_face_value.at(security_type) = Money{subconfig["face_value"].get<double>()};
            *this->security_interest_rate_ptrs.at(security_type) = subconfig["interest_rate"].get<double>();
        }
    }

    void BondMarket::environment(unsigned int days_passed) {
        cur_day += std::chrono::days{days_passed};

        for (auto& [bill_type, volatility] : this->security_volatility) {
            double* interest_rate_ptr = this->security_interest_rate_ptrs.at(bill_type);
            *interest_rate_ptr *= (1 + this->rng.normal(0, volatility));
            *interest_rate_ptr = std::max(0.0, *interest_rate_ptr); // Is this a good assumption? I think so
        }
    }


    void BondMarket::set_bonds_maturity(std::vector<Bond>& bond_vec) const {
        for (auto& bond : bond_vec) {
            bond.should_mature = this->cur_day >= bond.maturation_date;
        }
    }

    Money BondMarket::calc_coupon_payments(std::vector<Bond>& bond_vec) const {
        Money out{0.0};
        for (auto& bond : bond_vec) {
            if (cur_day >= bond.next_coupon_payment_date) {
                out += bond.face_value * bond.coupon_interest_rate;
                bond.next_coupon_payment_date = cur_day + bond.coupon_payment_interval_days;
            }
        }

        return out;
    }

    Bond BondMarket::buy_bond(BondDuration_days duration, SecurityType security_type) const {
        static_cast<void>(security_type);
        /// TODO: Add the ability to buy different security types
        DEBUG("Purchasing a bond on {}", this->cur_day);
        return Bond{
            this->cur_day,
            duration,
            this->security_face_value.at(security_type),
            *this->security_interest_rate_ptrs.at(security_type),
        };
    }
}