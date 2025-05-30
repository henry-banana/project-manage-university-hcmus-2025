/**
 * @file IAdminService.h
 * @brief Định nghĩa giao diện dịch vụ quản trị hệ thống
 * 
 * File này định nghĩa giao diện IAdminService, cung cấp các phương thức
 * để quản lý tài khoản người dùng trong hệ thống quản lý trường đại học,
 * bao gồm thêm, xóa, phê duyệt và quản lý quyền truy cập cho sinh viên và giảng viên.
 */
#ifndef IADMINSERVICE_H
#define IADMINSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../../common/LoginStatus.h"
#include "../../entities/Student.h"
#include "../../entities/Teacher.h"
#include "IAuthService.h" // Để dùng StudentRegistrationData

/**
 * @struct NewStudentDataByAdmin
 * @brief Cấu trúc chứa dữ liệu để quản trị viên tạo sinh viên mới
 * 
 * Cấu trúc này được sử dụng để truyền thông tin sinh viên mới và mật khẩu
 * ban đầu cho dịch vụ quản trị để tạo tài khoản sinh viên.
 */
struct NewStudentDataByAdmin {
    StudentRegistrationData studentInfo; ///< Thông tin đăng ký sinh viên
    std::string initialPassword; ///< Mật khẩu ban đầu
};

/**
 * @struct NewTeacherDataByAdmin
 * @brief Cấu trúc chứa dữ liệu để quản trị viên tạo giảng viên mới
 * 
 * Cấu trúc này được sử dụng để truyền thông tin giảng viên mới và mật khẩu
 * ban đầu cho dịch vụ quản trị để tạo tài khoản giảng viên.
 */
struct NewTeacherDataByAdmin {
    std::string id; ///< ID của giảng viên
    std::string firstName; ///< Tên của giảng viên
    std::string lastName; ///< Họ của giảng viên
    std::string facultyId; ///< Mã khoa
    std::string initialPassword; ///< Mật khẩu ban đầu
    std::string qualification; ///< Bằng cấp
    std::string specializationSubjects; ///< Chuyên ngành giảng dạy
    std::string designation; ///< Chức vụ
    int experienceYears; ///< Số năm kinh nghiệm
    // Thêm các trường thông tin cá nhân khác từ User nếu cần thiết cho việc tạo mới qua admin
    int birthDay, birthMonth, birthYear; ///< Ngày, tháng, năm sinh
    std::string address; ///< Địa chỉ
    std::string citizenId; ///< Số CCCD
    std::string email; ///< Địa chỉ email (Bắt buộc)
    std::string phoneNumber; ///< Số điện thoại
};

/**
 * @class IAdminService
 * @brief Giao diện dịch vụ quản trị hệ thống
 * 
 * Giao diện này định nghĩa các phương thức để quản lý tài khoản người dùng
 * trong hệ thống, bao gồm thêm, xóa, phê duyệt và quản lý quyền truy cập.
 */
class IAdminService {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IAdminService() = default;

    /**
     * @brief Phê duyệt đăng ký tài khoản sinh viên
     * 
     * @param studentIdToApprove ID của sinh viên cần phê duyệt
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> approveStudentRegistration(const std::string& studentIdToApprove) = 0;
    
    /**
     * @brief Lấy danh sách sinh viên theo trạng thái
     * 
     * @param status Trạng thái tài khoản cần lọc
     * @return Danh sách sinh viên nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Student>, Error> getStudentsByStatus(LoginStatus status) const = 0;
    
    /**
     * @brief Thêm sinh viên mới bởi quản trị viên
     * 
     * @param data Dữ liệu sinh viên mới
     * @return Đối tượng Student mới nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<Student, Error> addStudentByAdmin(const NewStudentDataByAdmin& data) = 0;
    
    /**
     * @brief Xóa tài khoản sinh viên
     * 
     * @param studentId ID của sinh viên cần xóa
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> removeStudentAccount(const std::string& studentId) = 0;

    /**
     * @brief Thêm giảng viên mới bởi quản trị viên
     * 
     * @param data Dữ liệu giảng viên mới
     * @return Đối tượng Teacher mới nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<Teacher, Error> addTeacherByAdmin(const NewTeacherDataByAdmin& data) = 0;
    
    /**
     * @brief Xóa tài khoản giảng viên
     * 
     * @param teacherId ID của giảng viên cần xóa
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> removeTeacherAccount(const std::string& teacherId) = 0;

    /**
     * @brief Đặt lại mật khẩu của người dùng
     * 
     * @param userId ID của người dùng
     * @param newPassword Mật khẩu mới
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> resetUserPassword(const std::string& userId, const std::string& newPassword) = 0;
    
    /**
     * @brief Vô hiệu hóa tài khoản người dùng
     * 
     * @param userId ID của người dùng
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> disableUserAccount(const std::string& userId) = 0;
    
    /**
     * @brief Kích hoạt tài khoản người dùng
     * 
     * @param userId ID của người dùng
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> enableUserAccount(const std::string& userId) = 0;
};

#endif // IADMINSERVICE_H