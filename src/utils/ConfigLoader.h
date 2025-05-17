#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <string>
#include <filesystem>
#include "../common/AppConfig.h" // Để trả về AppConfig object
#include "../common/OperationResult.h" // Để trả về lỗi nếu load config thất bại

class ConfigLoader {
private:
    std::filesystem::path _configFilePath;
    // (Private helper methods để parse file INI/JSON/YAML)
    OperationResult<AppConfig> parseIniFile() const; // Ví dụ
public:
    ConfigLoader(const std::filesystem::path& configFilePath);
    OperationResult<AppConfig> loadConfig() const;
};

#endif