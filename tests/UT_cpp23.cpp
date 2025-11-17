#include "gtest/gtest.h"
#include <string>
#include <print>
#include "Person.hpp"
#include "Money.hpp"
#include "SimConfig.hpp"

TEST(Test_cpp23, string_contains) {
    std::string f{"Foobar"};
    // Aw heck yeah
    ASSERT_TRUE(f.contains("Foo"));
}

TEST(Test_cpp23, print) {
    // Schweet. Still need newlines though. That's fine.
    std::print("This is a test {}\n", 23);
}

TEST(Test_cpp23, print_money) {
    std::print("{}\n", fmc::Money{42});
}

TEST(Test_cpp23, print_clamped_value) {
    // Sometimes it seems just implementing a to_string method is the way forward.
    std::print("{}\n", fmc::ClampedValue<double>{42, {0, 50}}.to_string());
}

TEST(Test_cpp23, print_person) {
    std::print("{}\n", fmc::Person{});
}

TEST(Test_cpp23, print_config) {
    std::print("{}\n", fmc::SimConfig{});
}

TEST(Test_cpp23, format_chrono_days) {
    using namespace std::chrono;
    std::println("{}", sys_days{2025y / 1 / 1});
    // There's even println? Now we're cookin' with gas
}