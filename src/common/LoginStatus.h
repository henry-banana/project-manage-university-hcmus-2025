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
    inline std::string toString(LoginStatus status) {
        switch (status) {
            case LoginStatus::ACTIVE: return "ACTIVE";
            case LoginStatus::PENDING_APPROVAL: return "PENDING_APPROVAL";
            case LoginStatus::DISABLED: return "DISABLED";
            default: return "UNKNOWN_STATUS";
        }
    }

    inline LoginStatus fromString(const std::string& statusStr) {
        if (statusStr == "ACTIVE") return LoginStatus::ACTIVE;
        if (statusStr == "PENDING_APPROVAL") return LoginStatus::PENDING_APPROVAL;
        if (statusStr == "DISABLED") return LoginStatus::DISABLED;
        // Mặc định hoặc throw lỗi nếu chuỗi không hợp lệ
        return LoginStatus::DISABLED; // Hoặc throw std::invalid_argument
    }
}

#endif // LOGIN_STATUS_H