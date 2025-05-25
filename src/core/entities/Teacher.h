/**
 * @file Teacher.h
 * @brief Định nghĩa lớp Teacher đại diện cho giảng viên trong hệ thống
 * 
 * File này định nghĩa lớp Teacher, kế thừa từ lớp User, đại diện cho thông tin 
 * giảng viên trong hệ thống quản lý trường đại học. Lớp này bổ sung các thuộc tính
 * và phương thức đặc trưng cho giảng viên.
 */
#ifndef TEACHER_H
#define TEACHER_H

#include "User.h"
#include <string>
#include <vector>

// Forward declaration
class Faculty;
// class Course; // Nếu Teacher có danh sách Course dạy

/**
 * @class Teacher
 * @brief Lớp đại diện cho giảng viên trong hệ thống
 * 
 * Lớp này kế thừa từ lớp User và bổ sung các thuộc tính và phương thức
 * đặc trưng cho giảng viên, như mã khoa, bằng cấp, chuyên ngành giảng dạy,
 * chức vụ và số năm kinh nghiệm.
 */
class Teacher : public User {
private:
    std::string _facultyId; ///< ID của Khoa mà giảng viên thuộc về
    std::string _qualification; ///< Bằng cấp (e.g., PhD, MSc)
    std::string _specializationSubjects; ///< Chuyên ngành giảng dạy (có thể là list)
    std::string _designation; ///< Chức vụ (e.g., Lecturer, Professor, HOD)
    int _experienceYears; ///< Số năm kinh nghiệm

public:
    /**
     * @brief Constructor tạo đối tượng Teacher
     * 
     * @param id Mã giảng viên
     * @param firstName Tên của giảng viên
     * @param lastName Họ của giảng viên
     * @param facultyId Mã khoa mà giảng viên thuộc về
     * @param status Trạng thái tài khoản (mặc định là ACTIVE)
     */
    Teacher(const std::string& id,
            const std::string& firstName,
            const std::string& lastName,
            const std::string& facultyId,
            LoginStatus status = LoginStatus::ACTIVE);

    /**
     * @brief Lấy mã khoa của giảng viên
     * @return Mã khoa
     */
    const std::string& getFacultyId() const;
    
    /**
     * @brief Lấy thông tin bằng cấp của giảng viên
     * @return Bằng cấp
     */
    const std::string& getQualification() const;
    
    /**
     * @brief Lấy thông tin chuyên ngành giảng dạy
     * @return Chuyên ngành giảng dạy
     */    /**
     * @brief Lấy chức vụ của giảng viên
     * @return Chức vụ
     */
    const std::string& getDesignation() const;
    
    /**
     * @brief Lấy số năm kinh nghiệm của giảng viên
     * @return Số năm kinh nghiệm
     */
    int getExperienceYears() const;

    /**
     * @brief Đặt mã khoa cho giảng viên
     * @param facultyId Mã khoa mới
     * @return true nếu thành công, false nếu thất bại
     */
    bool setFacultyId(const std::string& facultyId);
    
    /**
     * @brief Đặt bằng cấp cho giảng viên
     * @param qualification Bằng cấp mới
     * @return true nếu thành công, false nếu thất bại
     */
    bool setQualification(const std::string& qualification);
    
    /**
     * @brief Đặt chuyên ngành giảng dạy cho giảng viên
     * @param subjects Chuyên ngành giảng dạy mới
     * @return true nếu thành công, false nếu thất bại
     */
    bool setSpecializationSubjects(const std::string& subjects);
    
    /**
     * @brief Đặt chức vụ cho giảng viên
     * @param designation Chức vụ mới
     * @return true nếu thành công, false nếu thất bại
     */
    bool setDesignation(const std::string& designation);
    
    /**
     * @brief Đặt số năm kinh nghiệm cho giảng viên
     * @param years Số năm kinh nghiệm mới (phải >= 0)
     * @return true nếu thành công, false nếu thất bại
     */
    bool setExperienceYears(int years);

    /**
     * @brief Tạo chuỗi hiển thị thông tin của giảng viên
     * @return Chuỗi chứa thông tin của giảng viên
     */
    std::string display() const override;
    
    /**
     * @brief Kiểm tra tính hợp lệ của các thuộc tính cơ bản
     * @return Kết quả kiểm tra tính hợp lệ
     */
    ValidationResult validateBasic() const override;
};

#endif // TEACHER_H