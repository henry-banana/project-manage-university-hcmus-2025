#include "SqlLoginDao.h"
#include <vector>
#include <stdexcept>

SqlLoginDao::SqlLoginDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                         std::shared_ptr<IEntityParser<LoginCredentials, DbQueryResultRow>> parser)
    : _dbAdapter(std::move(dbAdapter)), _parser(std::move(parser)) {
    if (!_dbAdapter) {
        throw std::invalid_argument("Database adapter cannot be null for SqlLoginDao.");
    }
    if (!_parser) {
        throw std::invalid_argument("LoginCredentials parser cannot be null for SqlLoginDao.");
    }
}

std::expected<LoginCredentials, Error> SqlLoginDao::findCredentialsByUserId(const std::string& userId) const {
    // Lấy thông tin từ cả Logins và Users để có role và status
    std::string sql = "SELECT L.userId, L.passwordHash, L.salt, U.role, U.status "
                      "FROM Logins L JOIN Users U ON L.userId = U.id "
                      "WHERE L.userId = ?;";
    std::vector<DbQueryParam> params = {userId};

    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    const auto& table = queryResult.value();
    if (table.empty()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Login credentials for User ID " + userId + " not found."});
    }
    return _parser->parse(table[0]);
}

// addUserCredentials sẽ được gọi sau khi User đã được tạo trong bảng Users
// Nó chỉ thêm thông tin vào bảng Logins.
std::expected<bool, Error> SqlLoginDao::addUserCredentials(const std::string& userId, const std::string& passwordHash, const std::string& salt, UserRole role, LoginStatus status) {
    // role và status không được dùng trực tiếp ở đây vì chúng thuộc bảng Users
    // Hàm này chỉ tập trung vào bảng Logins. Việc tạo User đã xảy ra trước đó.
    (void)role; // Đánh dấu là không dùng
    (void)status; // Đánh dấu là không dùng

    std::string sql = "INSERT INTO Logins (userId, passwordHash, salt) VALUES (?, ?, ?);";
    LoginCredentials creds = {userId, passwordHash, salt, UserRole::UNKNOWN, LoginStatus::DISABLED}; // Role, Status tạm thời không quan trọng cho parser này
    auto paramsResult = _parser->toQueryInsertParams(creds);

    if (!paramsResult.has_value()) {
         return std::unexpected(paramsResult.error());
    }

    auto execResult = _dbAdapter->executeUpdate(sql, paramsResult.value());
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to add user credentials, no rows affected."});
    }
    return true;
}

std::expected<bool, Error> SqlLoginDao::updatePassword(const std::string& userId, const std::string& newPasswordHash, const std::string& newSalt) {
    std::string sql = "UPDATE Logins SET passwordHash = ?, salt = ? WHERE userId = ?;";
    LoginCredentials creds = {userId, newPasswordHash, newSalt, UserRole::UNKNOWN, LoginStatus::DISABLED};
    auto paramsResult = _parser->toQueryUpdateParams(creds); // Parser này sẽ lấy hash, salt, userId

    if (!paramsResult.has_value()) {
         return std::unexpected(paramsResult.error());
    }
    
    auto execResult = _dbAdapter->executeUpdate(sql, paramsResult.value());
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "User ID " + userId + " not found for password update."});
    }
    return true;
}

std::expected<bool, Error> SqlLoginDao::removeUserCredentials(const std::string& userId) {
    // Xóa User sẽ tự động cascade xóa Login theo schema
    // Tuy nhiên, nếu muốn xóa riêng Login mà không xóa User (ít xảy ra), thì dùng hàm này.
    std::string sql = "DELETE FROM Logins WHERE userId = ?;";
    std::vector<DbQueryParam> params = {userId};

    auto execResult = _dbAdapter->executeUpdate(sql, params);
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    // Việc không tìm thấy để xóa không nhất thiết là lỗi ở đây, có thể User đó chưa có credentials
    // if (execResult.value() == 0) {
    //     return std::unexpected(Error{ErrorCode::NOT_FOUND, "User ID " + userId + " not found in Logins for removal."});
    // }
    return true;
}

std::expected<UserRole, Error> SqlLoginDao::getUserRole(const std::string& userId) const {
    std::string sql = "SELECT role FROM Users WHERE id = ?;";
    std::vector<DbQueryParam> params = {userId};
    auto queryResult = _dbAdapter->executeQuery(sql, params);

    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }
    const auto& table = queryResult.value();
    if (table.empty()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "User ID " + userId + " not found to get role."});
    }
    try {
        long long role_val = std::any_cast<long long>(table[0].at("role"));
        return static_cast<UserRole>(role_val);
    } catch (const std::bad_any_cast& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse UserRole: " + std::string(e.what())});
    } catch (const std::out_of_range& e) {
         return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Column 'role' not found in query result."});
    }
}

std::expected<LoginStatus, Error> SqlLoginDao::getUserStatus(const std::string& userId) const {
    std::string sql = "SELECT status FROM Users WHERE id = ?;";
    std::vector<DbQueryParam> params = {userId};
    auto queryResult = _dbAdapter->executeQuery(sql, params);

    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }
    const auto& table = queryResult.value();
    if (table.empty()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "User ID " + userId + " not found to get status."});
    }
     try {
        long long status_val = std::any_cast<long long>(table[0].at("status"));
        return static_cast<LoginStatus>(status_val);
    } catch (const std::bad_any_cast& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse LoginStatus: " + std::string(e.what())});
    } catch (const std::out_of_range& e) {
         return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Column 'status' not found in query result."});
    }
}

std::expected<bool, Error> SqlLoginDao::updateUserRoleAndStatus(const std::string& userId, UserRole newRole, LoginStatus newStatus) {
    std::string sql = "UPDATE Users SET role = ?, status = ? WHERE id = ?;";
    std::vector<DbQueryParam> params = {static_cast<int>(newRole), static_cast<int>(newStatus), userId};
    
    auto execResult = _dbAdapter->executeUpdate(sql, params);
    if (!execResult.has_value()) {
        return std::unexpected(execResult.error());
    }
    if (execResult.value() == 0) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "User ID " + userId + " not found for role/status update."});
    }
    return true;
}

std::expected<std::vector<LoginCredentials>, Error> SqlLoginDao::findByStatus(LoginStatus status) const {
    std::string sql = "SELECT L.userId, L.passwordHash, L.salt, U.role, U.status "
                      "FROM Logins L JOIN Users U ON L.userId = U.id "
                      "WHERE U.status = ?;";
    std::vector<DbQueryParam> params = {static_cast<int>(status)};

    auto queryResult = _dbAdapter->executeQuery(sql, params);
    if (!queryResult.has_value()) {
        return std::unexpected(queryResult.error());
    }

    std::vector<LoginCredentials> credentialsList;
    for (const auto& row : queryResult.value()) {
        auto parseResult = _parser->parse(row);
        if (parseResult.has_value()) {
            credentialsList.push_back(parseResult.value());
        } else {
             return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse one or more LoginCredentials by status."});
        }
    }
    return credentialsList;
}