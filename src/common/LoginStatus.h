#ifndef LOGIN_STATUS_H
#define LOGIN_STATUS_H

#include <string> // Cho hàm toString helper

enum class LoginStatus {
    ACTIVE,
    PENDING_APPROVAL,
    DISABLED // Tài khoản bị vô hiệu hóa
};

// Helper function (có thể đặt trong namespace hoặc global tùy thích)
namespace LoginStatusUtils {
    std::string toString(LoginStatus status);
    LoginStatus fromString(const std::string& statusStr);
}

#endif // LOGIN_STATUS_H