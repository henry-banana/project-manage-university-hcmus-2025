#ifndef SESSIONCONTEXT_H
#define SESSIONCONTEXT_H

/**
 * @file SessionContext.h
 * @brief Định nghĩa lớp quản lý phiên làm việc người dùng
 */

#include <string>
#include <optional>
#include <memory>
#include "../../common/UserRole.h"
#include "../entities/User.h" // Để lưu shared_ptr<User>

/**
 * @class SessionContext
 * @brief Lớp quản lý thông tin phiên làm việc hiện tại của người dùng
 * 
 * Lớp này lưu trữ và quản lý thông tin về người dùng đã đăng nhập hiện tại,
 * cho phép các thành phần khác trong hệ thống kiểm tra trạng thái xác thực
 * và vai trò của người dùng.
 */
class SessionContext {
private:
    std::optional<std::shared_ptr<User>> _currentUser; ///< Thông tin người dùng hiện tại, nếu đã đăng nhập

public:
    /**
     * @brief Hàm khởi tạo mặc định
     */
    SessionContext() = default;

    /**
     * @brief Đặt người dùng hiện tại cho phiên làm việc
     * @param user Con trỏ đến đối tượng User đã đăng nhập
     */
    void setCurrentUser(std::shared_ptr<User> user);
    
    /**
     * @brief Xóa thông tin người dùng hiện tại (đăng xuất)
     */
    void clearCurrentUser();

    /**
     * @brief Kiểm tra người dùng đã được xác thực hay chưa
     * @return true nếu đã xác thực, false nếu chưa
     */
    virtual bool isAuthenticated() const;
    
    /**
     * @brief Lấy ID của người dùng hiện tại
     * @return ID người dùng nếu đã đăng nhập, hoặc std::nullopt nếu chưa
     */
    virtual std::optional<std::string> getCurrentUserId() const;
    
    /**
     * @brief Lấy vai trò của người dùng hiện tại
     * @return Vai trò người dùng nếu đã đăng nhập, hoặc std::nullopt nếu chưa
     */
    virtual std::optional<UserRole> getCurrentUserRole() const;
    
    /**
     * @brief Lấy thông tin đầy đủ của người dùng hiện tại
     * @return Con trỏ đến đối tượng User nếu đã đăng nhập, hoặc std::nullopt nếu chưa
     */
    virtual std::optional<std::shared_ptr<User>> getCurrentUser() const;
};

#endif // SESSIONCONTEXT_H