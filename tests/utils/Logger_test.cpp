#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "Logger.h"

TEST(LoggerTest, SingletonInstance) {
    Logger& logger1 = Logger::getInstance();
    Logger& logger2 = Logger::getInstance();
    EXPECT_EQ(&logger1, &logger2); // Cùng một instance
}

TEST(LoggerTest, ConfigureAndLogFileCreated) {
    std::filesystem::path logDir = "test_logs";
    Logger& logger = Logger::getInstance();
    logger.configure(Logger::Level::DEBUG, logDir, "testlog", ".log");
    logger.info("Test info message");
    logger.error("Test error message");

    // Kiểm tra có file log mới tạo không
    bool found = false;
    for (const auto& entry : std::filesystem::directory_iterator(logDir)) {
        if (entry.is_regular_file() && entry.path().filename().string().find("testlog_") == 0) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

TEST(LoggerTest, SetLogLevel) {
    Logger& logger = Logger::getInstance();
    logger.setLogLevel(Logger::Level::ERROR);
    EXPECT_EQ(logger.levelToString(Logger::Level::ERROR), "ERROR   ");
}

TEST(LoggerTest, ReleaseInstance) {
    Logger::releaseInstance();
    // Sau khi release, gọi lại getInstance vẫn hoạt động
    Logger& logger = Logger::getInstance();
    logger.info("Logger recreated after release.");
}