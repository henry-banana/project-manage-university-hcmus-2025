#ifndef CSVFEEREPO_H
#define CSVFEEREPO_H

#include "../interface/IFeeRepository.h"
#include "../../entities/FeeRecord.h"
#include "../../../utils/FileHandler.h"
#include <string>
#include <vector>
#include <mutex>
#include <optional>

class CsvFeeRepo : public IFeeRepository {
private:
    std::string _filePath;
    mutable std::mutex _fileMutex;
    const char DELIMITER = ',';

    // Column indices
    enum ColumnIndex {
        COL_STUDENT_ID = 0,
        COL_TOTAL_FEE,
        COL_PAID_FEE,
        NUM_COLUMNS
    };

    std::vector<FeeRecord> loadFromFile() const;
    bool saveToFile(const std::vector<FeeRecord>& records);
    FeeRecord parseFeeRecord(const std::vector<std::string>& csvRow) const;
    std::vector<std::string> formatFeeRecord(const FeeRecord& record) const;

public:
    explicit CsvFeeRepo(std::string csvFilePath);

    // IFeeRepository implementation
    std::optional<FeeRecord> findByStudentId(const std::string& studentId) override;
    std::vector<FeeRecord> findAll() const override;
    bool add(const FeeRecord& feeRecord) override;
    bool update(const FeeRecord& feeRecord) override;
    bool remove(const std::string& studentId) override;
};

#endif // CSVFEEREPO_H