#include "LoginStatus.h"
// #include <stdexcept> // For std::invalid_argument

/**
 * @namespace LoginStatusUtils
 * @brief Không gian tên chứa các hàm tiện ích xử lý trạng thái đăng nhập
 * 
 * Namespace này cung cấp các hàm tiện ích để chuyển đổi giữa enum LoginStatus
 * và các biểu diễn chuỗi tương ứng.
 */
namespace LoginStatusUtils {

    /**
     * @brief Chuyển đổi trạng thái đăng nhập thành chuỗi
     * 
     * Phương thức này chuyển đổi giá trị enum LoginStatus thành 
     * biểu diễn chuỗi tương ứng.
     * 
     * @param status Trạng thái đăng nhập cần chuyển đổi
     * @return std::string Chuỗi đại diện cho trạng thái đăng nhập
     */
    std::string toString(LoginStatus status) {
        switch (status) {
            case LoginStatus::ACTIVE: return "ACTIVE";
            case LoginStatus::PENDING_APPROVAL: return "PENDING_APPROVAL";
            case LoginStatus::DISABLED: return "DISABLED";
            default: return "UNKNOWN_STATUS";
        }
    }

    /**
     * @brief Chuyển đổi chuỗi thành trạng thái đăng nhập
     * 
     * Phương thức này chuyển đổi biểu diễn chuỗi thành 
     * giá trị enum LoginStatus tương ứng.
     * 
     * @param statusStr Chuỗi đại diện cho trạng thái đăng nhập
     * @return LoginStatus Trạng thái đăng nhập tương ứng
     */
    LoginStatus fromString(const std::string& statusStr) {
        if (statusStr == "ACTIVE") return LoginStatus::ACTIVE;
        if (statusStr == "PENDING_APPROVAL") return LoginStatus::PENDING_APPROVAL;
        if (statusStr == "DISABLED") return LoginStatus::DISABLED;
        throw std::invalid_argument("Invalid LoginStatus string: " + statusStr);
    }

} // namespace LoginStatusUtils