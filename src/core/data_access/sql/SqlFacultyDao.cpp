#include "SqlFacultyDao.h"
#include <vector>

SqlFacultyDao::SqlFacultyDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                             std::shared_ptr<IEntityParser<Faculty, DbQueryResultRow>> parser)
    : _dbAdapter(std::move(dbAdapter)), _parser(std::move(parser)) {
    if (!_dbAdapter) {
        throw std::invalid_argument("Database adapter cannot be null for SqlFacultyDao.");
    }
    if (!_parser) {
        throw std::invalid_argument("Faculty parser cannot be null for SqlFacultyDao.");
    }
}

std::expected<Faculty, Error> SqlFacultyDao::getById(const std::string& id) const {
    std::string sql = "SELECT id, name FROM Faculties WHERE id = ?;";
    std::vector<DbQueryParam> params = {id};

    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    const auto& table = queryResult.value();
    if (table.empty()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Faculty with ID " + id + " not found."});
    }
    return _parser->parse(table[0]);
}

std::expected<std::vector<Faculty>, Error> SqlFacultyDao::getAll() const {
    std::string sql = "SELECT id, name FROM Faculties;";
    auto queryResult = _dbAdapter->executeQuery(sql);

    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    std::vector<Faculty> faculties;
    for (const auto& row : queryResult.value()) {
        auto parseResult = _parser->parse(row);
        if (parseResult.has_value()) {
            faculties.push_back(parseResult.value());
        } else {
            // Log lỗi parse hoặc trả về lỗi ngay lập tức tùy chiến lược
            // Hiện tại, bỏ qua row lỗi và tiếp tục
            // TODO: Xem xét log lỗi parse
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse one or more faculties."});
        }
    }
    return faculties;
}

std::expected<Faculty, Error> SqlFacultyDao::add(const Faculty& faculty) {
    ValidationResult vr = faculty.validateBasic();
    if (!vr.isValid) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid Faculty data for add: " + vr.getErrorMessagesCombined()});
    }

    // Kiểm tra trùng tên trước khi thêm
    auto existNameCheck = findByName(faculty.getName());
    if (existNameCheck.has_value()) {
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Faculty with name '" + faculty.getName() + "' already exists."});
    }
    // Kiểm tra trùng ID
    auto existIdCheck = exists(faculty.getId());
    if (existIdCheck.has_value() && existIdCheck.value()){
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Faculty with ID '" + faculty.getId() + "' already exists."});
    }
    if (!existIdCheck.has_value() && existIdCheck.error().code != ErrorCode::NOT_FOUND) { // Error khác NOT_FOUND
        return std::unexpected(existIdCheck.error());
    }


    std::string sql = "INSERT INTO Faculties (id, name) VALUES (?, ?);";
    auto paramsResult = _parser->toQueryInsertParams(faculty);
    if (!paramsResult.has_value()) {
        return std::unexpected(paramsResult.error());
    }

    auto execResult = _dbAdapter->executeUpdate(sql, paramsResult.value());
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) { // Không có dòng nào được thêm
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to add faculty, no rows affected."});
    }
    return faculty; // Trả về faculty đã thêm thành công
}

std::expected<bool, Error> SqlFacultyDao::update(const Faculty& faculty) {
    ValidationResult vr = faculty.validateBasic();
    if (!vr.isValid) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid Faculty data for update: " + vr.getErrorMessagesCombined()});
    }

    // Kiểm tra xem có faculty nào khác (khác ID hiện tại) đang dùng tên mới không
    auto byName = findByName(faculty.getName());
    if (byName.has_value() && byName.value().getId() != faculty.getId()) {
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Another faculty already has the name: " + faculty.getName()});
    }
    // Nếu lỗi không phải là NOT_FOUND, thì có vấn đề khác
    if (!byName.has_value() && byName.error().code != ErrorCode::NOT_FOUND) {
        return std::unexpected(byName.error());
    }


    std::string sql = "UPDATE Faculties SET name = ? WHERE id = ?;";
    auto paramsResult = _parser->toQueryUpdateParams(faculty);
     if (!paramsResult.has_value()) {
        return std::unexpected(paramsResult.error());
    }

    auto execResult = _dbAdapter->executeUpdate(sql, paramsResult.value());
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
         return std::unexpected(Error{ErrorCode::NOT_FOUND, "Faculty with ID " + faculty.getId() + " not found for update, or no changes made."});
    }
    return true;
}

std::expected<bool, Error> SqlFacultyDao::remove(const std::string& id) {
    // Cần kiểm tra xem có Student hoặc Teacher hoặc Course nào đang tham chiếu đến Faculty này không
    // Tạm thời, giả sử đã có cơ chế kiểm tra ở tầng Service hoặc DB dùng ON DELETE RESTRICT (mặc định)
    // Nếu DB dùng ON DELETE SET NULL hoặc CASCADE thì không cần check ở đây.
    // Schema hiện tại dùng ON DELETE SET NULL cho Courses, Students, Teachers.

    std::string sql = "DELETE FROM Faculties WHERE id = ?;";
    std::vector<DbQueryParam> params = {id};

    auto execResult = _dbAdapter->executeUpdate(sql, params);
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) { // Không có dòng nào bị xóa
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Faculty with ID " + id + " not found for removal."});
    }
    return true;
}

std::expected<bool, Error> SqlFacultyDao::exists(const std::string& id) const {
    std::string sql = "SELECT 1 FROM Faculties WHERE id = ? LIMIT 1;";
    std::vector<DbQueryParam> params = {id};
    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }
    return !queryResult.value().empty();
}

std::expected<Faculty, Error> SqlFacultyDao::findByName(const std::string& name) const {
    std::string sql = "SELECT id, name FROM Faculties WHERE name = ?;";
    std::vector<DbQueryParam> params = {name};

    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    const auto& table = queryResult.value();
    if (table.empty()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Faculty with name '" + name + "' not found."});
    }
    return _parser->parse(table[0]);
}