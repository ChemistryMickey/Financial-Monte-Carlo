#include "Bond.hpp"
#include "magic_enum/magic_enum.hpp"

namespace fmc {
    Bond::Bond(
        std::chrono::sys_days purchsed_date_,
        BondDuration_days bond_duration_,
        const Money& face_value_,
        double interest_rate_,
        std::chrono::days coupon_payment_interval_days_,
        double coupon_interest_rate_,
        SecurityType security_type_
    ) : purchased_date{purchsed_date_},
        coupon_payment_interval_days{coupon_payment_interval_days_},
        coupon_interest_rate{coupon_interest_rate_},
        face_value{face_value_},
        interest_rate{interest_rate_},
        security_type{security_type_} {
        this->maturation_value = this->face_value * (1 + this->interest_rate);
        this->maturation_date = this->purchased_date + static_cast<std::chrono::days>(magic_enum::enum_integer(bond_duration_));
        this->next_coupon_payment_date = this->purchased_date + this->coupon_payment_interval_days;
    }
}