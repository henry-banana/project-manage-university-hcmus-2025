#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H

#include <memory>
#include <string>
#include <optional>
#include "../repo/interface/ILoginRepository.h"

class IAuthService {
public:
    virtual ~IAuthService() = default;
    
    // Login operations
    virtual std::optional<UserType> login(const std::string& username, const std::string& password) = 0;
    virtual bool logout() = 0;
    virtual bool isAuthenticated() const = 0;
    virtual std::optional<UserType> getCurrentUserType() const = 0;
    virtual std::optional<std::string> getCurrentUserId() const = 0;
};

class AuthService : public IAuthService {
private:
    std::shared_ptr<ILoginRepository> _loginRepo;
    bool _isAuthenticated;
    std::optional<UserType> _currentUserType;
    std::optional<std::string> _currentUserId;
    
public:
    explicit AuthService(std::shared_ptr<ILoginRepository> loginRepo);
    
    // Login operations
    std::optional<UserType> login(const std::string& username, const std::string& password) override;
    bool logout() override;
    bool isAuthenticated() const override;
    std::optional<UserType> getCurrentUserType() const override;
    std::optional<std::string> getCurrentUserId() const override;
};

#endif // AUTH_SERVICE_H