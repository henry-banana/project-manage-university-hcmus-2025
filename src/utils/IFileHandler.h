#ifndef IFILEHANDLER_H
#define IFILEHANDLER_H

#include <string>
#include <vector>
#include <filesystem>
#include "../../common/OperationResult.h"

class IFileHandler {
public:
    virtual ~IFileHandler() = default;

    // Đọc tất cả các dòng, mỗi dòng là một string
    virtual OperationResult<std::vector<std::string>> readAllLines(const std::filesystem::path& filePath) const = 0;
    // Ghi tất cả các dòng (ghi đè)
    virtual OperationResult<bool> writeAllLines(const std::filesystem::path& filePath, const std::vector<std::string>& lines) const = 0;
    // Ghi thêm một dòng
    virtual OperationResult<bool> appendLine(const std::filesystem::path& filePath, const std::string& line) const = 0;
    virtual bool fileExists(const std::filesystem::path& filePath) const = 0;
    virtual bool ensureDirectoryExists(const std::filesystem::path& dirPath) const = 0; // (➕)
};

#endif