#pragma once
#include <cinttypes>
#include <chrono>
#include <numeric>

#include "Money.hpp"

namespace fmc {
    enum class SecurityType : uint8_t {
        /// @brief Standard treasury bill. Matures after a certain period with no coupon payments.
        T_Bill,
        /// @brief Treasury bill with coupon payments specified as part of the bill.
        T_Note,
        /// @brief Treasury Inflation-Protected Securities. Principal adjusts with inflation as measured by the Consumer Price Index.
        TIPS,
        /// @brief Floating Rate Notes. Interest rate fluctuates based on a benchmark index (usually 3-month T-bill rate) plus a spread.
        // FRN // Not gonna do this one for now. Too complicated.
    };

    struct Bond {
        /// @brief The date this bond was purchased
        std::chrono::sys_days purchased_date;

        /// @brief The date upon which the security matures and you receive the maturation value.
        std::chrono::sys_days maturation_date;

        /// @brief The next time a coupon payment will be made
        std::chrono::sys_days next_coupon_payment_date = std::chrono::sys_days::max();

        /// @brief The interval between which coupon payments are made (only applies to security types with coupons)
        std::chrono::days coupon_payment_interval_days = std::chrono::days::max();

        /// @brief The proportion of the current market value of the security you get as a coupon payment (not a percent)
        double coupon_interest_rate = 0.0;

        /// @brief The price you pay for the bond
        Money face_value{0.0};

        /// @brief The price you receive upon the maturation of the bond
        Money maturation_value{0.0};

        /// @brief The interest rate (used to calculate the maturation value from the face value) (not a percent)
        double interest_rate = 0.0;

        /// @brief Whether or not this bond should mature this tick (which should grant its maturation value and remove it from your portfolio)
        bool should_mature = false;

        /// @brief The security type (e.g. T-Bill, T-Note (has coupon payments), TIPS, etc.)
        SecurityType security_type = SecurityType::T_Bill;

        Bond(
            std::chrono::sys_days purchsed_date,
            std::chrono::sys_days maturation_date,
            const Money& face_value,
            double interest_rate,
            std::chrono::days coupone_payment_interval_days = std::chrono::days::max(),
            double coupon_interest_rate = 0.0,
            SecurityType security_type = SecurityType::T_Bill
        );
    }; // James Bond
}