/**
 * @file AdminUser.h
 * @brief Định nghĩa lớp AdminUser đại diện cho quản trị viên trong hệ thống
 * 
 * File này định nghĩa lớp AdminUser, kế thừa từ lớp User, đại diện cho thông tin 
 * quản trị viên trong hệ thống quản lý trường đại học. Lớp này bổ sung các phương thức
 * đặc trưng cho quản trị viên.
 */
// src/core/entities/AdminUser.h
#ifndef ADMINUSER_H
#define ADMINUSER_H

#include "User.h" // Kế thừa từ User

/**
 * @class AdminUser
 * @brief Lớp đại diện cho quản trị viên trong hệ thống
 * 
 * Lớp này kế thừa từ lớp User và bổ sung các phương thức
 * đặc trưng cho quản trị viên hệ thống.
 */
class AdminUser : public User {
public:
    /**
     * @brief Constructor tạo đối tượng AdminUser
     * 
     * @param id Mã quản trị viên
     * @param firstName Tên của quản trị viên
     * @param lastName Họ của quản trị viên
     * @param status Trạng thái tài khoản (mặc định là ACTIVE)
     */
    AdminUser(const std::string& id,
              const std::string& firstName,
              const std::string& lastName,
              LoginStatus status = LoginStatus::ACTIVE); // Role sẽ tự đặt là ADMIN

    /**
     * @brief Tạo chuỗi hiển thị thông tin của quản trị viên
     * @return Chuỗi chứa thông tin của quản trị viên
     */
    std::string display() const override;
    
    /**
     * @brief Kiểm tra tính hợp lệ của các thuộc tính cơ bản
     * @return Kết quả kiểm tra tính hợp lệ
     */
    ValidationResult validateBasic() const override;
    
    // Thêm các phương thức hoặc thuộc tính đặc thù cho AdminUser nếu có
    // ví dụ: std::string getAdminLevel() const; 
};

#endif // ADMINUSER_H