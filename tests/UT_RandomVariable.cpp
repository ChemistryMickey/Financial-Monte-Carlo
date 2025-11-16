#include "RandomVariable.hpp"
#include "gtest/gtest.h"

namespace fmc {
    TEST(Test_MonteCarloVariable, instantiation_of_each_type) {
        {
            RandomVariable mc_var{0, UniformDistributionArgs{.lower = 0, .upper = 1}, 42};
            ASSERT_EQ(mc_var.cur_val, 0);
            ASSERT_EQ(mc_var.distribution_type, DistributionType::Uniform);
            ASSERT_EQ(mc_var.initial_undispersed_value, 0);

            double val = mc_var.next_value();
            ASSERT_TRUE(val > 0 && val < 1);
        }
        {
            RandomVariable mc_var{0, GaussianDistributionArgs{.mean = 0, .stddev = 1}, 42};
            ASSERT_EQ(mc_var.cur_val, 0);
            ASSERT_EQ(mc_var.distribution_type, DistributionType::Gaussian);
            ASSERT_EQ(mc_var.initial_undispersed_value, 0);
            mc_var.next_value();
        }
        {
            RandomVariable mc_var{0, TruncatedGaussianDistributionArgs{.limits{.lower = 0, .upper = 1}, .normal{.mean = 0, .stddev = 1}}, 42};
            ASSERT_EQ(mc_var.cur_val, 0);
            ASSERT_EQ(mc_var.distribution_type, DistributionType::TruncatedGaussian);
            ASSERT_EQ(mc_var.initial_undispersed_value, 0);

            double val = mc_var.next_value();
            ASSERT_TRUE(val > 0 && val < 1);
        }
    }

    TEST(Test_MonteCarloVariable, reseedability) {
        RandomVariable mc_var{0, UniformDistributionArgs{.lower = 0, .upper = 1}, 42};
        ASSERT_EQ(mc_var.cur_val, 0);
        ASSERT_EQ(mc_var.distribution_type, DistributionType::Uniform);
        ASSERT_EQ(mc_var.initial_undispersed_value, 0);

        double first_rand_val = mc_var.next_value();

#ifdef UNIT_TESTING
        mc_var.rng.reseed(42);
#endif

        double second_rand_val = mc_var.next_value();

        ASSERT_EQ(first_rand_val, second_rand_val);

        double third_rand_val = mc_var.next_value();
        ASSERT_NE(first_rand_val, third_rand_val);
    }

    TEST(Test_MonteCarloVariable, from_config) {
        {
            nlohmann::json j{
                           {"initial_undispersed_value", 0.05},
                           {"dispersion_type", "uniform"},
                           {"dispersion", {
                               {"lower", 0},
                               {"upper", 0.1}
                           }},
                           {"seed", 42}
            };
            RandomVariable mc_var{j};
            EXPECT_EQ(mc_var.initial_undispersed_value, 0.05);
            EXPECT_EQ(mc_var.cur_val, 0.05);
            EXPECT_EQ(mc_var.distribution_type, DistributionType::Uniform);
            EXPECT_EQ(mc_var.rng_args.limits.lower, 0);
            EXPECT_EQ(mc_var.rng_args.limits.upper, 0.1);

            EXPECT_NEAR(mc_var.next_value(), 0.13, 0.01);
            mc_var.rng.reseed(42);
            EXPECT_NEAR(mc_var.next_value(), 0.13, 0.01);
        }
        {
            nlohmann::json j{
                           {"initial_undispersed_value", 0.05},
                           {"dispersion_type", "gaussian"},
                           {"dispersion", {
                               {"mean", 0},
                               {"standard_deviation", 0.1}
                           }},
                           {"seed", 42}
            };
            RandomVariable mc_var{j};
            EXPECT_EQ(mc_var.initial_undispersed_value, 0.05);
            EXPECT_EQ(mc_var.cur_val, 0.05);
            EXPECT_EQ(mc_var.distribution_type, DistributionType::Gaussian);
            EXPECT_EQ(mc_var.rng_args.normal.mean, 0);
            EXPECT_EQ(mc_var.rng_args.normal.stddev, 0.1);

            EXPECT_NEAR(mc_var.next_value(), -0.005, 0.0005);
            mc_var.rng.reseed(42);
            EXPECT_NEAR(mc_var.next_value(), -0.005, 0.0005);
        }
        {
            nlohmann::json j{
                           {"initial_undispersed_value", 0.05},
                           {"dispersion_type", "truncated_gaussian"},
                           {"dispersion", {
                               {"lower", 0},
                               {"upper", 0.1},
                               {"mean", 0},
                               {"standard_deviation", 0.2}
                           }},
                           {"seed", 42}
            };
            RandomVariable mc_var{j};
            EXPECT_EQ(mc_var.initial_undispersed_value, 0.05);
            EXPECT_EQ(mc_var.cur_val, 0.05);
            EXPECT_EQ(mc_var.distribution_type, DistributionType::TruncatedGaussian);
            EXPECT_EQ(mc_var.rng_args.limits.lower, 0);
            EXPECT_EQ(mc_var.rng_args.limits.upper, 0.1);
            EXPECT_EQ(mc_var.rng_args.normal.mean, 0);
            EXPECT_EQ(mc_var.rng_args.normal.stddev, 0.2);

            EXPECT_NEAR(mc_var.next_value(), 0.13, 0.02);
            mc_var.rng.reseed(42);
            EXPECT_NEAR(mc_var.next_value(), 0.13, 0.02);
        }
    }
}