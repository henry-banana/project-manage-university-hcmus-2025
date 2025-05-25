#include "SqlFeeRecordDao.h"
#include <vector>
#include <stdexcept> // For std::invalid_argument

SqlFeeRecordDao::SqlFeeRecordDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                                 std::shared_ptr<IEntityParser<FeeRecord, DbQueryResultRow>> parser)
    : _dbAdapter(std::move(dbAdapter)), _parser(std::move(parser)) {
    if (!_dbAdapter) {
        throw std::invalid_argument("Database adapter cannot be null for SqlFeeRecordDao.");
    }
    if (!_parser) {
        throw std::invalid_argument("FeeRecord parser cannot be null for SqlFeeRecordDao.");
    }
}

std::expected<FeeRecord, Error> SqlFeeRecordDao::getById(const std::string& studentId) const {
    std::string sql = "SELECT studentId, totalFee, paidFee FROM FeeRecords WHERE studentId = ?;";
    std::vector<DbQueryParam> params = {studentId};

    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    const auto& table = queryResult.value();
    if (table.empty()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "FeeRecord for Student ID " + studentId + " not found."});
    }
    return _parser->parse(table[0]);
}

std::expected<std::vector<FeeRecord>, Error> SqlFeeRecordDao::getAll() const {
    std::string sql = "SELECT studentId, totalFee, paidFee FROM FeeRecords;";
    auto queryResult = _dbAdapter->executeQuery(sql);

    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    std::vector<FeeRecord> records;
    for (const auto& row : queryResult.value()) {
        auto parseResult = _parser->parse(row);
        if (parseResult.has_value()) {
            records.push_back(parseResult.value());
        } else {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse one or more fee records."});
        }
    }
    return records;
}

std::expected<FeeRecord, Error> SqlFeeRecordDao::add(const FeeRecord& feeRecord) {
    ValidationResult vr = feeRecord.validateBasic();
    if(!vr.isValid){
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid FeeRecord data for add: " + vr.getErrorMessagesCombined()});
    }
    // Kiểm tra tồn tại
    auto existCheck = exists(feeRecord.getStudentId());
    if (existCheck.has_value() && existCheck.value()){
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "FeeRecord for Student ID '" + feeRecord.getStudentId() + "' already exists."});
    }
    if (!existCheck.has_value() && existCheck.error().code != ErrorCode::NOT_FOUND) {
        return std::unexpected(existCheck.error());
    }


    std::string sql = "INSERT INTO FeeRecords (studentId, totalFee, paidFee) VALUES (?, ?, ?);";
    auto paramsResult = _parser->toQueryInsertParams(feeRecord);
     if (!paramsResult.has_value()) {
        return std::unexpected(paramsResult.error());
    }

    auto execResult = _dbAdapter->executeUpdate(sql, paramsResult.value());
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to add fee record, no rows affected."});
    }
    return feeRecord;
}

std::expected<bool, Error> SqlFeeRecordDao::update(const FeeRecord& feeRecord) {
    ValidationResult vr = feeRecord.validateBasic();
    if(!vr.isValid){
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid FeeRecord data for update: " + vr.getErrorMessagesCombined()});
    }
    std::string sql = "UPDATE FeeRecords SET totalFee = ?, paidFee = ? WHERE studentId = ?;";
    auto paramsResult = _parser->toQueryUpdateParams(feeRecord);
    if (!paramsResult.has_value()) {
        return std::unexpected(paramsResult.error());
    }

    auto execResult = _dbAdapter->executeUpdate(sql, paramsResult.value());
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "FeeRecord for Student ID " + feeRecord.getStudentId() + " not found for update, or no changes made."});
    }
    return true;
}

std::expected<bool, Error> SqlFeeRecordDao::remove(const std::string& studentId) {
    // Schema DB: FeeRecords có ON DELETE CASCADE Users(id)
    // Hàm này sẽ xóa FeeRecord độc lập (nếu User không bị xóa)
    // Hoặc không cần thiết nếu việc xóa User đã xử lý cascade.
    // Giả sử hàm này dùng để xóa FeeRecord mà không xóa Student.
    std::string sql = "DELETE FROM FeeRecords WHERE studentId = ?;";
    std::vector<DbQueryParam> params = {studentId};
    auto execResult = _dbAdapter->executeUpdate(sql, params);
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "FeeRecord for Student ID " + studentId + " not found for removal."});
    }
    return true;
}

std::expected<bool, Error> SqlFeeRecordDao::exists(const std::string& studentId) const {
    std::string sql = "SELECT 1 FROM FeeRecords WHERE studentId = ? LIMIT 1;";
    std::vector<DbQueryParam> params = {studentId};
    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }
    return !queryResult.value().empty();
}