#include "gtest/gtest.h"
#include <filesystem>
#include <print>

#include "Logger.hpp"
#include "utils.hpp"

namespace fmc {
    TEST(Test_Logger, buffer_size) {
        Logger::instance().initialize(".", 5);
        std::filesystem::path out_path = std::filesystem::path{"output.log"};
        EXPECT_EQ(Logger::instance().buffer.size(), 1);
        INFO("Thingy");
        EXPECT_FALSE(std::filesystem::exists(out_path));

        EXPECT_EQ(Logger::instance().buffer.size(), 2);
        INFO("msg");
        INFO("msg");
        DEBUG("msg");
        EXPECT_EQ(Logger::instance().buffer.size(), 0);
        EXPECT_TRUE(std::filesystem::exists(out_path));
        WARN("msg");
        EXPECT_EQ(Logger::instance().buffer.size(), 1);
    }

    TEST(Test_Logger, error) {
        Logger::instance().initialize(".", 5);
        EXPECT_THROW(ERROR("Testing"), std::runtime_error);

        EXPECT_TRUE(std::filesystem::exists("output.log"));
    }
}