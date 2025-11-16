#include "gtest/gtest.h"
#include <chrono>
#include <print>

#include "utils.hpp"

using namespace std::chrono;

TEST(Test_time, increment_sys_days) {
    sys_days d = 2020y / 1 / 1;
    for (int step = 0; step < 10000000; ++step) {
        d += days{1};
    }

    year_month_day final_date = d; // implicit conversion
    std::print("{}\n", final_date);
}

TEST(Test_time, parse_from_string) {
    std::string date = "2025-11-16";
    std::chrono::year_month_day ymd = string2ymd(date);
    std::print("{}\n", ymd);

    std::chrono::sys_days sd = string2sys_days(date);
    sd += std::chrono::days{20};
    std::print("{}\n", sd);
}