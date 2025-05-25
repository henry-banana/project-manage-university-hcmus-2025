/**
 * @file IAuthService.h
 * @brief Định nghĩa giao diện dịch vụ xác thực người dùng
 * 
 * File này định nghĩa giao diện IAuthService, cung cấp các phương thức
 * để quản lý xác thực người dùng trong hệ thống quản lý trường đại học,
 * bao gồm đăng nhập, đăng xuất, đăng ký và kiểm tra quyền.
 */
#ifndef IAUTHSERVICE_H
#define IAUTHSERVICE_H

#include <string>
#include <optional>
#include <memory>
#include <expected> // (➕) Thay thế OperationResult
#include "../../../common/UserRole.h"
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/User.h"

/**
 * @struct StudentRegistrationData
 * @brief Cấu trúc chứa dữ liệu đăng ký sinh viên mới
 * 
 * Cấu trúc này được sử dụng để truyền thông tin đăng ký của sinh viên mới
 * cho dịch vụ xác thực để tạo tài khoản sinh viên.
 */
struct StudentRegistrationData {
    std::string email; ///< Địa chỉ email
    std::string firstName; ///< Tên
    std::string lastName; ///< Họ
    std::string facultyId; ///< Mã khoa
    int birthDay, birthMonth, birthYear; ///< Ngày, tháng, năm sinh
    std::string address; ///< Địa chỉ
    std::string citizenId; ///< Số CCCD
    std::string phoneNumber; ///< Số điện thoại
};

/**
 * @class IAuthService
 * @brief Giao diện dịch vụ xác thực người dùng
 * 
 * Giao diện này định nghĩa các phương thức để quản lý xác thực người dùng
 * trong hệ thống, bao gồm đăng nhập, đăng xuất, đăng ký và kiểm tra quyền.
 */
class IAuthService {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IAuthService() = default;
    
    /**
     * @brief Đăng nhập vào hệ thống
     * 
     * @param userIdOrEmail ID hoặc email của người dùng
     * @param password Mật khẩu
     * @return Đối tượng User nếu đăng nhập thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::shared_ptr<User>, Error> login(const std::string& userIdOrEmail, const std::string& password) = 0;
    
    /**
     * @brief Đăng xuất khỏi hệ thống
     */
    virtual void logout() = 0;
    
    /**
     * @brief Kiểm tra người dùng đã đăng nhập chưa
     * @return true nếu đã đăng nhập, false nếu chưa
     */
    virtual bool isAuthenticated() const = 0;
    
    /**
     * @brief Lấy ID của người dùng hiện tại
     * @return ID của người dùng nếu đã đăng nhập, hoặc std::nullopt nếu chưa
     */
    virtual std::optional<std::string> getCurrentUserId() const = 0;
      /**
     * @brief Lấy vai trò của người dùng hiện tại
     * @return Vai trò của người dùng nếu đã đăng nhập, hoặc std::nullopt nếu chưa
     */
    virtual std::optional<UserRole> getCurrentUserRole() const = 0;
    
    /**
     * @brief Lấy đối tượng người dùng hiện tại
     * @return Đối tượng User nếu đã đăng nhập, hoặc std::nullopt nếu chưa
     */
    virtual std::optional<std::shared_ptr<User>> getCurrentUser() const = 0;
    
    /**
     * @brief Đăng ký tài khoản sinh viên mới
     * 
     * @param data Dữ liệu đăng ký sinh viên
     * @param plainPassword Mật khẩu dạng văn bản thuần
     * @return true nếu đăng ký thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> registerStudent(const StudentRegistrationData& data, const std::string& plainPassword) = 0;
    
    /**
     * @brief Thay đổi mật khẩu
     * 
     * @param currentUserId ID của người dùng hiện tại
     * @param oldPassword Mật khẩu cũ
     * @param newPassword Mật khẩu mới
     * @return true nếu thay đổi thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> changePassword(const std::string& currentUserId, const std::string& oldPassword, const std::string& newPassword) = 0;
};

#endif // IAUTHSERVICE_H