#ifndef LOGIN_STATUS_H
#define LOGIN_STATUS_H

#include <string> // Cho hàm toString helper
#include <stdexcept> // Cho std::invalid_argument

/**
 * @enum LoginStatus
 * @brief Liệt kê các trạng thái đăng nhập của tài khoản
 * 
 * Enum này định nghĩa các trạng thái có thể có của một tài khoản đăng nhập
 * trong hệ thống, bao gồm hoạt động, chờ phê duyệt, và vô hiệu hóa.
 */
enum class LoginStatus {
    ACTIVE,           ///< Tài khoản đang hoạt động bình thường
    PENDING_APPROVAL, ///< Tài khoản đang chờ được phê duyệt
    DISABLED          ///< Tài khoản đã bị vô hiệu hóa
};

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
     * @param status Trạng thái đăng nhập cần chuyển đổi
     * @return std::string Chuỗi đại diện cho trạng thái đăng nhập
     */
    std::string toString(LoginStatus status);
    
    /**
     * @brief Chuyển đổi chuỗi thành trạng thái đăng nhập
     * @param statusStr Chuỗi đại diện cho trạng thái đăng nhập
     * @return LoginStatus Trạng thái đăng nhập tương ứng
     */
    LoginStatus fromString(const std::string& statusStr);
}

#endif // LOGIN_STATUS_H