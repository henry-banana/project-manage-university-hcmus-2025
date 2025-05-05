#include "CsvSalaryRepo.h"
#include "../../../utils/Logger.h"
#include <stdexcept>
#include <algorithm>
#include <charconv>

CsvSalaryRepo::CsvSalaryRepo(std::string csvFilePath) : _filePath(std::move(csvFilePath)) {
    // Optional: Initialize file/header
}

// --- Helper Functions ---
std::vector<SalaryRecord> CsvSalaryRepo::loadFromFile() const {
     std::vector<SalaryRecord> records;
     std::vector<std::vector<std::string>> csvData;
     try {
         std::lock_guard<std::mutex> lock(_fileMutex);
         csvData = CsvFileHandler::readAllLines(_filePath, DELIMITER);
     } catch (const std::runtime_error& e) {
         LOG_ERROR("Error reading salary data: " + std::string(e.what()));
         return records;
     }

     bool isFirstRow = true;
     for (const auto& row : csvData) {
         if (isFirstRow) {
             isFirstRow = false; continue; // Skip header
         }
         try {
             records.push_back(parseSalaryRecord(row));
         } catch (const std::exception& e) {
              LOG_ERROR("Skipping invalid salary row: " + CsvFileHandler::join(row, DELIMITER) + " Error: " + e.what());
         }
     }
     return records;
}

bool CsvSalaryRepo::saveToFile(const std::vector<SalaryRecord>& records) {
    std::vector<std::vector<std::string>> csvData;
    csvData.push_back({"TeacherID", "BasicMonthlyPay"}); // Header

    for (const auto& record : records) {
         try {
              csvData.push_back(formatSalaryRecord(record));
         } catch (const std::exception& e) {
              LOG_ERROR("Error formatting salary record for saving: " + record.teacherId() + " Error: " + e.what());
              return false;
         }
    }

    try {
        std::lock_guard<std::mutex> lock(_fileMutex);
        return CsvFileHandler::writeAllLines(_filePath, csvData, DELIMITER);
    } catch (const std::runtime_error& e) {
        LOG_ERROR("Error writing salary data: " + std::string(e.what()));
        return false;
    }
}

SalaryRecord CsvSalaryRepo::parseSalaryRecord(const std::vector<std::string>& csvRow) const {
     if (csvRow.size() < NUM_COLUMNS) {
         throw std::runtime_error("Incorrect number of columns in salary CSV row.");
     }

     long basicPay = 0;
     auto res = std::from_chars(csvRow[COL_BASIC_MONTHLY_PAY].data(), csvRow[COL_BASIC_MONTHLY_PAY].data() + csvRow[COL_BASIC_MONTHLY_PAY].size(), basicPay);

     if (res.ec != std::errc()) {
          LOG_WARN("Invalid basic pay '" + csvRow[COL_BASIC_MONTHLY_PAY] + "' for teacher " + csvRow[COL_TEACHER_ID] + ". Defaulting to 0."); basicPay = 0;
     }

     return SalaryRecord(csvRow[COL_TEACHER_ID], basicPay);
}

std::vector<std::string> CsvSalaryRepo::formatSalaryRecord(const SalaryRecord& record) const {
     return {
         record.teacherId(),
         std::to_string(record.basicMonthlyPay())
     };
}

// --- ISalaryRepository Implementation ---
std::optional<SalaryRecord> CsvSalaryRepo::findByTeacherId(const std::string& teacherId) const {
    auto records = loadFromFile();
    auto it = std::find_if(records.begin(), records.end(), [&](const SalaryRecord& r){ return r.teacherId() == teacherId; });
    return (it != records.end()) ? std::optional<SalaryRecord>(*it) : std::nullopt;
}

std::vector<SalaryRecord> CsvSalaryRepo::findAll() const {
    return loadFromFile();
}

bool CsvSalaryRepo::add(const SalaryRecord& salaryRecord) {
    if (findByTeacherId(salaryRecord.teacherId())) return false; // Already exists
    auto records = loadFromFile();
    records.push_back(salaryRecord);
    return saveToFile(records);
}

bool CsvSalaryRepo::update(const SalaryRecord& salaryRecord) {
    auto records = loadFromFile();
    auto it = std::find_if(records.begin(), records.end(), [&](const SalaryRecord& r){ return r.teacherId() == salaryRecord.teacherId(); });
    if (it != records.end()) {
        *it = salaryRecord;
        return saveToFile(records);
    }
    return false; // Not found
}

bool CsvSalaryRepo::remove(const std::string& teacherId) {
    auto records = loadFromFile();
    auto it = std::remove_if(records.begin(), records.end(), [&](const SalaryRecord& r){ return r.teacherId() == teacherId; });
    if (it != records.end()) {
        records.erase(it, records.end());
        return saveToFile(records);
    }
    return false; // Not found
}