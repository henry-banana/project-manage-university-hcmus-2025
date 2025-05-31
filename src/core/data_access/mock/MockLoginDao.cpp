// --- START OF MODIFIED FILE src/core/data_access/mock/MockLoginDao.cpp ---
#include "MockLoginDao.h"
#include "../../../utils/PasswordInput.h"
#include "../../../common/ErrorType.h"
#include <map>
#include <vector>
#include <algorithm>
#include <expected>

namespace {
    std::map<std::string, LoginCredentials> mock_login_credentials_data; 
    bool mock_login_data_initialized_flag = false;

    LoginCredentials createMockCred(const std::string& id, const std::string& plainPass, UserRole role, LoginStatus status) {
        std::string salt = PasswordUtils::generateSalt(); 
        std::string hash = PasswordUtils::hashPassword(plainPass, salt);
        return {id, hash, salt, role, status};
    }
} 

void MockLoginDao::initializeDefaultMockData() {
    if (!mock_login_data_initialized_flag) {
        mock_login_credentials_data.clear();
        mock_login_credentials_data.emplace("admin", createMockCred("admin", "admin123", UserRole::ADMIN, LoginStatus::ACTIVE));
        mock_login_credentials_data.emplace("S001", createMockCred("S001", "alicepass", UserRole::STUDENT, LoginStatus::ACTIVE));
        mock_login_credentials_data.emplace("T001", createMockCred("T001", "teacherpass", UserRole::TEACHER, LoginStatus::ACTIVE));
        mock_login_credentials_data.emplace("S002_Pending", createMockCred("S002_Pending", "pendingpass", UserRole::PENDING_STUDENT, LoginStatus::PENDING_APPROVAL));
        mock_login_credentials_data.emplace("T003_Disabled", createMockCred("T003_Disabled", "filiuspass", UserRole::TEACHER, LoginStatus::DISABLED));
        mock_login_data_initialized_flag = true;
    }
}
void MockLoginDao::clearMockData() {
    mock_login_credentials_data.clear();
    mock_login_data_initialized_flag = false;
}

MockLoginDao::MockLoginDao() {
    // Constructor không tự động init data
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
    auto insert_result = mock_login_credentials_data.emplace(userId, LoginCredentials{userId, passwordHash, salt, role, status});
    if(!insert_result.second){
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to emplace login credentials into mock data."});
    }
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
    auto credsExp = findCredentialsByUserId(userId); // findCredentialsByUserId đã xử lý NOT_FOUND
    if (credsExp.has_value()) {
        return credsExp.value().role;
    }
    // Nếu không tìm thấy user (NOT_FOUND từ findCredentialsByUserId) hoặc lỗi khác
    return std::unexpected(credsExp.error()); 
}

std::expected<LoginStatus, Error> MockLoginDao::getUserStatus(const std::string& userId) const {
    auto credsExp = findCredentialsByUserId(userId);
    if (credsExp.has_value()) {
        return credsExp.value().status;
    }
    return std::unexpected(credsExp.error());
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
    return results; 
}
// --- END OF MODIFIED FILE src/core/data_access/mock/MockLoginDao.cpp ---