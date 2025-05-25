#include "SqlCourseResultDao.h"
#include <vector>
#include <stdexcept> // For std::invalid_argument

SqlCourseResultDao::SqlCourseResultDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                                       std::shared_ptr<IEntityParser<CourseResult, DbQueryResultRow>> parser)
    : _dbAdapter(std::move(dbAdapter)), _parser(std::move(parser)) {
    if (!_dbAdapter) {
        throw std::invalid_argument("Database adapter cannot be null for SqlCourseResultDao.");
    }
    if (!_parser) {
        throw std::invalid_argument("CourseResult parser cannot be null for SqlCourseResultDao.");
    }
}

std::expected<CourseResult, Error> SqlCourseResultDao::find(const std::string& studentId, const std::string& courseId) const {
    std::string sql = "SELECT studentId, courseId, marks, grade FROM CourseResults WHERE studentId = ? AND courseId = ?;";
    std::vector<DbQueryParam> params = {studentId, courseId};

    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    const auto& table = queryResult.value();
    if (table.empty()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "CourseResult not found for Student " + studentId + ", Course " + courseId});
    }
    return _parser->parse(table[0]);
}

std::expected<std::vector<CourseResult>, Error> SqlCourseResultDao::findByStudentId(const std::string& studentId) const {
    std::string sql = "SELECT studentId, courseId, marks, grade FROM CourseResults WHERE studentId = ?;";
    std::vector<DbQueryParam> params = {studentId};
    auto queryResult = _dbAdapter->executeQuery(sql, params);

    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    std::vector<CourseResult> results;
    for (const auto& row : queryResult.value()) {
        auto parseResult = _parser->parse(row);
        if (parseResult.has_value()) {
            results.push_back(parseResult.value());
        } else {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse one or more course results for student " + studentId});
        }
    }
    return results;
}

std::expected<std::vector<CourseResult>, Error> SqlCourseResultDao::findByCourseId(const std::string& courseId) const {
    std::string sql = "SELECT studentId, courseId, marks, grade FROM CourseResults WHERE courseId = ?;";
    std::vector<DbQueryParam> params = {courseId};
    auto queryResult = _dbAdapter->executeQuery(sql, params);

    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    std::vector<CourseResult> results;
    for (const auto& row : queryResult.value()) {
        auto parseResult = _parser->parse(row);
        if (parseResult.has_value()) {
            results.push_back(parseResult.value());
        } else {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse one or more course results for course " + courseId});
        }
    }
    return results;
}

std::expected<bool, Error> SqlCourseResultDao::addOrUpdate(const CourseResult& result) {
    ValidationResult vr = result.validate();
    if(!vr.isValid){
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid CourseResult data: " + vr.getErrorMessagesCombined()});
    }

    // Schema DB có PRIMARY KEY (studentId, courseId) và CHECK (marks IS NULL OR (marks >= -1 AND marks <= 100))
    // Grade không cần insert/update vì nó được tính.
    // SQLite không có `INSERT ... ON DUPLICATE KEY UPDATE` trực tiếp như MySQL.
    // Cách phổ biến là `INSERT OR REPLACE` hoặc `INSERT ... ON CONFLICT (...) DO UPDATE SET ...`
    // Hoặc kiểm tra tồn tại rồi INSERT/UPDATE riêng.
    // Sử dụng INSERT OR REPLACE cho đơn giản, hoặc INSERT ON CONFLICT nếu chỉ muốn update marks.
    // "INSERT OR REPLACE INTO CourseResults (studentId, courseId, marks) VALUES (?, ?, ?);"
    // Hoặc:
    std::string sql = "INSERT INTO CourseResults (studentId, courseId, marks) VALUES (?, ?, ?) "
                      "ON CONFLICT(studentId, courseId) DO UPDATE SET marks = excluded.marks;";
                      // excluded.marks (SQLite) hoặc VALUES(marks) (MySQL) để lấy giá trị mới

    auto paramsResult = _parser->toQueryInsertParams(result); // studentId, courseId, marks
    if(!paramsResult.has_value()){
        return std::unexpected(paramsResult.error());
    }

    auto execResult = _dbAdapter->executeUpdate(sql, paramsResult.value());
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    // executeUpdate trả về số dòng bị ảnh hưởng. Với INSERT OR REPLACE, có thể là 1 (nếu insert) hoặc 2 (nếu replace = delete+insert).
    // Với INSERT ON CONFLICT DO UPDATE, là 1.
    if (execResult.value() == 0) { 
        // Điều này không nên xảy ra với INSERT OR REPLACE hoặc INSERT ON CONFLICT DO UPDATE nếu không có lỗi
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to add or update course result, no rows affected unexpectedly."});
    }
    return true;
}

std::expected<bool, Error> SqlCourseResultDao::remove(const std::string& studentId, const std::string& courseId) {
    std::string sql = "DELETE FROM CourseResults WHERE studentId = ? AND courseId = ?;";
    std::vector<DbQueryParam> params = {studentId, courseId};
    auto execResult = _dbAdapter->executeUpdate(sql, params);
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "CourseResult not found for removal (Student: " + studentId + ", Course: " + courseId + ")."});
    }
    return true;
}

std::expected<bool, Error> SqlCourseResultDao::removeAllForStudent(const std::string& studentId) {
    std::string sql = "DELETE FROM CourseResults WHERE studentId = ?;";
    std::vector<DbQueryParam> params = {studentId};
    auto execResult = _dbAdapter->executeUpdate(sql, params);
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    return true; // Không coi là lỗi nếu không có gì để xóa
}

std::expected<bool, Error> SqlCourseResultDao::removeAllForCourse(const std::string& courseId) {
    std::string sql = "DELETE FROM CourseResults WHERE courseId = ?;";
    std::vector<DbQueryParam> params = {courseId};
    auto execResult = _dbAdapter->executeUpdate(sql, params);
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    return true; // Không coi là lỗi nếu không có gì để xóa
}