#include "SqlEnrollmentDao.h"
#include <vector>
#include <stdexcept> // For std::invalid_argument
#include "../../database_adapter/sql/sqlite3.h"
#include "../../parsing/impl_sql_parser/SqlParserUtils.h"

SqlEnrollmentDao::SqlEnrollmentDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                                   std::shared_ptr<IEntityParser<EnrollmentRecord, DbQueryResultRow>> parser)
    : _dbAdapter(std::move(dbAdapter)), _parser(std::move(parser)) {
    if (!_dbAdapter) {
        throw std::invalid_argument("Database adapter cannot be null for SqlEnrollmentDao.");
    }
    if (!_parser) {
        throw std::invalid_argument("EnrollmentRecord parser cannot be null for SqlEnrollmentDao.");
    }
}

std::expected<bool, Error> SqlEnrollmentDao::addEnrollment(const std::string& studentId, const std::string& courseId) {
    // enrollmentDate sẽ là DEFAULT CURRENT_TIMESTAMP trong DB
    std::string sql = "INSERT INTO Enrollments (studentId, courseId) VALUES (?, ?);";
    EnrollmentRecord record = {studentId, courseId}; // Parser cần object để lấy params
    auto paramsResult = _parser->toQueryInsertParams(record);
    if(!paramsResult.has_value()){
        return std::unexpected(paramsResult.error());
    }

    auto execResult = _dbAdapter->executeUpdate(sql, paramsResult.value());
    if (!execResult.has_value()) {
        // Kiểm tra lỗi có phải là UNIQUE constraint (đã đăng ký) không
        // SQLite trả về SQLITE_CONSTRAINT_PRIMARYKEY (là SQLITE_CONSTRAINT | (6<<8)) cho lỗi khóa chính
        // và SQLITE_CONSTRAINT_UNIQUE (SQLITE_CONSTRAINT | (8<<8)) cho lỗi unique
        // Mã lỗi chung là SQLITE_CONSTRAINT (19)
        if (execResult.error().code == ErrorCode::ALREADY_EXISTS) {
            return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Student " + studentId + " already enrolled in course " + courseId + "."});
        }
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to add enrollment, no rows affected."});
    }
    return true;
}

std::expected<bool, Error> SqlEnrollmentDao::removeEnrollment(const std::string& studentId, const std::string& courseId) {
    std::string sql = "DELETE FROM Enrollments WHERE studentId = ? AND courseId = ?;";
    std::vector<DbQueryParam> params = {studentId, courseId};

    auto execResult = _dbAdapter->executeUpdate(sql, params);
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Enrollment not found for Student " + studentId + " in Course " + courseId + "."});
    }
    return true;
}

std::expected<bool, Error> SqlEnrollmentDao::removeEnrollmentsByStudent(const std::string& studentId) {
    std::string sql = "DELETE FROM Enrollments WHERE studentId = ?;";
    std::vector<DbQueryParam> params = {studentId};
    auto execResult = _dbAdapter->executeUpdate(sql, params);
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    // Việc không xóa dòng nào (vì SV không đăng ký môn nào) không phải là lỗi.
    return true;
}

std::expected<bool, Error> SqlEnrollmentDao::removeEnrollmentsByCourse(const std::string& courseId) {
    std::string sql = "DELETE FROM Enrollments WHERE courseId = ?;";
    std::vector<DbQueryParam> params = {courseId};
    auto execResult = _dbAdapter->executeUpdate(sql, params);
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    // Tương tự, không xóa dòng nào không phải là lỗi.
    return true;
}

std::expected<bool, Error> SqlEnrollmentDao::isEnrolled(const std::string& studentId, const std::string& courseId) const {
    std::string sql = "SELECT 1 FROM Enrollments WHERE studentId = ? AND courseId = ? LIMIT 1;";
    std::vector<DbQueryParam> params = {studentId, courseId};
    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }
    return !queryResult.value().empty();
}

std::expected<std::vector<std::string>, Error> SqlEnrollmentDao::findCourseIdsByStudentId(const std::string& studentId) const {
    std::string sql = "SELECT courseId FROM Enrollments WHERE studentId = ?;";
    std::vector<DbQueryParam> params = {studentId};
    auto queryResult = _dbAdapter->executeQuery(sql, params);

    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }
    std::vector<std::string> courseIds;
    for (const auto& row : queryResult.value()) {
        try {
            courseIds.push_back(SqlParserUtils::getOptional<std::string>(row, "courseId"));
        } catch (const std::bad_any_cast& e) {
             return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse courseId: " + std::string(e.what())});
        }
    }
    return courseIds;
}

std::expected<std::vector<std::string>, Error> SqlEnrollmentDao::findStudentIdsByCourseId(const std::string& courseId) const {
    std::string sql = "SELECT studentId FROM Enrollments WHERE courseId = ?;";
    std::vector<DbQueryParam> params = {courseId};
    auto queryResult = _dbAdapter->executeQuery(sql, params);

    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }
    std::vector<std::string> studentIds;
    for (const auto& row : queryResult.value()) {
         try {
            studentIds.push_back(SqlParserUtils::getOptional<std::string>(row, "studentId"));
        } catch (const std::bad_any_cast& e) {
             return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse studentId: " + std::string(e.what())});
        }
    }
    return studentIds;
}

std::expected<std::vector<EnrollmentRecord>, Error> SqlEnrollmentDao::getAllEnrollments() const {
    std::string sql = "SELECT studentId, courseId, enrollmentDate FROM Enrollments;"; // Lấy cả enrollmentDate
    auto queryResult = _dbAdapter->executeQuery(sql);

    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }
    std::vector<EnrollmentRecord> records;
    for (const auto& row : queryResult.value()) {
        auto parseResult = _parser->parse(row); // Parser cần được cập nhật để xử lý enrollmentDate nếu có
        if (parseResult.has_value()) {
            records.push_back(parseResult.value());
        } else {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse one or more enrollment records."});
        }
    }
    return records;
}