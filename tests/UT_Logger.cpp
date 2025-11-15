#include "gtest/gtest.h"
#include <filesystem>
#include <print>

#include "Logger.hpp"
#include "utils.hpp"

namespace fmc {
    Logger logger{"test_output_log.log", 5};
    TEST(Test_Logger, buffer_size) {
        std::filesystem::path out_path = std::filesystem::path{"test_output_log.log"};
        logger.out_path = out_path;
        EXPECT_EQ(logger.buffer.size(), 0);
        INFO("Thingy");
        EXPECT_FALSE(std::filesystem::exists(out_path));

        EXPECT_EQ(logger.buffer.size(), 1);
        INFO("msg");
        INFO("msg");
        DEBUG("msg");
        WARN("msg");
        EXPECT_EQ(logger.buffer.size(), 0);
        EXPECT_TRUE(std::filesystem::exists(out_path));
        ERROR("msg");
        EXPECT_EQ(logger.buffer.size(), 1);
    }
}