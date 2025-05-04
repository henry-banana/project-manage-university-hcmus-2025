#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include <memory>
#include <string>
#include <optional>
#include "../repo/interface/ILoginRepository.h"

class IAuthService {
public:
    virtual ~IAuthService() = default;

    // Attempts to log in a user. Returns the UserRole on success, nullopt otherwise.
    virtual std::optional<UserRole> login(const std::string& userId, const std::string& password) = 0;

    // Logs out the current user.
    virtual void logout() = 0;

    // Checks if a user is currently authenticated.
    virtual bool isAuthenticated() const = 0;

    // Gets the role of the currently logged-in user, if any.
    virtual std::optional<UserRole> getCurrentUserRole() const = 0;

    // Gets the ID of the currently logged-in user, if any.
    virtual std::optional<std::string> getCurrentUserId() const = 0;

    // Optional: Change password for the currently logged-in user
    virtual bool changePassword(const std::string& oldPassword, const std::string& newPassword) = 0;

    // Optional: Add a new user (might be better in AdminService, but related to login)
    // Requires hashing the password before storing.
    virtual bool registerUser(const std::string& userId, const std::string& plainPassword, UserRole role) = 0;
};

class AuthService : public IAuthService {
private:
    std::shared_ptr<ILoginRepository> _loginRepo;
    std::optional<std::string> _currentUserId;
    std::optional<UserRole> _currentUserRole;

public:
    explicit AuthService(std::shared_ptr<ILoginRepository> loginRepo);

    std::optional<UserRole> login(const std::string& userId, const std::string& password) override;

    void logout() override;

    bool isAuthenticated() const override;

    std::optional<UserRole> getCurrentUserRole() const override;

    std::optional<std::string> getCurrentUserId() const override;

    bool changePassword(const std::string& oldPassword, const std::string& newPassword) override;

    bool registerUser(const std::string& userId, const std::string& plainPassword, UserRole role) override;

};
#endif // AUTH_SERVICE_H