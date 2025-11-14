#include "gtest/gtest.h"
#include <string>

TEST(Test_cpp23, string_contains) {
    std::string f{"Foobar"};
    // Aw heck yeah
    ASSERT_TRUE(f.contains("Foo"));
}