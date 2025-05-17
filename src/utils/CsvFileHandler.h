#ifndef CSVFILEHANDLER_H
#define CSVFILEHANDLER_H

#include "IFileHandler.h"
#include <vector>
#include <string>

class CsvFileHandler : public IFileHandler {
public:
    CsvFileHandler(char delimiter = ',');
    ~CsvFileHandler() override = default;

    OperationResult<std::vector<std::string>> readAllLines(const std::filesystem::path& filePath) const override;
    OperationResult<bool> writeAllLines(const std::filesystem::path& filePath, const std::vector<std::string>& lines) const override;
    OperationResult<bool> appendLine(const std::filesystem::path& filePath, const std::string& line) const override;
    bool fileExists(const std::filesystem::path& filePath) const override;
    bool ensureDirectoryExists(const std::filesystem::path& dirPath) const override;

    // CSV specific helpers (có thể là static hoặc public member)
    static std::vector<std::string> parseCsvLine(const std::string& line, char delimiter);
    static std::string formatCsvLine(const std::vector<std::string>& fields, char delimiter);
private:
    char _delimiter;
};

#endif