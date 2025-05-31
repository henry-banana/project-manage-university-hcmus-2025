#include "SessionContext.h"

void SessionContext::setCurrentUser(std::shared_ptr<User> user) {
    if (user == nullptr) { // (➕) Thêm kiểm tra này
        clearCurrentUser();
    } else {
        _currentUser = std::move(user);
    }
}

void SessionContext::clearCurrentUser() {
    _currentUser.reset(); // Hoặc _currentUser = std::nullopt;
}

bool SessionContext::isAuthenticated() const {
    // (➕) Sửa lại isAuthenticated để kiểm tra cả con trỏ bên trong
    return _currentUser.has_value() && _currentUser.value() != nullptr;
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
    // (➕) Nếu _currentUser chứa nullptr, thì nên trả về std::nullopt
    if (_currentUser.has_value() && _currentUser.value() != nullptr) {
        return _currentUser;
    }
    return std::nullopt;
}