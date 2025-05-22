#include "SqlSalaryRecordDao.h"
#include <vector>
#include <stdexcept> // For std::invalid_argument

SqlSalaryRecordDao::SqlSalaryRecordDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                                     std::shared_ptr<IEntityParser<SalaryRecord, DbQueryResultRow>> parser)
    : _dbAdapter(std::move(dbAdapter)), _parser(std::move(parser)) {
    if (!_dbAdapter) {
        throw std::invalid_argument("Database adapter cannot be null for SqlSalaryRecordDao.");
    }
    if (!_parser) {
        throw std::invalid_argument("SalaryRecord parser cannot be null for SqlSalaryRecordDao.");
    }
}

std::expected<SalaryRecord, Error> SqlSalaryRecordDao::getById(const std::string& teacherId) const {
    std::string sql = "SELECT teacherId, basicMonthlyPay FROM SalaryRecords WHERE teacherId = ?;";
    std::vector<DbQueryParam> params = {teacherId};

    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    const auto& table = queryResult.value();
    if (table.empty()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "SalaryRecord for Teacher ID " + teacherId + " not found."});
    }
    return _parser->parse(table[0]);
}

std::expected<std::vector<SalaryRecord>, Error> SqlSalaryRecordDao::getAll() const {
    std::string sql = "SELECT teacherId, basicMonthlyPay FROM SalaryRecords;";
    auto queryResult = _dbAdapter->executeQuery(sql);

    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    std::vector<SalaryRecord> records;
    for (const auto& row : queryResult.value()) {
        auto parseResult = _parser->parse(row);
        if (parseResult.has_value()) {
            records.push_back(parseResult.value());
        } else {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse one or more salary records."});
        }
    }
    return records;
}

std::expected<SalaryRecord, Error> SqlSalaryRecordDao::add(const SalaryRecord& salaryRecord) {
    ValidationResult vr = salaryRecord.validateBasic();
    if(!vr.isValid){
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid SalaryRecord data for add: " + vr.getErrorMessagesCombined()});
    }
     // Kiểm tra tồn tại
    auto existCheck = exists(salaryRecord.getTeacherId());
    if (existCheck.has_value() && existCheck.value()){
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "SalaryRecord for Teacher ID '" + salaryRecord.getTeacherId() + "' already exists."});
    }
     if (!existCheck.has_value() && existCheck.error().code != ErrorCode::NOT_FOUND) {
        return std::unexpected(existCheck.error());
    }


    std::string sql = "INSERT INTO SalaryRecords (teacherId, basicMonthlyPay) VALUES (?, ?);";
    auto paramsResult = _parser->toQueryInsertParams(salaryRecord);
     if (!paramsResult.has_value()) {
        return std::unexpected(paramsResult.error());
    }

    auto execResult = _dbAdapter->executeUpdate(sql, paramsResult.value());
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to add salary record, no rows affected."});
    }
    return salaryRecord;
}

std::expected<bool, Error> SqlSalaryRecordDao::update(const SalaryRecord& salaryRecord) {
    ValidationResult vr = salaryRecord.validateBasic();
    if(!vr.isValid){
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid SalaryRecord data for update: " + vr.getErrorMessagesCombined()});
    }
    std::string sql = "UPDATE SalaryRecords SET basicMonthlyPay = ? WHERE teacherId = ?;";
    auto paramsResult = _parser->toQueryUpdateParams(salaryRecord);
    if (!paramsResult.has_value()) {
        return std::unexpected(paramsResult.error());
    }
    
    auto execResult = _dbAdapter->executeUpdate(sql, paramsResult.value());
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "SalaryRecord for Teacher ID " + salaryRecord.getTeacherId() + " not found for update, or no changes made."});
    }
    return true;
}

std::expected<bool, Error> SqlSalaryRecordDao::remove(const std::string& teacherId) {
    // Schema DB: SalaryRecords có ON DELETE CASCADE Users(id)
    std::string sql = "DELETE FROM SalaryRecords WHERE teacherId = ?;";
    std::vector<DbQueryParam> params = {teacherId};
    auto execResult = _dbAdapter->executeUpdate(sql, params);
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "SalaryRecord for Teacher ID " + teacherId + " not found for removal."});
    }
    return true;
}

std::expected<bool, Error> SqlSalaryRecordDao::exists(const std::string& teacherId) const {
    std::string sql = "SELECT 1 FROM SalaryRecords WHERE teacherId = ? LIMIT 1;";
    std::vector<DbQueryParam> params = {teacherId};
    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }
    return !queryResult.value().empty();
}