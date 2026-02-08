#include "gtest/gtest.h"
#include "models/BondMarket.hpp"
#include "utils.hpp"

namespace fmc {
    class TestBondMarket : public ::testing::Test {
    public:
        nlohmann::json config = {
            {
                "T_Bill", {
                    {"volatitlity", 0},
                    {"face_value", 100},
                    {"interest_rate", 0.01}
                }
            },
            {
                "T_Note", {
                    {"volatitlity", 0},
                    {"face_value", 100},
                    {"interest_rate", 0.001},
                    {"coupon_award_rate", 1e-5},
                    {"coupon_proc_rate_months", 6}
                }
            },
            {
                "T_IPS", {
                    {"volatitlity", 0},
                    {"face_value", 100},
                    {"interest_rate", 0.001}
                }
            }
        };

        std::chrono::sys_days start_date = string2sys_days("2020-01-01");

        BondMarket market{config, start_date};
    };

    TEST_F(TestBondMarket, init) {
        EXPECT_EQ(market.cur_day, string2sys_days("2020-01-01"));
        EXPECT_EQ(market.security_interest_rates[SecurityType::T_IPS], 0.001);
    }

    TEST_F(TestBondMarket, SetBondMaturity) {
        std::vector<Bond> bonds{
            Bond {
                string2sys_days("2020-01-01"),
                string2sys_days("2020-07-01"),
                Money{100.0},
                0.01,
            },
            Bond {
                string2sys_days("2020-01-01"),
                string2sys_days("2020-08-01"),
                Money{100.0},
                0.01,
            },
            Bond {
                string2sys_days("2020-01-01"),
                string2sys_days("2020-09-01"),
                Money{100.0},
                0.01,
            },
            Bond {
                string2sys_days("2020-01-01"),
                string2sys_days("2020-10-01"),
                Money{100.0},
                0.01,
            },
        };

        market.cur_day = string2sys_days("2020-06-30");
        market.set_bonds_maturity(bonds);
        for (auto& bond : bonds) {
            EXPECT_FALSE(bond.should_mature);
        }

        market.cur_day += std::chrono::days{30};
        market.set_bonds_maturity(bonds);
        EXPECT_TRUE(bonds[0].should_mature);
        EXPECT_FALSE(bonds[1].should_mature);

        market.cur_day += std::chrono::days{30};
        market.set_bonds_maturity(bonds);
        EXPECT_TRUE(bonds[1].should_mature);
        EXPECT_FALSE(bonds[2].should_mature);

        market.cur_day += std::chrono::days{30};
        market.set_bonds_maturity(bonds);
        EXPECT_TRUE(bonds[2].should_mature);
        EXPECT_FALSE(bonds[3].should_mature);

        market.cur_day += std::chrono::days{30};
        market.set_bonds_maturity(bonds);
        EXPECT_TRUE(bonds.back().should_mature);
    }

    TEST_F(TestBondMarket, CalcCouponPayments) {
        std::vector<Bond> bonds{
            Bond {
                string2sys_days("2020-01-01"),
                string2sys_days("2021-01-01"),
                Money{100.0},
                0.01,
            },
            Bond {
                string2sys_days("2020-01-01"),
                string2sys_days("2021-01-01"),
                Money{100.0},
                0.001,
                std::chrono::days{30},
                0.0005,
                SecurityType::T_Note
            },
            Bond {
                string2sys_days("2020-01-01"),
                string2sys_days("2021-01-01"),
                Money{100.0},
                0.001,
                std::chrono::days{32},
                0.001,
                SecurityType::T_Note
            },
            Bond {
                string2sys_days("2020-01-01"),
                string2sys_days("2021-01-01"),
                Money{100.0},
                0.01,
            },
        };

        EXPECT_EQ(market.calc_coupon_payments(bonds), Money{0.0});

        market.cur_day += std::chrono::days{30};
        EXPECT_EQ(market.calc_coupon_payments(bonds), Money{0.0005 * 100.0});

        market.cur_day += std::chrono::days{2};
        EXPECT_EQ(market.calc_coupon_payments(bonds), Money{0.001 * 100.0});
    }
}