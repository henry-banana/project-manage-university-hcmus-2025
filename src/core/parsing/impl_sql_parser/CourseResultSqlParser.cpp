#include "CourseResultSqlParser.h"
#include "SqlParserUtils.h"

std::expected<CourseResult, Error> CourseResultSqlParser::parse(const DbQueryResultRow& row) const {
    try {
        std::string studentId = SqlParserUtils::getOptional<std::string>(row, "studentId");
        std::string courseId = SqlParserUtils::getOptional<std::string>(row, "courseId");
        
        if (studentId.empty() || courseId.empty()) {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "StudentId or CourseId not found or empty in CourseResult SQL row."});
        }

        int marks = -1; // Mặc định
        auto marks_it = row.find("marks");
        if (marks_it != row.end() && marks_it->second.has_value()) {
            marks = SqlParserUtils::getOptional<long long>(row, "marks", -1);
        }
        // Trường 'grade' trong DB là TEXT, nhưng trong CourseResult là char và được tính toán.
        // Parser không cần đọc 'grade' từ DB nếu nó luôn được tính lại.
        // Nếu DB lưu grade, thì đọc:
        // std::string grade_str = SqlParserUtils::getOptional<std::string>(row, "grade");
        // char grade = grade_str.empty() ? '-' : grade_str[0];

        return CourseResult(studentId, courseId, marks);

    } catch (const std::bad_any_cast& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse CourseResult from SQL row: " + std::string(e.what())});
    } catch (const std::exception& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Generic error parsing CourseResult from SQL row: " + std::string(e.what())});
    }
}

std::expected<DbQueryResultRow, Error> CourseResultSqlParser::serialize(const CourseResult& result) const {
    DbQueryResultRow row;
    row.emplace("studentId", result.getStudentId());
    row.emplace("courseId", result.getCourseId());
    if (result.getMarks() == -1) {
        row.emplace("marks", std::any{}); // SQL NULL
    } else {
        row.emplace("marks", result.getMarks());
    }
    // Grade được tính, nhưng nếu schema DB có cột grade thì cũng nên set
    // row.emplace("grade", std::string(1, result.getGrade()));
    return row;
}

// Thường dùng cho INSERT OR REPLACE hoặc một logic addOrUpdate
std::expected<std::vector<std::any>, Error> CourseResultSqlParser::toQueryInsertParams(const CourseResult& result) const {
    std::vector<std::any> params;
    params.push_back(result.getStudentId());
    params.push_back(result.getCourseId());
    if (result.getMarks() == -1) {
        params.push_back(std::any{}); // SQL NULL
    } else {
        params.push_back(result.getMarks());
    }
    // Grade không cần bind nếu DB tự tính hoặc ứng dụng tính khi đọc
    return params;
}

// Thường dùng cho UPDATE SET marks = ? WHERE studentId = ? AND courseId = ?
std::expected<std::vector<std::any>, Error> CourseResultSqlParser::toQueryUpdateParams(const CourseResult& result) const {
    std::vector<std::any> params;
    if (result.getMarks() == -1) {
        params.push_back(std::any{}); // marks = SQL NULL
    } else {
        params.push_back(result.getMarks());
    }
    // Grade không cần bind nếu DB tự tính hoặc ứng dụng tính khi đọc
    params.push_back(result.getStudentId()); // for WHERE
    params.push_back(result.getCourseId());  // for WHERE
    return params;
}