#include "math.hpp"
#include "gtest/gtest.h"

namespace fmc {
    TEST(Test_math, truncated_normal_sanity_check) {
        // Just sample this 1000 times and make sure no values are outside the bounds
        std::mt19937 gen{42};
        for (size_t i = 0; i < 1000; ++i) {
            double val = truncated_normal_icdf(gen, 0, 1, -0.01, 0.01);
            ASSERT_GT(val, -0.01);
            ASSERT_LT(val, 0.01);
        }
    }
}