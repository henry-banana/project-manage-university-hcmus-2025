#include "EnrollmentRecordSqlParser.h"

std::expected<EnrollmentRecord, Error> EnrollmentRecordSqlParser::parse(const DbQueryResultRow& row) const {
    try {
        EnrollmentRecord record;
        record.studentId = SqlParserUtils::getOptional<std::string>(row, "studentId");
        record.courseId = SqlParserUtils::getOptional<std::string>(row, "courseId");

        if (record.studentId.empty() || record.courseId.empty()) {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "studentId or courseId not found or empty in EnrollmentRecord SQL row."});
        }
        // enrollmentDate không có trong struct EnrollmentRecord ban đầu, nhưng có trong schema DB.
        // Nếu cần, thêm vào struct và parse ở đây:
        // record.enrollmentDate = SqlParserUtils::getOptional<std::string>(row, "enrollmentDate");

        return record;
    } catch (const std::bad_any_cast& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse EnrollmentRecord from SQL row: " + std::string(e.what())});
    } catch (const std::exception& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Generic error parsing EnrollmentRecord from SQL row: " + std::string(e.what())});
    }
}

std::expected<DbQueryResultRow, Error> EnrollmentRecordSqlParser::serialize(const EnrollmentRecord& record) const {
    DbQueryResultRow row;
    row["studentId"] = record.studentId;
    row["courseId"] = record.courseId;
    // row["enrollmentDate"] = record.enrollmentDate; // nếu có
    return row;
}

std::expected<std::vector<std::any>, Error> EnrollmentRecordSqlParser::toQueryInsertParams(const EnrollmentRecord& record) const {
    std::vector<std::any> params;
    params.push_back(record.studentId);
    params.push_back(record.courseId);
    // params.push_back(record.enrollmentDate); // Nếu enrollmentDate được quản lý bởi ứng dụng
    // Nếu enrollmentDate là DEFAULT CURRENT_TIMESTAMP trong DB, thì không cần bind ở đây
    // cho INSERT.
    return params;
}