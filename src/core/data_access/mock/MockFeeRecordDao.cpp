#include "MockFeeRecordDao.h"
#include "../../entities/FeeRecord.h" // Đảm bảo include FeeRecord.h
#include "../../../common/ErrorType.h"
#include <map>
#include <vector>
#include <algorithm>
#include <expected>

// Namespace ẩn danh và initializeMockFeeRecordDataIfNeeded
namespace {
    std::map<std::string, FeeRecord> mock_fee_records_data;
    bool mock_fee_record_data_initialized = false;

    void initializeMockFeeRecordDataIfNeeded() {
        if (!mock_fee_record_data_initialized) {
            
            mock_fee_records_data.emplace("S001", FeeRecord("S001", 5000000, 2500000));
            mock_fee_records_data.emplace("S002", FeeRecord("S002", 5500000, 5500000));
            mock_fee_records_data.emplace("S003", FeeRecord("S003", 4800000, 1000000));
            mock_fee_record_data_initialized = true;
        }
    }
}

MockFeeRecordDao::MockFeeRecordDao() {
    initializeMockFeeRecordDataIfNeeded();
}

std::expected<FeeRecord, Error> MockFeeRecordDao::getById(const std::string& studentId) const {
    auto it = mock_fee_records_data.find(studentId);
    if (it != mock_fee_records_data.end()) {
        return it->second;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock FeeRecord for Student ID " + studentId + " not found."});
}

std::expected<std::vector<FeeRecord>, Error> MockFeeRecordDao::getAll() const {
    std::vector<FeeRecord> records;
    for (const auto& pair : mock_fee_records_data) {
        records.push_back(pair.second);
    }
    return records;
}

std::expected<FeeRecord, Error> MockFeeRecordDao::add(const FeeRecord& feeRecord) {
    ValidationResult vr = feeRecord.validateBasic();
    if (!vr.isValid) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid FeeRecord data: " + vr.getErrorMessagesCombined()});
    }
    if (mock_fee_records_data.count(feeRecord.getStudentId())) {
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Mock FeeRecord for Student ID " + feeRecord.getStudentId() + " already exists."});
    }
    
    // mock_fee_records_data[feeRecord.getStudentId()] = feeRecord; // Dòng cũ
    auto insert_result = mock_fee_records_data.emplace(feeRecord.getStudentId(), feeRecord);
    if (!insert_result.second) {
         return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to emplace fee record into mock data."});
    }
    return feeRecord;
}

std::expected<bool, Error> MockFeeRecordDao::update(const FeeRecord& feeRecord) {
    ValidationResult vr = feeRecord.validateBasic();
    if (!vr.isValid) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid FeeRecord data for update: " + vr.getErrorMessagesCombined()});
    }
    auto it = mock_fee_records_data.find(feeRecord.getStudentId());
    if (it != mock_fee_records_data.end()) {
        it->second = feeRecord; // Gán trực tiếp khi đã tìm thấy là OK
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock FeeRecord for Student ID " + feeRecord.getStudentId() + " not found for update."});
}

std::expected<bool, Error> MockFeeRecordDao::remove(const std::string& studentId) {
    if (mock_fee_records_data.erase(studentId) > 0) {
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock FeeRecord for Student ID " + studentId + " not found for removal."});
}

std::expected<bool, Error> MockFeeRecordDao::exists(const std::string& studentId) const {
    return mock_fee_records_data.count(studentId) > 0;
}