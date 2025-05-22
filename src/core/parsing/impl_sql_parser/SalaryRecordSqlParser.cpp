#include "SalaryRecordSqlParser.h"

std::expected<SalaryRecord, Error> SalaryRecordSqlParser::parse(const DbQueryResultRow& row) const {
    try {
        std::string teacherId = SqlParserUtils::getOptional<std::string>(row, "teacherId");
        if (teacherId.empty()) {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "SalaryRecord teacherId not found or empty in SQL row."});
        }
        long basicPay = SqlParserUtils::getOptional<long long>(row, "basicMonthlyPay", 0L);

        return SalaryRecord(teacherId, basicPay);
    } catch (const std::bad_any_cast& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse SalaryRecord from SQL row: " + std::string(e.what())});
    } catch (const std::exception& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Generic error parsing SalaryRecord from SQL row: " + std::string(e.what())});
    }
}

std::expected<DbQueryResultRow, Error> SalaryRecordSqlParser::serialize(const SalaryRecord& record) const {
    DbQueryResultRow row;
    row["teacherId"] = record.getTeacherId();
    row["basicMonthlyPay"] = record.getBasicMonthlyPay();
    return row;
}

std::expected<std::vector<std::any>, Error> SalaryRecordSqlParser::toQueryInsertParams(const SalaryRecord& record) const {
    std::vector<std::any> params;
    params.push_back(record.getTeacherId());
    params.push_back(record.getBasicMonthlyPay());
    return params;
}

std::expected<std::vector<std::any>, Error> SalaryRecordSqlParser::toQueryUpdateParams(const SalaryRecord& record) const {
    std::vector<std::any> params;
    params.push_back(record.getBasicMonthlyPay());
    params.push_back(record.getTeacherId()); // For WHERE clause
    return params;
}