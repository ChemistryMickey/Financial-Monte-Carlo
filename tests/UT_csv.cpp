#include "gtest/gtest.h"
#include "include/csv.hpp"

#include <filesystem>

TEST(Test_csv, file_exists) {
    write_csv("test.csv", { {"foo", {1, 2, 3}} });
    ASSERT_TRUE(std::filesystem::exists("test.csv"));
}

TEST(Test_csv, unequal_columns) {
    EXPECT_THROW(write_csv("test.csv", { {"foo", {1, 2, 3}}, {"bar", {1, 2}} })
        , std::runtime_error);
}

TEST(Test_csv, simple_read) {
    write_csv("test.csv", { {"foo", {1, 2, 3}} });
    ASSERT_TRUE(std::filesystem::exists("test.csv"));

    auto in_csv = read_csv("test.csv");

    ASSERT_EQ(in_csv.at(0).first, "foo");
    ASSERT_EQ(in_csv.at(0).second[0], 1);
}