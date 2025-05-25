#ifndef ENROLLMENTRECORDSQLPARSER_H
#define ENROLLMENTRECORDSQLPARSER_H

#include "../interface/IEntityParser.h" // Vẫn dùng interface này cho struct EnrollmentRecord
#include "../../data_access/interface/IEnrollmentDao.h" // Để dùng struct EnrollmentRecord
#include "StudentSqlParser.h" // Để dùng SqlParserUtils

class EnrollmentRecordSqlParser : public IEntityParser<EnrollmentRecord, DbQueryResultRow> {
public:
    EnrollmentRecordSqlParser() = default;

    std::expected<EnrollmentRecord, Error> parse(const DbQueryResultRow& row) const override;
    std::expected<DbQueryResultRow, Error> serialize(const EnrollmentRecord& record) const override;
    // enrollmentDate có thể là TEXT DEFAULT CURRENT_TIMESTAMP trong DB
    std::expected<std::vector<std::any>, Error> toQueryInsertParams(const EnrollmentRecord& record) const override; // studentId, courseId, enrollmentDate (nếu có)
    
    // Enrollment thường không update, chỉ add/remove. Nếu có update (ví dụ enrollmentDate) thì cần hàm này.
    // Hiện tại, giả định không có update trực tiếp record, mà là remove rồi add lại nếu cần.
    // Do đó, toQueryUpdateParams có thể không cần thiết hoặc trả về lỗi.
    std::expected<std::vector<std::any>, Error> toQueryUpdateParams(const EnrollmentRecord& record) const override {
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "EnrollmentRecord direct update not typically supported via this parser method."});
    }
};

#endif // ENROLLMENTRECORDSQLPARSER_H