#include "gtest/gtest.h"
#include "../src/utils/Logger.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <thread>

// Helper function to get the current timestamp in the format used by the logger
std::string getCurrentTimestampForFilename() {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = {};

#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&now_tm, &now_c);
#else
    localtime_r(&now_c, &now_tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y%m%d_%H%M%S");
    return oss.str();
}

class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Clean up any existing log files and reset the logger instance
        Logger::releaseInstance();
        if (std::filesystem::exists(logDirectory)) {
            std::filesystem::remove_all(logDirectory);
        }
        std::filesystem::create_directory(logDirectory);
    }

    void TearDown() override {
        // Clean up log files and reset the logger instance
        Logger::releaseInstance();
        if (std::filesystem::exists(logDirectory)) {
            std::filesystem::remove_all(logDirectory);
        }
    }

    // Configuration values for the logger
    std::filesystem::path logDirectory = std::filesystem::temp_directory_path() / "test_logs";
    std::string logFileBaseName = "test_app";
    std::string logFileExtension = ".log";
};

TEST_F(LoggerTest, SingletonInstance) {
    Logger& logger1 = Logger::getInstance();
    Logger& logger2 = Logger::getInstance();
    ASSERT_EQ(&logger1, &logger2);
}

TEST_F(LoggerTest, Configure) {
    Logger& logger = Logger::getInstance();
    logger.configure(Logger::Level::DEBUG, logDirectory, logFileBaseName, logFileExtension);

    // Check that the log directory was created
    ASSERT_TRUE(std::filesystem::exists(logDirectory));

    // Check that the log file was created with the correct name
    std::string expectedLogFileName = logFileBaseName + "_" + getCurrentTimestampForFilename() + logFileExtension;
    std::filesystem::path expectedLogFilePath = logDirectory / expectedLogFileName;
    ASSERT_TRUE(std::filesystem::exists(expectedLogFilePath));

    // Check that the log file contains the initial log message
    std::ifstream logFile(expectedLogFilePath.string());
    std::string line;
    ASSERT_TRUE(std::getline(logFile, line));
    ASSERT_NE(line.find("Logger configured. New log session started."), std::string::npos);
}

TEST_F(LoggerTest, SetLogLevel) {
    Logger& logger = Logger::getInstance();
    logger.configure(Logger::Level::INFO, logDirectory, logFileBaseName, logFileExtension);

    // Set the log level to DEBUG
    logger.setLogLevel(Logger::Level::DEBUG);

    // Check that the log level was changed
    std::string expectedLogFileName = logFileBaseName + "_" + getCurrentTimestampForFilename() + logFileExtension;
    std::filesystem::path expectedLogFilePath = logDirectory / expectedLogFileName;
    std::ifstream logFile(expectedLogFilePath.string());
    std::string line;
    while (std::getline(logFile, line)) {
        if (line.find("Log level changed from INFO to DEBUG") != std::string::npos) {
            return; // Test passed
        }
    }
    FAIL() << "Log level change message not found in log file.";
}

TEST_F(LoggerTest, Log) {
    Logger& logger = Logger::getInstance();
    logger.configure(Logger::Level::INFO, logDirectory, logFileBaseName, logFileExtension);

    // Log a message with a level of DEBUG (should not be written)
    logger.log(Logger::Level::DEBUG, "This is a debug message.");

    // Log a message with a level of INFO (should be written)
    logger.log(Logger::Level::INFO, "This is an info message.");

    // Check that the info message was written to the log file
    std::string expectedLogFileName = logFileBaseName + "_" + getCurrentTimestampForFilename() + logFileExtension;
    std::filesystem::path expectedLogFilePath = logDirectory / expectedLogFileName;
    std::ifstream logFile(expectedLogFilePath.string());
    std::string line;
    bool infoMessageFound = false;
    while (std::getline(logFile, line)) {
        if (line.find("This is an info message.") != std::string::npos) {
            infoMessageFound = true;
            break;
        }
    }
    ASSERT_TRUE(infoMessageFound);
}
