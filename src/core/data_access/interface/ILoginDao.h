#ifndef ILOGINDAO_H
#define ILOGINDAO_H

#include <string>
#include <optional>
#include <vector> // Thêm để dùng cho findByStatus
#include "../../../common/UserRole.h"
#include "../../../common/LoginStatus.h"
#include "../../../common/OperationResult.h"

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
    virtual OperationResult<LoginCredentials> findCredentialsByUserId(const std::string& userId) const = 0;
    virtual OperationResult<bool> addUserCredentials(const std::string& userId, const std::string& passwordHash, const std::string& salt, UserRole role, LoginStatus status) = 0;
    virtual OperationResult<bool> updatePassword(const std::string& userId, const std::string& newPasswordHash, const std::string& newSalt) = 0;
    virtual OperationResult<bool> removeUserCredentials(const std::string& userId) = 0;
    virtual OperationResult<UserRole> getUserRole(const std::string& userId) const = 0;
    virtual OperationResult<LoginStatus> getUserStatus(const std::string& userId) const = 0;
    virtual OperationResult<bool> updateUserRoleAndStatus(const std::string& userId, UserRole newRole, LoginStatus newStatus) = 0;
    virtual OperationResult<std::vector<LoginCredentials>> findByStatus(LoginStatus status) const = 0;
};

#endif // ILOGINDAO_H