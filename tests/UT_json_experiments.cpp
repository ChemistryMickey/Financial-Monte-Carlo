#include "include/json.hpp"
#include "gtest/gtest.h"

#include <string>

struct Config {
    int width;
    std::string name;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, width, name);
};

TEST(JSON_tests, deserialization) {
    Config cfg = nlohmann::json::parse("{\"width\": 42, \"name\": \"Foobar\"}");

    EXPECT_EQ(cfg.width, 42);
    EXPECT_EQ(cfg.name, "Foobar");
}