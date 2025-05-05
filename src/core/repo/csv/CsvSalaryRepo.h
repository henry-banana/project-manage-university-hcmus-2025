#ifndef CSVSALARYREPO_H
#define CSVSALARYREPO_H

#include "../interface/ISalaryRepository.h"
#include "../../entities/SalaryRecord.h"
#include "../../../utils/FileHandler.h"
#include <string>
#include <vector>
#include <mutex>
#include <optional>

class CsvSalaryRepo : public ISalaryRepository {
private:
    std::string _filePath;
    mutable std::mutex _fileMutex;
    const char DELIMITER = ',';

    // Column indices
    enum ColumnIndex {
        COL_TEACHER_ID = 0,
        COL_BASIC_MONTHLY_PAY,
        // Add columns for bonus rate, etc. if needed
        NUM_COLUMNS
    };

    std::vector<SalaryRecord> loadFromFile() const;
    bool saveToFile(const std::vector<SalaryRecord>& records);
    SalaryRecord parseSalaryRecord(const std::vector<std::string>& csvRow) const;
    std::vector<std::string> formatSalaryRecord(const SalaryRecord& record) const;

public:
    explicit CsvSalaryRepo(std::string csvFilePath);

    // ISalaryRepository implementation
    std::optional<SalaryRecord> findByTeacherId(const std::string& teacherId) const override;
    std::vector<SalaryRecord> findAll() const override;
    bool add(const SalaryRecord& salaryRecord) override;
    bool update(const SalaryRecord& salaryRecord) override;
    bool remove(const std::string& teacherId) override;
};

#endif // CSVSALARYREPO_H