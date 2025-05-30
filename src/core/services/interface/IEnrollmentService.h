/**
 * @file IEnrollmentService.h
 * @brief Định nghĩa giao diện dịch vụ quản lý đăng ký khóa học
 * 
 * File này định nghĩa giao diện IEnrollmentService, cung cấp các phương thức
 * để quản lý việc đăng ký và hủy đăng ký khóa học của sinh viên trong hệ thống
 * quản lý trường đại học.
 */
#ifndef IENROLLMENTSERVICE_H
#define IENROLLMENTSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/Course.h"
#include "../../entities/Student.h"

/**
 * @class IEnrollmentService
 * @brief Giao diện dịch vụ quản lý đăng ký khóa học
 * 
 * Giao diện này định nghĩa các phương thức để quản lý việc đăng ký và hủy đăng ký
 * khóa học của sinh viên, cũng như truy vấn thông tin về các khóa học đã đăng ký.
 */
class IEnrollmentService {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IEnrollmentService() = default;
    
    /**
     * @brief Đăng ký khóa học cho sinh viên
     * 
     * @param studentId ID của sinh viên
     * @param courseId ID của khóa học
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> enrollStudentInCourse(const std::string& studentId, const std::string& courseId) = 0;
    
    /**
     * @brief Hủy đăng ký khóa học cho sinh viên
     * 
     * @param studentId ID của sinh viên
     * @param courseId ID của khóa học
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> dropCourseForStudent(const std::string& studentId, const std::string& courseId) = 0;
    
    /**
     * @brief Lấy danh sách khóa học mà sinh viên đã đăng ký
     * 
     * @param studentId ID của sinh viên
     * @return Danh sách khóa học nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Course>, Error> getEnrolledCoursesByStudent(const std::string& studentId) const = 0;
    
    /**
     * @brief Lấy danh sách sinh viên đã đăng ký khóa học
     * 
     * @param courseId ID của khóa học
     * @return Danh sách sinh viên nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Student>, Error> getEnrolledStudentsByCourse(const std::string& courseId) const = 0;
    
    /**
     * @brief Kiểm tra sinh viên đã đăng ký khóa học chưa
     * 
     * @param studentId ID của sinh viên
     * @param courseId ID của khóa học
     * @return true nếu đã đăng ký, false nếu chưa, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> isStudentEnrolled(const std::string& studentId, const std::string& courseId) const = 0;
};

#endif // IENROLLMENTSERVICE_H