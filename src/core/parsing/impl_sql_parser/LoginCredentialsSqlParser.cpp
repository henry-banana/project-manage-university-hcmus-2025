#include "LoginCredentialsSqlParser.h"
#include "SqlParserUtils.h"

std::expected<LoginCredentials, Error> LoginCredentialsSqlParser::parse(const DbQueryResultRow& row) const {
    try {
        LoginCredentials creds;
        creds.userId = SqlParserUtils::getOptional<std::string>(row, "userId");
        if (creds.userId.empty()) {
            return std::unexpected(Error{ErrorCode::PARSING_ERROR, "LoginCredentials userId not found or empty in SQL row."});
        }
        creds.passwordHash = SqlParserUtils::getOptional<std::string>(row, "passwordHash");
        creds.salt = SqlParserUtils::getOptional<std::string>(row, "salt");
        
        // Role và Status thường lấy từ bảng Users khi JOIN
        // Nếu query chỉ từ bảng Logins thì role và status sẽ không có ở đây
        // SqlLoginDao sẽ chịu trách nhiệm query Users nếu cần role/status
        auto role_it = row.find("role");
        if (role_it != row.end() && role_it->second.has_value()) {
             creds.role = static_cast<UserRole>(std::any_cast<long long>(role_it->second));
        } else {
            creds.role = UserRole::UNKNOWN; // Mặc định nếu không có thông tin role
        }

        auto status_it = row.find("status");
         if (status_it != row.end() && status_it->second.has_value()) {
            creds.status = static_cast<LoginStatus>(std::any_cast<long long>(status_it->second));
        } else {
            // Giả định nếu query chỉ từ bảng Logins thì không có status, hoặc DAO sẽ set mặc định
             creds.status = LoginStatus::DISABLED; // Mặc định nếu không có
        }
        return creds;

    } catch (const std::bad_any_cast& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Failed to parse LoginCredentials from SQL row: " + std::string(e.what())});
    } catch (const std::exception& e) {
        return std::unexpected(Error{ErrorCode::PARSING_ERROR, "Generic error parsing LoginCredentials from SQL row: " + std::string(e.what())});
    }
}

std::expected<DbQueryResultRow, Error> LoginCredentialsSqlParser::serialize(const LoginCredentials& creds) const {
    DbQueryResultRow row;
    row.emplace("userId", creds.userId);
    row.emplace("passwordHash", creds.passwordHash);
    row.emplace("salt", creds.salt);
    return row;
}

std::expected<std::vector<std::any>, Error> LoginCredentialsSqlParser::toQueryInsertParams(const LoginCredentials& creds) const {
    std::vector<std::any> params;
    params.push_back(creds.userId);
    params.push_back(creds.passwordHash);
    params.push_back(creds.salt);
    // Role và status được insert vào bảng Users, không phải bảng Logins
    return params;
}

// Dùng cho việc update password trong bảng Logins
std::expected<std::vector<std::any>, Error> LoginCredentialsSqlParser::toQueryUpdateParams(const LoginCredentials& creds) const {
    std::vector<std::any> params;
    params.push_back(creds.passwordHash); // newPasswordHash
    params.push_back(creds.salt);         // newSalt
    params.push_back(creds.userId);       // for WHERE clause
    return params;
}