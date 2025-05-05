#include "AuthService.h"
#include "../repo/interface/ILoginRepository.h"
#include "../../utils/PasswordInput.h"
#include "../../utils/Logger.h"
#include <memory> // For std::shared_ptr
#include <iostream>

AuthService::AuthService(std::shared_ptr<ILoginRepository> loginRepo)
    : _loginRepo(loginRepo), _currentUserId(std::nullopt), _currentUserRole(std::nullopt)
{
    if (!_loginRepo) {
        throw std::invalid_argument("LoginRepository cannot be null.");
    }
}

std::optional<UserRole> AuthService::login(const std::string& userId, const std::string& password) {
    logout(); // Ensure previous user is logged out
    auto roleOpt = _loginRepo->validateCredentials(userId, password);
    if (roleOpt) {
        _currentUserId = userId;
        _currentUserRole = roleOpt;
        LOG_INFO("User logged in successfully: " + userId);
    } else {
        LOG_WARN("Login failed for user: " + userId);
        std::cerr << "Invalid User ID or Password.\n";
    }
    return roleOpt;
}

void AuthService::logout() {
        if (_currentUserId) {
            LOG_INFO("User logged out: " + _currentUserId.value());
        }
    _currentUserId = std::nullopt;
    _currentUserRole = std::nullopt;
}

bool AuthService::isAuthenticated() const {
    return _currentUserId.has_value();
}

std::optional<UserRole> AuthService::getCurrentUserRole() const {
    return _currentUserRole;
}

std::optional<std::string> AuthService::getCurrentUserId() const {
    return _currentUserId;
}

bool AuthService::changePassword(const std::string& oldPassword, const std::string& newPassword) {
    if (!isAuthenticated()) {
        LOG_WARN("Change password attempt failed: No user logged in.");
        std::cerr << "No user is currently logged in.\n";
        return false;
    }
    std::string userId = _currentUserId.value();

    // 1. Validate old password
    auto credsOpt = _loginRepo->findCredentialsById(userId);
    if (!credsOpt || !verifyPassword(oldPassword, credsOpt.value().passwordHash, credsOpt.value().salt)) {
            LOG_WARN("Change password failed: Old password validation failed for user: " + userId);
            std::cerr << "Old password is incorrect.\n";
            return false;
    }

        // 2. Validate new password complexity (optional, add rules in PasswordUtils)
        if (!isPasswordComplexEnough(newPassword)) {
        LOG_WARN("Change password failed: New password does not meet complexity requirements for user: " + userId);
        std::cerr << "New password must be at least 8 characters long, contain uppercase and lowercase letters, numbers, and special characters.\n";
        // Maybe return a specific error code or throw an exception
        return false;
        }

    // 3. Hash new password
    std::string newSalt = generateSalt();
    std::string newHash = hashPassword(newPassword, newSalt);

    // 4. Update in repository
    if (_loginRepo->updatePassword(userId, newHash, newSalt)) {
            LOG_INFO("Password changed successfully for user: " + userId);
            return true;
    } else {
            LOG_ERROR("Failed to update password in repository for user: " + userId);
            std::cerr << "Failed to update password in repository.\n";
            return false;
    }
}

bool AuthService::registerUser(const std::string& userId, const std::string& plainPassword, UserRole role) {
        // Add validation for userId format/existence in other repos if needed
        if (_loginRepo->findCredentialsById(userId)) {
            LOG_WARN("Registration failed: User ID already exists: " + userId);
            std::cerr << "User ID already exists. Please choose a different one.\n";
            return false;
        }

        // Validate password complexity
        if (!isPasswordComplexEnough(plainPassword)) {
            LOG_WARN("Registration failed: Password does not meet complexity requirements for user: " + userId);
            std::cerr << "Password must be at least 8 characters long, contain uppercase and lowercase letters, numbers, and special characters.\n";
            return false;
        }


        // Hash password
        std::string salt = generateSalt();
        std::string hash = hashPassword(plainPassword, salt);

        // Add credentials
        if (_loginRepo->addUserCredentials(userId, hash, salt, role)) {
            LOG_INFO("User registered successfully: " + userId);
            return true;
        } else {
            LOG_ERROR("Failed to add user credentials to repository for user: " + userId);
            std::cerr << "Failed to register user. Please try again.\n";
            return false;
        }
}