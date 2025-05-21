#include "MockLoginDao.h"
#include "../../../utils/PasswordInput.h"
#include "../../../common/ErrorType.h"
#include <map>
#include <vector>
#include <algorithm>
#include <expected>

// Namespace ẩn danh và initializeMockLoginDataIfNeeded giữ nguyên như trước
namespace {
    std::map<std::string, LoginCredentials> mock_login_credentials_data; // Key là userId
    bool mock_login_data_initialized = false;

    LoginCredentials createMockCred(const std::string& id, const std::string& plainPass, UserRole role, LoginStatus status) {
        std::string salt = PasswordUtils::generateSalt(); // PasswordUtils đã có
        std::string hash = PasswordUtils::hashPassword(plainPass, salt);
        return {id, hash, salt, role, status};
    }

    void initializeMockLoginDataIfNeeded() {
        if (!mock_login_data_initialized) {
            mock_login_credentials_data.emplace("admin", createMockCred("admin", "admin123", UserRole::ADMIN, LoginStatus::ACTIVE));
            mock_login_credentials_data.emplace("S001", createMockCred("S001", "alicepass", UserRole::STUDENT, LoginStatus::ACTIVE));
            mock_login_credentials_data.emplace("S002", createMockCred("S002", "bobpass", UserRole::STUDENT, LoginStatus::PENDING_APPROVAL));
            mock_login_credentials_data.emplace("S003", createMockCred("S003", "charliepass", UserRole::STUDENT, LoginStatus::ACTIVE));
            mock_login_credentials_data.emplace("S004", createMockCred("S004", "dianapass", UserRole::STUDENT, LoginStatus::DISABLED));
            mock_login_credentials_data.emplace("T001", createMockCred("T001", "snapepass", UserRole::TEACHER, LoginStatus::ACTIVE));
            mock_login_credentials_data.emplace("T002", createMockCred("T002", "minervapass", UserRole::TEACHER, LoginStatus::ACTIVE));
            mock_login_credentials_data.emplace("T003", createMockCred("T003", "filiuspass", UserRole::TEACHER, LoginStatus::DISABLED));
            mock_login_data_initialized = true;
        }
    }
} // namespace ẩn danh

MockLoginDao::MockLoginDao() {
    initializeMockLoginDataIfNeeded();
}

std::expected<LoginCredentials, Error> MockLoginDao::findCredentialsByUserId(const std::string& userId) const {
    auto it = mock_login_credentials_data.find(userId);
    if (it != mock_login_credentials_data.end()) {
        return it->second;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock LoginCredentials for User ID " + userId + " not found"});
}

std::expected<bool, Error> MockLoginDao::addUserCredentials(const std::string& userId, const std::string& passwordHash, const std::string& salt, UserRole role, LoginStatus status) {
    if (mock_login_credentials_data.count(userId)) {
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Mock User ID " + userId + " already exists in login data."});
    }
    mock_login_credentials_data[userId] = {userId, passwordHash, salt, role, status};
    return true;
}

std::expected<bool, Error> MockLoginDao::updatePassword(const std::string& userId, const std::string& newPasswordHash, const std::string& newSalt) {
    auto it = mock_login_credentials_data.find(userId);
    if (it != mock_login_credentials_data.end()) {
        it->second.passwordHash = newPasswordHash;
        it->second.salt = newSalt;
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock User ID " + userId + " not found for password update."});
}

std::expected<bool, Error> MockLoginDao::removeUserCredentials(const std::string& userId) {
    if (mock_login_credentials_data.erase(userId) > 0) {
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock User ID " + userId + " not found for removal from login data."});
}

std::expected<UserRole, Error> MockLoginDao::getUserRole(const std::string& userId) const {
    auto creds = findCredentialsByUserId(userId);
    if (creds.has_value()) {
        return creds.value().role;
    }
    return std::unexpected(creds.error()); // Trả về lỗi từ findCredentialsByUserId
}

std::expected<LoginStatus, Error> MockLoginDao::getUserStatus(const std::string& userId) const {
    auto creds = findCredentialsByUserId(userId);
    if (creds.has_value()) {
        return creds.value().status;
    }
    return std::unexpected(creds.error());
}

std::expected<bool, Error> MockLoginDao::updateUserRoleAndStatus(const std::string& userId, UserRole newRole, LoginStatus newStatus) {
    auto it = mock_login_credentials_data.find(userId);
    if (it != mock_login_credentials_data.end()) {
        it->second.role = newRole;
        it->second.status = newStatus;
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock User ID " + userId + " not found for role/status update."});
}

std::expected<std::vector<LoginCredentials>, Error> MockLoginDao::findByStatus(LoginStatus status) const {
    std::vector<LoginCredentials> results;
    for(const auto& pair : mock_login_credentials_data) {
        if (pair.second.status == status) {
            results.push_back(pair.second);
        }
    }
    return results; // Trả về vector, có thể rỗng
}