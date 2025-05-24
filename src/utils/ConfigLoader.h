#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <string>
#include <filesystem>
#include "../common/AppConfig.h" // Để trả về AppConfig object
#include "../common/ErrorType.h"
#include <expected>

class ConfigLoader {
private:
    std::filesystem::path _configFilePath;
    // (Private helper methods để parse file INI/JSON/YAML)
    std::expected<AppConfig, Error> parseIniFile() const; // Ví dụ
public:
    ConfigLoader(const std::filesystem::path& configFilePath);
    std::expected<AppConfig, Error> loadConfig() const;
};

// Helper function to parse AppConfig::DataSourceType from string
DataSourceType parseDataSourceType(const std::string& str);

// Helper function to parse Logger::Level from string
Logger::Level parseLogLevel(const std::string& str);

#endif