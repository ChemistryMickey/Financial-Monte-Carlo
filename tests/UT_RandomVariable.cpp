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
}