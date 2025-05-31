#include "FeeRecordSqlParser.h"
#include "SqlParserUtils.h"

std::expected<FeeRecord, Error> FeeRecordSqlParser::parse(const DbQueryResultRow& row) const {
    try {
        std::string studentId = SqlParserUtils::getOptional<std::string>(row, "studentId");
        if (studentId.empty()) { // Giữ nguyên check này
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "FeeRecord studentId not found or empty in SQL row."});
        }

        // Kiểm tra sự tồn tại của key trước khi lấy giá trị
        if (row.find("totalFee") == row.end()) {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "FeeRecord totalFee not found in SQL row."});
        }
        long totalFee = SqlParserUtils::getOptional<long long>(row, "totalFee", 0L); // Default value ít quan trọng hơn nếu đã check key

        if (row.find("paidFee") == row.end()) {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "FeeRecord paidFee not found in SQL row."});
        }
        long paidFee = SqlParserUtils::getOptional<long long>(row, "paidFee", 0L);

        return FeeRecord(studentId, totalFee, paidFee);
    } catch (const std::bad_any_cast& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse FeeRecord: " + std::string(e.what())});
    } catch (const std::exception& e) { // Bắt lỗi chung hơn
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Generic error parsing FeeRecord: " + std::string(e.what())});
    }
}

std::expected<DbQueryResultRow, Error> FeeRecordSqlParser::serialize(const FeeRecord& record) const {
    DbQueryResultRow row;
    row.emplace("studentId", record.getStudentId());
    row.emplace("totalFee", record.getTotalFee());
    row.emplace("paidFee", record.getPaidFee());
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