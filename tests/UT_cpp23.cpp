#include "gtest/gtest.h"
#include <string>
#include <print>

TEST(Test_cpp23, string_contains) {
    std::string f{"Foobar"};
    // Aw heck yeah
    ASSERT_TRUE(f.contains("Foo"));
}

TEST(Test_cpp23, print) {
    // Schweet. Still need newlines though. That's fine.
    std::print("This is a test {}\n", 23);
}