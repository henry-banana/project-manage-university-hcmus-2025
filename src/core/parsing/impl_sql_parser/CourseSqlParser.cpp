#include "CourseSqlParser.h"
#include "SqlParserUtils.h"

std::expected<Course, Error> CourseSqlParser::parse(const DbQueryResultRow& row) const {
    try {
        std::string id = SqlParserUtils::getOptional<std::string>(row, "id");
        if (id.empty()) {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Course ID not found or empty in SQL row."});
        }
        std::string name = SqlParserUtils::getOptional<std::string>(row, "name");
        
        int credits_val;
        try {
            // Thử cast sang kiểu dữ liệu mà SQLiteAdapter có thể trả về cho INTEGER
            // SQLiteAdapter thường trả về int64_t cho cột INTEGER
            credits_val = static_cast<int>(SqlParserUtils::getOptional<long long>(row, "credits", 0LL));
        } catch (const std::bad_any_cast& ) {
            // Nếu không phải long long, thử int (ít khả năng hơn nếu từ DB)
            try {
                credits_val = SqlParserUtils::getOptional<int>(row, "credits", 0);
            } catch (const std::bad_any_cast& e) {
                return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse Course credits: " + std::string(e.what())});
            }
        }
        int credits = credits_val;

        std::string facultyId = SqlParserUtils::getOptional<std::string>(row, "facultyId");

        if (name.empty()) {
             return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Course name not found or empty in SQL row."});
        }
        if (credits <= 0) {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Course credits invalid in SQL row."});
        }
        // facultyId có thể null trong DB, Course constructor có thể cần điều chỉnh hoặc Course entity
        // chấp nhận facultyId rỗng và service sẽ validate sau. Giả sử Course entity chấp nhận.

        return Course(id, name, credits, facultyId);
    } catch (const std::bad_any_cast& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse Course from SQL row: " + std::string(e.what())});
    } catch (const std::exception& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Generic error parsing Course from SQL row: " + std::string(e.what())});
    }
}

std::expected<DbQueryResultRow, Error> CourseSqlParser::serialize(const Course& course) const {
    DbQueryResultRow row;
    row.emplace("id", course.getId());
    row.emplace("name", course.getName());
    row.emplace("credits", course.getCredits());
    row.emplace("facultyId", course.getFacultyId().empty() ? std::any{} : std::any{course.getFacultyId()});
    return row;
}

std::expected<std::vector<std::any>, Error> CourseSqlParser::toQueryInsertParams(const Course& course) const {
    std::vector<std::any> params;
    params.push_back(course.getId());
    params.push_back(course.getName());
    params.push_back(course.getCredits());
    params.push_back(course.getFacultyId().empty() ? std::any{} : std::any{course.getFacultyId()});
    return params;
}

std::expected<std::vector<std::any>, Error> CourseSqlParser::toQueryUpdateParams(const Course& course) const {
    std::vector<std::any> params;
    params.push_back(course.getName());
    params.push_back(course.getCredits());
    params.push_back(course.getFacultyId().empty() ? std::any{} : std::any{course.getFacultyId()});
    params.push_back(course.getId()); // For WHERE clause
    return params;
}