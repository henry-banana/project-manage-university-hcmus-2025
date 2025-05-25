/**
 * @file Student.h
 * @brief Định nghĩa lớp Student đại diện cho sinh viên trong hệ thống
 * 
 * File này định nghĩa lớp Student, kế thừa từ lớp User, đại diện cho thông tin 
 * sinh viên trong hệ thống quản lý trường đại học. Lớp này bổ sung các thuộc tính
 * và phương thức đặc trưng cho sinh viên.
 */
#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include <string>

// Forward declaration
class Faculty;

/**
 * @class Student
 * @brief Lớp đại diện cho sinh viên trong hệ thống
 * 
 * Lớp này kế thừa từ lớp User và bổ sung các thuộc tính và phương thức
 * đặc trưng cho sinh viên, như mã khoa và các phương thức để quản lý thông tin
 * sinh viên.
 */
class Student : public User {
private:
    std::string _facultyId; ///< ID của Khoa mà sinh viên thuộc về

public:
    /**
     * @brief Constructor tạo đối tượng Student
     * 
     * @param id Mã sinh viên
     * @param firstName Tên của sinh viên
     * @param lastName Họ của sinh viên
     * @param facultyId Mã khoa mà sinh viên thuộc về
     * @param status Trạng thái tài khoản (mặc định là PENDING_APPROVAL)
     */
    Student(const std::string& id,
            const std::string& firstName,
            const std::string& lastName,
            const std::string& facultyId,
            LoginStatus status = LoginStatus::PENDING_APPROVAL); // SV mới đăng ký mặc định chờ duyệt

    /**
     * @brief Lấy mã khoa của sinh viên
     * @return Mã khoa
     */
    const std::string& getFacultyId() const;
    
    /**
     * @brief Đặt mã khoa cho sinh viên
     * 
     * @param facultyId Mã khoa mới
     * @return true nếu thành công, false nếu thất bại
     */
    bool setFacultyId(const std::string& facultyId); // Cần validate Faculty ID tồn tại (ở Service)

    /**
     * @brief Tạo chuỗi hiển thị thông tin của sinh viên
     * @return Chuỗi chứa thông tin của sinh viên
     */
    std::string display() const override;
    
    /**
     * @brief Kiểm tra tính hợp lệ của các thuộc tính cơ bản
     * @return Kết quả kiểm tra tính hợp lệ
     */
    ValidationResult validateBasic() const override;
};

#endif // STUDENT_H