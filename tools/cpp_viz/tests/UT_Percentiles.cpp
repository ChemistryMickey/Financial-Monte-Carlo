#include "gtest/gtest.h"

#include "Percentiles.hpp"

TEST(TestPercentiles, InitTest) {
    // Just check that these tests are discoverable to google test
}

TEST(TestPercentiles, EmptyData) {
    fmc::PercentileStats p{};
    EXPECT_EQ(p.max.size(), 0);
    EXPECT_EQ(p.min.size(), 0);
    EXPECT_EQ(p.p5.size(), 0);
    EXPECT_EQ(p.p25.size(), 0);
    EXPECT_EQ(p.p50.size(), 0);
    EXPECT_EQ(p.p75.size(), 0);
    EXPECT_EQ(p.p95.size(), 0);
}

TEST(TestPercentiles, RepeatedData) {
    const std::vector<std::vector<double>> allRuns = {
        {0, 1, 2},
        {0, 1, 2},
        {0, 1, 2}
    };
    fmc::PercentileStats stats = fmc::computeStats(allRuns);


    EXPECT_EQ(stats.max[0], 0);
}