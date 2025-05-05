#ifndef ILOGINREPOSITORY_H
#define ILOGINREPOSITORY_H

#include <optional>
#include <string>
#include "../../entities/User.h"
// Interface cho Login Repository (quản lý authentication)
class ILoginRepository {
private:

protected:

public:
    virtual ~ILoginRepository() = default;

    virtual std::optional<UserRole> validateCredentials(const std::string& userId, const std::string& plainPassword) const = 0;

    // Get user details needed for validation (hash, salt, role)
    struct UserCredentials {
        std::string userId;
        std::string passwordHash;
        std::string salt;
        UserRole role;
    };

    virtual std::optional<UserCredentials> findCredentialsById(const std::string& userId) const = 0;

    // Add new user credentials (requires hashed password and salt)
    virtual bool addUserCredentials(const std::string& userId, const std::string& passwordHash, const std::string& salt, UserRole userRole) = 0;

    // Update password (requires new hash and salt)
    virtual bool updatePassword(const std::string& userId, const std::string& newPasswordHash, const std::string& newSalt) = 0;

    // Remove user credentials
    virtual bool removeUserCredentials(const std::string& userId) = 0;

    // Get user role (if needed separately)
    virtual std::optional<UserRole> getUserRole(const std::string& userId) const = 0;
};

#endif // ILOGINREPOSITORY_H