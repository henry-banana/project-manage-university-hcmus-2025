/**
 * @file ICourseResultDao.h
 * @brief Định nghĩa giao diện DAO cho việc truy cập dữ liệu kết quả khóa học
 * 
 * Giao diện ICourseResultDao định nghĩa các phương thức cần thiết để truy cập
 * và thao tác với dữ liệu kết quả khóa học của sinh viên.
 */
#ifndef ICOURSERESULTDAO_H
#define ICOURSERESULTDAO_H

#include "../../entities/CourseResult.h"
#include <string>
#include <vector>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)

/**
 * @class ICourseResultDao
 * @brief Giao diện DAO cho việc truy cập dữ liệu kết quả khóa học
 * 
 * Giao diện này định nghĩa các phương thức để truy cập và thao tác
 * với dữ liệu kết quả khóa học của sinh viên.
 */
class ICourseResultDao {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~ICourseResultDao() = default;

    /**
     * @brief Tìm kết quả khóa học theo ID sinh viên và ID khóa học
     * @param studentId ID của sinh viên
     * @param courseId ID của khóa học
     * @return Đối tượng CourseResult nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<CourseResult, Error> find(const std::string& studentId, const std::string& courseId) const = 0;
    
    /**
     * @brief Tìm tất cả kết quả khóa học của một sinh viên
     * @param studentId ID của sinh viên
     * @return Danh sách các đối tượng CourseResult nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<CourseResult>, Error> findByStudentId(const std::string& studentId) const = 0;
    
    /**
     * @brief Tìm tất cả kết quả của một khóa học
     * @param courseId ID của khóa học
     * @return Danh sách các đối tượng CourseResult nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<CourseResult>, Error> findByCourseId(const std::string& courseId) const = 0;
    
    /**
     * @brief Thêm mới hoặc cập nhật kết quả khóa học
     * @param result Đối tượng CourseResult cần thêm hoặc cập nhật
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> addOrUpdate(const CourseResult& result) = 0;
    
    /**
     * @brief Xóa kết quả khóa học theo ID sinh viên và ID khóa học
     * @param studentId ID của sinh viên
     * @param courseId ID của khóa học
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> remove(const std::string& studentId, const std::string& courseId) = 0;
    
    /**
     * @brief Xóa tất cả kết quả khóa học của một sinh viên
     * @param studentId ID của sinh viên
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> removeAllForStudent(const std::string& studentId) = 0;
    
    /**
     * @brief Xóa tất cả kết quả của một khóa học
     * @param courseId ID của khóa học
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> removeAllForCourse(const std::string& courseId) = 0;
};

#endif // ICOURSERESULTDAO_H