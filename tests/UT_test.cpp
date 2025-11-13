#include "gtest/gtest.h"

TEST(Gtest_test, test_empty)
{
}

TEST(Gtest_test, test_trivial)
{
    EXPECT_EQ(1, 1);
}

TEST(Gtest_test, test_neq)
{
    EXPECT_NE(1, 2);
}

TEST(Gtest_test, make_sure_ut_def_is_defined) {
#ifdef UNIT_TESTING
    std::cout << "UNIT_TESTING is defined\n";
#endif
}