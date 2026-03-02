#include "include/json.hpp"
#include "gtest/gtest.h"
#include "utils.hpp"

#include <string>
#include <fstream>

using json = nlohmann::json;

struct Config {
    int width;
    std::string name;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, width, name);
};

template<typename T>
struct TemplateConfig {
    T width;
    std::string name;

    TemplateConfig(T t, const std::string& n) : width{t}, name{n} {}
};

namespace nlohmann {
    template<typename T>
    struct adl_serializer<TemplateConfig<T>> {
        static TemplateConfig<T> from_json(const json& j) {
            return TemplateConfig<T>(
                j.at("width").get<T>(),
                j.at("name").get<std::string>()
            );
        }
    };
}

TEST(JSON_tests, deserialization) {
    // Neat. I didn't know you could do this
    Config cfg = json::parse("{\"width\": 42, \"name\": \"Foobar\"}");

    EXPECT_EQ(cfg.width, 42);
    EXPECT_EQ(cfg.name, "Foobar");
}

TEST(JSON_tests, TemplateDeserialization) {
    // Neat. I didn't know you could do this
    TemplateConfig<double> cfg = json::parse("{\"width\": 42, \"name\": \"Foobar\"}");

    EXPECT_DOUBLE_EQ(cfg.width, 42);
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

enum class FooEnum {
    a,
    b,
    c,
    d
};

TEST(JSON_tests, EnumSerialization) {
    // Serialize
    nlohmann::json j = FooEnum::a;
    // Deserialize
    FooEnum foo = j;

    EXPECT_EQ(foo, FooEnum::a);
}