#include "gtest/gtest.h"
#include <filesystem>
#include <print>

#include "Logger.hpp"
#include "utils.hpp"

namespace fmc {
    Logger logger{"test_output_log.log"};
    TEST(Test_Logger, buffer_size) {
        std::filesystem::path full_out_path = get_envvar("PROJECT_HOME") / std::filesystem::path{"test_output_log.log"};
        logger.out_path = full_out_path;
        EXPECT_EQ(logger.buffer.size(), 0);
        INFO("Thingy");
        EXPECT_FALSE(std::filesystem::exists(full_out_path));

        EXPECT_EQ(logger.buffer.size(), 1);
        INFO("msg");
        INFO("msg");
        DEBUG("msg");
        WARN("msg");
        EXPECT_EQ(logger.buffer.size(), 0);
        EXPECT_TRUE(std::filesystem::exists(full_out_path));
        ERROR("msg");
        EXPECT_EQ(logger.buffer.size(), 1);
    }
}