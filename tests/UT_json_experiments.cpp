#include "include/json.hpp"
#include "gtest/gtest.h"

#include <string>
#include <fstream>

using json = nlohmann::json;

struct Config {
    int width;
    std::string name;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, width, name);
};

TEST(JSON_tests, deserialization) {
    // Neat. I didn't know you could do this
    Config cfg = json::parse("{\"width\": 42, \"name\": \"Foobar\"}");

    EXPECT_EQ(cfg.width, 42);
    EXPECT_EQ(cfg.name, "Foobar");
}

TEST(JSON_tests, read_json_file) {
    std::string raw_json = "{\"width\": 42, \"name\": \"Foobar\"}";
    {
        std::ofstream f{"test_read_json_file.json"};
        f.write(raw_json.c_str(), raw_json.size());
        f.flush();
    }

    json j;
    {
        std::ifstream f{"test_read_json_file.json"};
        f >> j;
    }
    EXPECT_EQ(j["width"], 42);
    EXPECT_EQ(j["name"], "Foobar");
}