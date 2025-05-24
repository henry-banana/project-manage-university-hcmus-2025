// src/core/services/impl/SessionContext.cpp
#include "SessionContext.h"

void SessionContext::setCurrentUser(std::shared_ptr<User> user) {
    _currentUser = std::move(user);
}

void SessionContext::clearCurrentUser() {
    _currentUser.reset();
}

bool SessionContext::isAuthenticated() const {
    return _currentUser.has_value();
}

std::optional<std::string> SessionContext::getCurrentUserId() const {
    if (_currentUser.has_value() && _currentUser.value() != nullptr) {
        return _currentUser.value()->getId();
    }
    return std::nullopt;
}

std::optional<UserRole> SessionContext::getCurrentUserRole() const {
    if (_currentUser.has_value() && _currentUser.value() != nullptr) {
        return _currentUser.value()->getRole();
    }
    return std::nullopt;
}

std::optional<std::shared_ptr<User>> SessionContext::getCurrentUser() const {
    return _currentUser;
}