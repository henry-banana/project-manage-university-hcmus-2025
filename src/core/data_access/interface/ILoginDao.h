#ifndef ILOGINDAO_H
#define ILOGINDAO_H

#include <string>
#include <vector> // (➕) Cần cho findByStatus
#include <expected> // (➕)
#include "../../../common/UserRole.h"
#include "../../../common/LoginStatus.h"
#include "../../../common/ErrorType.h" // (➕)

// LoginCredentials đã được khai báo trong file header ILoginDao trước đó, giữ nguyên
struct LoginCredentials {
    std::string userId;
    std::string passwordHash;
    std::string salt;
    UserRole role;
    LoginStatus status;
};

class ILoginDao {
public:
    virtual ~ILoginDao() = default;

    virtual std::expected<LoginCredentials, Error> findCredentialsByUserId(const std::string& userId) const = 0;
    virtual std::expected<bool, Error> addUserCredentials(const std::string& userId, const std::string& passwordHash, const std::string& salt, UserRole role, LoginStatus status) = 0;
    virtual std::expected<bool, Error> updatePassword(const std::string& userId, const std::string& newPasswordHash, const std::string& newSalt) = 0;
    virtual std::expected<bool, Error> removeUserCredentials(const std::string& userId) = 0;
    virtual std::expected<UserRole, Error> getUserRole(const std::string& userId) const = 0; // Trả UserRole hoặc Error
    virtual std::expected<LoginStatus, Error> getUserStatus(const std::string& userId) const = 0; // Trả LoginStatus hoặc Error
    virtual std::expected<bool, Error> updateUserRoleAndStatus(const std::string& userId, UserRole newRole, LoginStatus newStatus) = 0;
    virtual std::expected<std::vector<LoginCredentials>, Error> findByStatus(LoginStatus status) const = 0;
};

#endif // ILOGINDAO_H