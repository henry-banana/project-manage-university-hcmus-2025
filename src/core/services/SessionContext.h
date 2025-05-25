#ifndef SESSIONCONTEXT_H
#define SESSIONCONTEXT_H

#include <string>
#include <optional>
#include <memory>
#include "../../common/UserRole.h"
#include "../entities/User.h" // Để lưu shared_ptr<User>

class SessionContext {
private:
    std::optional<std::shared_ptr<User>> _currentUser;

public:
    SessionContext() = default;

    void setCurrentUser(std::shared_ptr<User> user);
    void clearCurrentUser();

    bool isAuthenticated() const;
    std::optional<std::string> getCurrentUserId() const;
    std::optional<UserRole> getCurrentUserRole() const;
    std::optional<std::shared_ptr<User>> getCurrentUser() const;
};

#endif // SESSIONCONTEXT_H