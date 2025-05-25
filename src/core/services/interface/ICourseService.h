/**
 * @file ICourseService.h
 * @brief Định nghĩa giao diện dịch vụ quản lý khóa học
 * 
 * File này định nghĩa giao diện ICourseService, cung cấp các phương thức
 * để quản lý thông tin khóa học trong hệ thống quản lý trường đại học,
 * bao gồm thêm, sửa, xóa, và truy vấn khóa học.
 */
#ifndef ICOURSESERVICE_H
#define ICOURSESERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/Course.h"

/**
 * @class ICourseService
 * @brief Giao diện dịch vụ quản lý khóa học
 * 
 * Giao diện này định nghĩa các phương thức để quản lý thông tin khóa học
 * trong hệ thống, bao gồm thêm, sửa, xóa, và truy vấn khóa học.
 */
class ICourseService {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~ICourseService() = default;
    
    /**
     * @brief Lấy thông tin khóa học theo ID
     * 
     * @param courseId ID của khóa học cần lấy thông tin
     * @return Đối tượng Course nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<Course, Error> getCourseById(const std::string& courseId) const = 0;
    
    /**
     * @brief Lấy danh sách tất cả khóa học
     * @return Danh sách khóa học nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Course>, Error> getAllCourses() const = 0;
    
    /**
     * @brief Lấy danh sách khóa học theo khoa
     * 
     * @param facultyId ID của khoa cần lấy danh sách khóa học
     * @return Danh sách khóa học nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Course>, Error> getCoursesByFaculty(const std::string& facultyId) const = 0;
    
    /**
     * @brief Thêm khóa học mới
     * 
     * @param id ID của khóa học mới
     * @param name Tên của khóa học mới
     * @param credits Số tín chỉ của khóa học mới
     * @param facultyId ID của khoa quản lý khóa học
     * @return Đối tượng Course mới nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<Course, Error> addCourse(const std::string& id, const std::string& name, int credits, const std::string& facultyId) = 0;
    
    /**
     * @brief Cập nhật thông tin khóa học
     * 
     * @param courseId ID của khóa học cần cập nhật
     * @param newName Tên mới của khóa học
     * @param newCredits Số tín chỉ mới của khóa học
     * @param newFacultyId ID mới của khoa quản lý khóa học
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> updateCourse(const std::string& courseId, const std::string& newName, int newCredits, const std::string& newFacultyId) = 0;
    
    /**
     * @brief Xóa khóa học
     * 
     * @param courseId ID của khóa học cần xóa
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> removeCourse(const std::string& courseId) = 0;
};

#endif // ICOURSESERVICE_H