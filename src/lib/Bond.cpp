#include "Bond.hpp"

namespace fmc {
    Bond::Bond(
        std::chrono::sys_days purchsed_date_,
        std::chrono::sys_days maturation_date_,
        const Money& face_value_,
        double interest_rate_,
        std::chrono::days coupon_payment_interval_days_,
        double coupon_interest_rate_,
        SecurityType security_type_
    ) : purchased_date{purchsed_date_},
        maturation_date{maturation_date_},
        face_value{face_value_},
        coupon_payment_interval_days{coupon_payment_interval_days_},
        interest_rate{interest_rate_},
        coupon_interest_rate{coupon_interest_rate_},
        security_type{security_type_} {
        this->maturation_value = this->face_value * (1 + this->interest_rate);
        this->next_coupon_payment_date = this->purchased_date + this->coupon_payment_interval_days;
    }
}