/// NOTE: The "Bonds" in this filename is to disambiguate it from BondMarket when globbing

#include "gtest/gtest.h"
#include "Bond.hpp"
#include "utils.hpp"

namespace fmc {
    TEST(TestBond, InitTBill) {
        Bond bond{
            string2sys_days("2020-01-01"),
            string2sys_days("2020-07-01"),
            Money{100.0},
            0.01,
        };

        EXPECT_EQ(bond.maturation_value, Money{101.0});
        EXPECT_EQ(bond.next_coupon_payment_date, string2sys_days("2020-01-01") + std::chrono::days{(size_t) 1e12});
    }

    TEST(TestBond, InitTNote) {
        Bond bond{
            string2sys_days("2020-01-01"),
            string2sys_days("2020-07-01"),
            Money{100.0},
            0.005,
            std::chrono::days{180},
            0.0005,
            SecurityType::T_Note,
        };
        EXPECT_EQ(bond.maturation_value, Money{100.5});
        EXPECT_EQ(bond.next_coupon_payment_date, string2sys_days("2020-01-01") + std::chrono::days{180});

    }
}