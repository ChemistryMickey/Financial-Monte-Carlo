#include "gtest/gtest.h"
#include <filesystem>
#include <print>

#include "Logger.hpp"
#include "utils.hpp"

namespace fmc {
    TEST(Test_Logger, buffer_size) {
        Logger::instance().initialize("test_output_log.log", 5);
        std::filesystem::path out_path = std::filesystem::path{"test_output_log.log"};
        EXPECT_EQ(Logger::instance().buffer.size(), 0);
        INFO("Thingy");
        EXPECT_FALSE(std::filesystem::exists(out_path));

        EXPECT_EQ(Logger::instance().buffer.size(), 1);
        INFO("msg");
        INFO("msg");
        DEBUG("msg");
        WARN("msg");
        EXPECT_EQ(Logger::instance().buffer.size(), 0);
        EXPECT_TRUE(std::filesystem::exists(out_path));
        ERROR("msg");
        EXPECT_EQ(Logger::instance().buffer.size(), 1);
    }
}