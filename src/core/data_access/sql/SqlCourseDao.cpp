#include "SqlCourseDao.h"
#include <vector>

SqlCourseDao::SqlCourseDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                           std::shared_ptr<IEntityParser<Course, DbQueryResultRow>> parser)
    : _dbAdapter(std::move(dbAdapter)), _parser(std::move(parser)) {
    if (!_dbAdapter) {
        throw std::invalid_argument("Database adapter cannot be null for SqlCourseDao.");
    }
    if (!_parser) {
        throw std::invalid_argument("Course parser cannot be null for SqlCourseDao.");
    }
}

std::expected<Course, Error> SqlCourseDao::getById(const std::string& id) const {
    std::string sql = "SELECT id, name, credits, facultyId FROM Courses WHERE id = ?;";
    std::vector<DbQueryParam> params = {id};

    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    const auto& table = queryResult.value();
    if (table.empty()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Course with ID " + id + " not found."});
    }
    return _parser->parse(table[0]);
}

std::expected<std::vector<Course>, Error> SqlCourseDao::getAll() const {
    std::string sql = "SELECT id, name, credits, facultyId FROM Courses;";
    auto queryResult = _dbAdapter->executeQuery(sql);

    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    std::vector<Course> courses;
    for (const auto& row : queryResult.value()) {
        auto parseResult = _parser->parse(row);
        if (parseResult.has_value()) {
            courses.push_back(parseResult.value());
        } else {
            // Log lỗi hoặc xử lý
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse one or more courses."});
        }
    }
    return courses;
}

std::expected<Course, Error> SqlCourseDao::add(const Course& course) {
    ValidationResult vr = course.validateBasic();
    if (!vr.isValid) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid Course data for add: " + vr.getErrorMessagesCombined()});
    }
    // Kiểm tra trùng ID
    auto existIdCheck = exists(course.getId());
    if (existIdCheck.has_value() && existIdCheck.value()){
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Course with ID '" + course.getId() + "' already exists."});
    }
     if (!existIdCheck.has_value() && existIdCheck.error().code != ErrorCode::NOT_FOUND) {
        return std::unexpected(existIdCheck.error());
    }

    std::string sql = "INSERT INTO Courses (id, name, credits, facultyId) VALUES (?, ?, ?, ?);";
    auto paramsResult = _parser->toQueryInsertParams(course);
    if (!paramsResult.has_value()) {
        return std::unexpected(paramsResult.error());
    }
    
    auto execResult = _dbAdapter->executeUpdate(sql, paramsResult.value());
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to add course, no rows affected."});
    }
    return course;
}

std::expected<bool, Error> SqlCourseDao::update(const Course& course) {
    ValidationResult vr = course.validateBasic();
    if (!vr.isValid) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid Course data for update: " + vr.getErrorMessagesCombined()});
    }

    std::string sql = "UPDATE Courses SET name = ?, credits = ?, facultyId = ? WHERE id = ?;";
    auto paramsResult = _parser->toQueryUpdateParams(course);
     if (!paramsResult.has_value()) {
        return std::unexpected(paramsResult.error());
    }

    auto execResult = _dbAdapter->executeUpdate(sql, paramsResult.value());
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
         return std::unexpected(Error{ErrorCode::NOT_FOUND, "Course with ID " + course.getId() + " not found for update, or no changes made."});
    }
    return true;
}

std::expected<bool, Error> SqlCourseDao::remove(const std::string& id) {
    // Schema DB: Enrollments và CourseResults có ON DELETE CASCADE Courses(id)
    std::string sql = "DELETE FROM Courses WHERE id = ?;";
    std::vector<DbQueryParam> params = {id};

    auto execResult = _dbAdapter->executeUpdate(sql, params);
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Course with ID " + id + " not found for removal."});
    }
    return true;
}

std::expected<bool, Error> SqlCourseDao::exists(const std::string& id) const {
    std::string sql = "SELECT 1 FROM Courses WHERE id = ? LIMIT 1;";
    std::vector<DbQueryParam> params = {id};
    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }
    return !queryResult.value().empty();
}

std::expected<std::vector<Course>, Error> SqlCourseDao::findByFacultyId(const std::string& facultyId) const {
    std::string sql = "SELECT id, name, credits, facultyId FROM Courses WHERE facultyId = ?;";
    std::vector<DbQueryParam> params = {facultyId};
    auto queryResult = _dbAdapter->executeQuery(sql, params);

    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    std::vector<Course> courses;
    for (const auto& row : queryResult.value()) {
        auto parseResult = _parser->parse(row);
        if (parseResult.has_value()) {
            courses.push_back(parseResult.value());
        } else {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse one or more courses for faculty " + facultyId});
        }
    }
    return courses;
}