#include "gtest/gtest.h"
#include "../src/common/AppConfig.h"
#include <filesystem>

TEST(AppConfigTest, DefaultValues) {
    AppConfig config;

    ASSERT_EQ(config.dataSourceType, DataSourceType::MOCK);
    ASSERT_TRUE(config.csvFilePaths.empty());
    ASSERT_TRUE(config.sqlConnectionString.empty());
    ASSERT_EQ(config.logLevel, Logger::Level::INFO);
    ASSERT_EQ(config.logFilePath, "logs/app.log");
}

TEST(AppConfigTest, SetValues) {
    AppConfig config;

    config.dataSourceType = DataSourceType::SQL;
    config.csvFilePaths[EntityType::STUDENT] = "path/to/students.csv";
    config.sqlConnectionString = "test_connection_string";
    config.logLevel = Logger::Level::DEBUG;
    config.logFilePath = "test_logs/test.log";

    ASSERT_EQ(config.dataSourceType, DataSourceType::SQL);
    ASSERT_EQ(config.csvFilePaths[EntityType::STUDENT], "path/to/students.csv");
    ASSERT_EQ(config.sqlConnectionString, "test_connection_string");
    ASSERT_EQ(config.logLevel, Logger::Level::DEBUG);
    ASSERT_EQ(config.logFilePath, "test_logs/test.log");
}

TEST(AppConfigTest, DataSourceTypeCSV) {
    AppConfig config;
    config.dataSourceType = DataSourceType::CSV;
    ASSERT_EQ(config.dataSourceType, DataSourceType::CSV);
}

TEST(AppConfigTest, DataSourceTypeSQL) {
    AppConfig config;
    config.dataSourceType = DataSourceType::SQL;
    ASSERT_EQ(config.dataSourceType, DataSourceType::SQL);
}

TEST(AppConfigTest, CSVFilePath) {
    AppConfig config;
    config.csvFilePaths[EntityType::COURSE] = "path/to/courses.csv";
    ASSERT_EQ(config.csvFilePaths[EntityType::COURSE], "path/to/courses.csv");
}

TEST(AppConfigTest, MultipleCSVFilePaths) {
    AppConfig config;
    config.csvFilePaths[EntityType::STUDENT] = "path/to/students.csv";
    config.csvFilePaths[EntityType::COURSE] = "path/to/courses.csv";
    ASSERT_EQ(config.csvFilePaths[EntityType::STUDENT], "path/to/students.csv");
    ASSERT_EQ(config.csvFilePaths[EntityType::COURSE], "path/to/courses.csv");
}

TEST(AppConfigTest, EmptySQLConnectionString) {
    AppConfig config;
    config.sqlConnectionString = "";
    ASSERT_TRUE(config.sqlConnectionString.empty());
}

TEST(AppConfigTest, NonEmptySQLConnectionString) {
    AppConfig config;
    config.sqlConnectionString = "valid_connection_string";
    ASSERT_EQ(config.sqlConnectionString, "valid_connection_string");
}

TEST(AppConfigTest, LogLevelWarning) {
    AppConfig config;
    config.logLevel = Logger::Level::WARN;
    ASSERT_EQ(config.logLevel, Logger::Level::WARN);
}

TEST(AppConfigTest, LogLevelError) {
    AppConfig config;
    config.logLevel = Logger::Level::ERROR;
    ASSERT_EQ(config.logLevel, Logger::Level::ERROR);
}

TEST(AppConfigTest, LogFilePathRelative) {
    AppConfig config;
    config.logFilePath = "relative/path/to/log.txt";
    ASSERT_EQ(config.logFilePath, "relative/path/to/log.txt");
}

TEST(AppConfigTest, LogFilePathAbsolute) {
    AppConfig config;
    config.logFilePath = "/absolute/path/to/log.txt";
    ASSERT_EQ(config.logFilePath, "/absolute/path/to/log.txt");
}

TEST(AppConfigTest, CSVFilePathWithSpecialChars) {
    AppConfig config;
    config.csvFilePaths[EntityType::STUDENT] = "path/to/students with spaces.csv";
    ASSERT_EQ(config.csvFilePaths[EntityType::STUDENT], "path/to/students with spaces.csv");
}

TEST(AppConfigTest, SQLConnectionStringWithSpecialChars) {
    AppConfig config;
    config.sqlConnectionString = "connection;string=with;special;chars";
    ASSERT_EQ(config.sqlConnectionString, "connection;string=with;special;chars");
}

TEST(AppConfigTest, LogFilePathWithUnicode) {
    AppConfig config;
    config.logFilePath = "path/to/файл.log";
    ASSERT_EQ(config.logFilePath, "path/to/файл.log");
}

TEST(AppConfigTest, CSVFilePathWithUnicode) {
    AppConfig config;
    config.csvFilePaths[EntityType::STUDENT] = "path/to/студенти.csv";
    ASSERT_EQ(config.csvFilePaths[EntityType::STUDENT], "path/to/студенти.csv");
}

TEST(AppConfigTest, LongSQLConnectionString) {
    AppConfig config;
    std::string longConnectionString(200, 'a');
    config.sqlConnectionString = longConnectionString;
    ASSERT_EQ(config.sqlConnectionString, longConnectionString);
}

TEST(AppConfigTest, LongLogFilePath) {
    AppConfig config;
    std::string longLogFilePath(200, 'b');
    config.logFilePath = longLogFilePath;
    ASSERT_EQ(config.logFilePath, longLogFilePath);
}

TEST(AppConfigTest, CopyConstructor) {
    AppConfig config1;
    config1.dataSourceType = DataSourceType::SQL;
    config1.csvFilePaths[EntityType::STUDENT] = "path/to/students.csv";
    config1.sqlConnectionString = "test_connection_string";
    config1.logLevel = Logger::Level::DEBUG;
    config1.logFilePath = "test_logs/test.log";

    AppConfig config2 = config1;

    ASSERT_EQ(config2.dataSourceType, DataSourceType::SQL);
    ASSERT_EQ(config2.csvFilePaths[EntityType::STUDENT], "path/to/students.csv");
    ASSERT_EQ(config2.sqlConnectionString, "test_connection_string");
    ASSERT_EQ(config2.logLevel, Logger::Level::DEBUG);
    ASSERT_EQ(config2.logFilePath, "test_logs/test.log");
}