#include "CsvFeeRepo.h"
#include "../../utils/Logger.h"
#include <stdexcept>
#include <algorithm>
#include <charconv> // For std::from_chars

CsvFeeRepo::CsvFeeRepo(std::string csvFilePath) : _filePath(std::move(csvFilePath)) {
    // Optional: Initialize file/header
}

// --- Helper Functions ---
std::vector<FeeRecord> CsvFeeRepo::loadFromFile() const {
    std::vector<FeeRecord> records;
    std::vector<std::vector<std::string>> csvData;
    try {
        std::lock_guard<std::mutex> lock(_fileMutex);
        csvData = CsvFileHandler::readAllLines(_filePath, DELIMITER);
    } catch (const std::runtime_error& e) {
        LOG_ERROR("Error reading fee data: " + std::string(e.what()));
        return records;
    }

    bool isFirstRow = true;
    for (const auto& row : csvData) {
        if (isFirstRow) {
            isFirstRow = false; continue; // Skip header
        }
        try {
            records.push_back(parseFeeRecord(row));
        } catch (const std::exception& e) {
             LOG_ERROR("Skipping invalid fee row: " + CsvFileHandler::join(row, DELIMITER) + " Error: " + e.what());
        }
    }
    return records;
}

bool CsvFeeRepo::saveToFile(const std::vector<FeeRecord>& records) {
    std::vector<std::vector<std::string>> csvData;
    csvData.push_back({"StudentID", "TotalFee", "PaidFee"}); // Header

    for (const auto& record : records) {
        try {
             csvData.push_back(formatFeeRecord(record));
        } catch (const std::exception& e) {
             LOG_ERROR("Error formatting fee record for saving: " + record.studentId() + " Error: " + e.what());
             return false;
        }
    }

    try {
        std::lock_guard<std::mutex> lock(_fileMutex);
        return CsvFileHandler::writeAllLines(_filePath, csvData, DELIMITER);
    } catch (const std::runtime_error& e) {
        LOG_ERROR("Error writing fee data: " + std::string(e.what()));
        return false;
    }
}

FeeRecord CsvFeeRepo::parseFeeRecord(const std::vector<std::string>& csvRow) const {
    if (csvRow.size() < NUM_COLUMNS) {
        throw std::runtime_error("Incorrect number of columns in fee CSV row.");
    }

    long totalFee = 0, paidFee = 0;
    auto resTotal = std::from_chars(csvRow[COL_TOTAL_FEE].data(), csvRow[COL_TOTAL_FEE].data() + csvRow[COL_TOTAL_FEE].size(), totalFee);
    auto resPaid = std::from_chars(csvRow[COL_PAID_FEE].data(), csvRow[COL_PAID_FEE].data() + csvRow[COL_PAID_FEE].size(), paidFee);

    if (resTotal.ec != std::errc()) {
         LOG_WARN("Invalid total fee '" + csvRow[COL_TOTAL_FEE] + "' for student " + csvRow[COL_STUDENT_ID] + ". Defaulting to 0."); totalFee = 0;
    }
    if (resPaid.ec != std::errc()) {
         LOG_WARN("Invalid paid fee '" + csvRow[COL_PAID_FEE] + "' for student " + csvRow[COL_STUDENT_ID] + ". Defaulting to 0."); paidFee = 0;
    }

    return FeeRecord(csvRow[COL_STUDENT_ID], totalFee, paidFee);
}

std::vector<std::string> CsvFeeRepo::formatFeeRecord(const FeeRecord& record) const {
    return {
        record.studentId(),
        std::to_string(record.totalFee()),
        std::to_string(record.paidFee())
    };
}

// --- IFeeRepository Implementation ---
std::optional<FeeRecord> CsvFeeRepo::findByStudentId(const std::string& studentId) {
    auto records = loadFromFile();
    auto it = std::find_if(records.begin(), records.end(), [&](const FeeRecord& r){ return r.studentId() == studentId; });
    return (it != records.end()) ? std::optional<FeeRecord>(*it) : std::nullopt;
}

std::vector<FeeRecord> CsvFeeRepo::findAll() const {
    return loadFromFile();
}

bool CsvFeeRepo::add(const FeeRecord& feeRecord) {
    if (findByStudentId(feeRecord.studentId())) return false; // Already exists
    auto records = loadFromFile();
    records.push_back(feeRecord);
    return saveToFile(records);
}

bool CsvFeeRepo::update(const FeeRecord& feeRecord) {
    auto records = loadFromFile();
    auto it = std::find_if(records.begin(), records.end(), [&](const FeeRecord& r){ return r.studentId() == feeRecord.studentId(); });
    if (it != records.end()) {
        *it = feeRecord;
        return saveToFile(records);
    }
    return false; // Not found
}

bool CsvFeeRepo::remove(const std::string& studentId) {
    auto records = loadFromFile();
    auto it = std::remove_if(records.begin(), records.end(), [&](const FeeRecord& r){ return r.studentId() == studentId; });
    if (it != records.end()) {
        records.erase(it, records.end());
        return saveToFile(records);
    }
    return false; // Not found
}