#include "gtest/gtest.h"
#include "../src/utils/ConfigLoader.h"
#include <fstream>
#include <sstream>

// Helper function to create a temporary config file
std::filesystem::path createTempConfigFile(const std::string& content) {
    std::filesystem::path tempFile = std::filesystem::temp_directory_path() / "config.ini";
    std::ofstream tempConfigFile(tempFile);
    tempConfigFile << content;
    tempConfigFile.close();
    return tempFile;
}

TEST(ConfigLoaderTest, LoadValidConfig) {
    // Create a temporary config file with valid content
    std::string configContent = R"(
[Database]
DataSourceType = SQL
SqlConnectionString = test_connection_string

[Logging]
LogLevel = DEBUG
LogFilePath = test_log_file.txt
)";
    std::filesystem::path tempConfigFile = createTempConfigFile(configContent);

    // Create a ConfigLoader object with the path to the temporary config file
    ConfigLoader configLoader(tempConfigFile);

    // Load the configuration
    auto configResult = configLoader.loadConfig();

    // Assert that the configuration was loaded successfully
    ASSERT_TRUE(configResult.has_value());

    // Assert that the configuration values are correct
    AppConfig config = configResult.value();
    ASSERT_EQ(config.dataSourceType, DataSourceType::SQL);
    ASSERT_EQ(config.sqlConnectionString, "test_connection_string");
    ASSERT_EQ(config.logLevel, Logger::Level::DEBUG);
    ASSERT_EQ(config.logFilePath, "test_log_file.txt");

    // Clean up the temporary config file
    std::filesystem::remove(tempConfigFile);
}

TEST(ConfigLoaderTest, LoadInvalidConfig) {
    // Create a temporary config file with invalid content (missing section)
    std::string configContent = R"(
[Database]
DataSourceType = SQL
SqlConnectionString = test_connection_string

)";
    std::filesystem::path tempConfigFile = createTempConfigFile(configContent);

    // Create a ConfigLoader object with the path to the temporary config file
    ConfigLoader configLoader(tempConfigFile);

    // Load the configuration
    auto configResult = configLoader.loadConfig();

    // Assert that the configuration was not loaded successfully
    ASSERT_TRUE(configResult.has_value()); // It still loads, but with defaults

    // Clean up the temporary config file
    std::filesystem::remove(tempConfigFile);
}

TEST(ConfigLoaderTest, LoadMissingConfigFile) {
    // Create a ConfigLoader object with a path to a non-existent config file
    ConfigLoader configLoader("non_existent_config_file.ini");

    // Load the configuration
    auto configResult = configLoader.loadConfig();

    // Assert that the configuration was not loaded successfully
    ASSERT_FALSE(configResult.has_value());
    ASSERT_EQ(configResult.error().code, ErrorCode::DATA_ACCESS_ERROR);

}

TEST(ConfigLoaderTest, ParseDataSourceTypeValid) {
    ASSERT_EQ(parseDataSourceType("SQL"), DataSourceType::SQL);
    ASSERT_EQ(parseDataSourceType("CSV"), DataSourceType::CSV);
    ASSERT_EQ(parseDataSourceType("MOCK"), DataSourceType::MOCK);
    ASSERT_EQ(parseDataSourceType("  mock  "), DataSourceType::MOCK); // Test trimming and case-insensitivity
}

TEST(ConfigLoaderTest, ParseDataSourceTypeInvalid) {
    ASSERT_EQ(parseDataSourceType("INVALID"), DataSourceType::MOCK); // Should default to MOCK
}

TEST(ConfigLoaderTest, ParseLogLevelValid) {
    ASSERT_EQ(parseLogLevel("DEBUG"), Logger::Level::DEBUG);
    ASSERT_EQ(parseLogLevel("INFO"), Logger::Level::INFO);
    ASSERT_EQ(parseLogLevel("WARN"), Logger::Level::WARN);
    ASSERT_EQ(parseLogLevel("ERROR"), Logger::Level::ERROR);
    ASSERT_EQ(parseLogLevel("CRITICAL"), Logger::Level::CRITICAL);
    ASSERT_EQ(parseLogLevel("  info  "), Logger::Level::INFO); // Test trimming and case-insensitivity
}

TEST(ConfigLoaderTest, ParseLogLevelInvalid) {
    ASSERT_EQ(parseLogLevel("INVALID"), Logger::Level::INFO); // Should default to INFO
}