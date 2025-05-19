#include "LoginStatus.h"
// #include <stdexcept> // For std::invalid_argument

namespace LoginStatusUtils {
    std::string toString(LoginStatus status) {
        switch (status) {
            case LoginStatus::ACTIVE: return "ACTIVE";
            case LoginStatus::PENDING_APPROVAL: return "PENDING_APPROVAL";
            case LoginStatus::DISABLED: return "DISABLED";
            default: return "UNKNOWN_STATUS";
        }
    }

    LoginStatus fromString(const std::string& statusStr) {
        if (statusStr == "ACTIVE") return LoginStatus::ACTIVE;
        if (statusStr == "PENDING_APPROVAL") return LoginStatus::PENDING_APPROVAL;
        if (statusStr == "DISABLED") return LoginStatus::DISABLED;
        // Hoặc ghi log và trả về một giá trị mặc định an toàn
        // Hoặc throw std::invalid_argument("Invalid LoginStatus string: " + statusStr);
        return LoginStatus::DISABLED; // Default fallback
    }
}