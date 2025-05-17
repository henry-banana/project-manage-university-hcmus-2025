#ifndef ILOGINDAO_H
#define ILOGINDAO_H

#include <string>
#include <vector>
#include <optional> // Hoặc OperationResult
#include "../../../common/UserRole.h"
#include "../../../common/LoginStatus.h"
#include "../../../common/OperationResult.h"

struct LoginCredentials { // (➕) Struct để nhóm thông tin login
    std::string userId;
    std::string passwordHash;
    std::string salt;
    UserRole role;
    LoginStatus status;
};

class ILoginDao {
public:
    virtual ~ILoginDao() = default;

    virtual OperationResult<LoginCredentials> findCredentialsByUserId(const std::string& userId) const = 0;
    // userId, passwordHash, salt, role, status
    virtual OperationResult<bool> addUserCredentials(const std::string& userId, const std::string& passwordHash, const std::string& salt, UserRole role, LoginStatus status) = 0;
    virtual OperationResult<bool> updatePassword(const std::string& userId, const std::string& newPasswordHash, const std::string& newSalt) = 0;
    virtual OperationResult<bool> removeUserCredentials(const std::string& userId) = 0;
    virtual OperationResult<UserRole> getUserRole(const std::string& userId) const = 0;
    virtual OperationResult<LoginStatus> getUserStatus(const std::string& userId) const = 0;
    virtual OperationResult<bool> updateUserRoleAndStatus(const std::string& userId, UserRole newRole, LoginStatus newStatus) = 0; // (➕) Cho admin duyệt
    virtual OperationResult<std::vector<LoginCredentials>> findByStatus(LoginStatus status) const = 0; // (➕) Để Admin lấy danh sách PENDING
};

#endif // ILOGINDAO_H