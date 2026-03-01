#include "math.hpp"
#include "gtest/gtest.h"
#include "json.hpp"

namespace fmc {
    TEST(TestMath, truncated_normal_sanity_check) {
        // Just sample this 1000 times and make sure no values are outside the bounds
        std::mt19937 gen{42};
        for (size_t i = 0; i < 1000; ++i) {
            double val = truncated_normal_icdf(gen, 0, 1, -0.01, 0.01);
            ASSERT_GT(val, -0.01);
            ASSERT_LT(val, 0.01);
        }
    }

    TEST(TestMath, DeserializingClampedValue) {
        ClampedValue<double> cv = nlohmann::json::parse("{\"value\": 5, \"upper\": 42, \"lower\": 0}");
        EXPECT_EQ(cv.get_value(), 5);
    }

    TEST(TestMath, SerializingClampedValue) {
        ClampedValue<double> cv{5, {0, 42}};
        nlohmann::json j = cv;
        std::cout << j.dump(4) << "\n";

        EXPECT_DOUBLE_EQ(j["value"].get<double>(), 5.0);
    }
}