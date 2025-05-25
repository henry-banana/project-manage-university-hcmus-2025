/**
 * @file AuthService.h
 * @brief Định nghĩa lớp dịch vụ xác thực người dùng
 * 
 * Lớp AuthService cung cấp các chức năng xác thực, đăng nhập, đăng ký và
 * quản lý phiên làm việc của người dùng trong hệ thống quản lý trường đại học.
 */
#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include "../interface/IAuthService.h" // Đã được cập nhật để dùng std::expected
#include "../../data_access/interface/ILoginDao.h"
#include "../../data_access/interface/IStudentDao.h"
#include "../../data_access/interface/ITeacherDao.h" // (➕) Thêm nếu cần check email teacher
#include "../../../utils/PasswordInput.h"
#include "../../../utils/Logger.h"
#include "../../validators/interface/IValidator.h" // IGeneralInputValidator
#include "../../entities/Student.h"
#include "../../entities/Teacher.h" // (➕)
#include "../SessionContext.h"
#include <memory> // Cho std::shared_ptr

/**
 * @class AuthService
 * @brief Lớp cung cấp các dịch vụ xác thực và quản lý phiên làm việc
 * 
 * AuthService triển khai các chức năng của IAuthService, bao gồm đăng nhập,
 * đăng xuất, đăng ký tài khoản sinh viên mới và quản lý thông tin phiên hiện tại.
 */
class AuthService : public IAuthService {
private:
    std::shared_ptr<ILoginDao> _loginDao; /**< DAO để truy vấn thông tin đăng nhập */
    std::shared_ptr<IStudentDao> _studentDao; /**< DAO để truy vấn thông tin sinh viên */
    std::shared_ptr<ITeacherDao> _teacherDao; /**< DAO để truy vấn thông tin giảng viên */
    std::shared_ptr<IGeneralInputValidator> _inputValidator; /**< Bộ xác thực đầu vào */
    std::shared_ptr<SessionContext> _sessionContext; /**< Context lưu trữ thông tin phiên làm việc */

public:
    /**
     * @brief Constructor với đầy đủ các dependency
     * @param loginDao DAO để truy vấn thông tin đăng nhập
     * @param studentDao DAO để truy vấn thông tin sinh viên
     * @param teacherDao DAO để truy vấn thông tin giảng viên
     * @param inputValidator Bộ xác thực đầu vào
     * @param sessionContext Context lưu trữ thông tin phiên làm việc
     */
    AuthService(std::shared_ptr<ILoginDao> loginDao,
                std::shared_ptr<IStudentDao> studentDao,
                std::shared_ptr<ITeacherDao> teacherDao,
                std::shared_ptr<IGeneralInputValidator> inputValidator,
                std::shared_ptr<SessionContext> sessionContext);
    
    /**
     * @brief Destructor mặc định
     */
    ~AuthService() override = default;

    /**
     * @brief Đăng nhập với ID hoặc email và mật khẩu
     * @param userIdOrEmail ID người dùng hoặc địa chỉ email
     * @param password Mật khẩu
     * @return Đối tượng User nếu thành công hoặc Error nếu thất bại
     */
    std::expected<std::shared_ptr<User>, Error> login(const std::string& userIdOrEmail, const std::string& password) override;
    
    /**
     * @brief Đăng xuất người dùng hiện tại
     */
    void logout() override;
    
    /**
     * @brief Kiểm tra xem có người dùng đang đăng nhập hay không
     * @return true nếu có người dùng đăng nhập, false nếu không
     */
    bool isAuthenticated() const override;
    
    /**
     * @brief Lấy ID của người dùng hiện tại
     * @return ID người dùng hoặc nullopt nếu không có người dùng đăng nhập
     */
    std::optional<std::string> getCurrentUserId() const override;
    
    /**
     * @brief Lấy vai trò của người dùng hiện tại
     * @return Vai trò người dùng hoặc nullopt nếu không có người dùng đăng nhập
     */
    std::optional<UserRole> getCurrentUserRole() const override;
    
    /**
     * @brief Lấy đối tượng User của người dùng hiện tại
     * @return Con trỏ đến đối tượng User hoặc nullopt nếu không có người dùng đăng nhập
     */
    std::optional<std::shared_ptr<User>> getCurrentUser() const override;
    
    /**
     * @brief Đăng ký tài khoản sinh viên mới
     * @param data Dữ liệu đăng ký sinh viên
     * @param plainPassword Mật khẩu dạng plain text
     * @return true nếu thành công hoặc Error nếu thất bại
     */
    std::expected<bool, Error> registerStudent(const StudentRegistrationData& data, const std::string& plainPassword) override;
    
    /**
     * @brief Thay đổi mật khẩu cho người dùng
     * @param currentUserId ID của người dùng cần thay đổi mật khẩu
     * @param oldPassword Mật khẩu cũ
     * @param newPassword Mật khẩu mới
     * @return true nếu thành công hoặc Error nếu thất bại
     */
    std::expected<bool, Error> changePassword(const std::string& currentUserId, const std::string& oldPassword, const std::string& newPassword) override;
};

#endif // AUTHSERVICE_H