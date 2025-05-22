#include "FeeRecordSqlParser.h"

std::expected<FeeRecord, Error> FeeRecordSqlParser::parse(const DbQueryResultRow& row) const {
    try {
        std::string studentId = SqlParserUtils::getOptional<std::string>(row, "studentId");
        if (studentId.empty()) {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "FeeRecord studentId not found or empty in SQL row."});
        }
        long totalFee = SqlParserUtils::getOptional<long long>(row, "totalFee", 0L);
        long paidFee = SqlParserUtils::getOptional<long long>(row, "paidFee", 0L);

        return FeeRecord(studentId, totalFee, paidFee);
    } catch (const std::bad_any_cast& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse FeeRecord from SQL row: " + std::string(e.what())});
    } catch (const std::exception& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Generic error parsing FeeRecord from SQL row: " + std::string(e.what())});
    }
}

std::expected<DbQueryResultRow, Error> FeeRecordSqlParser::serialize(const FeeRecord& record) const {
    DbQueryResultRow row;
    row["studentId"] = record.getStudentId();
    row["totalFee"] = record.getTotalFee();
    row["paidFee"] = record.getPaidFee();
    return row;
}

std::expected<std::vector<std::any>, Error> FeeRecordSqlParser::toQueryInsertParams(const FeeRecord& record) const {
    std::vector<std::any> params;
    params.push_back(record.getStudentId());
    params.push_back(record.getTotalFee());
    params.push_back(record.getPaidFee());
    return params;
}

std::expected<std::vector<std::any>, Error> FeeRecordSqlParser::toQueryUpdateParams(const FeeRecord& record) const {
    std::vector<std::any> params;
    params.push_back(record.getTotalFee());
    params.push_back(record.getPaidFee());
    params.push_back(record.getStudentId()); // For WHERE clause
    return params;
}