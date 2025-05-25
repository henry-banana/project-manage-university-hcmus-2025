/**
 * @file IResultService.h
 * @brief Định nghĩa giao diện dịch vụ quản lý kết quả học tập
 * 
 * File này định nghĩa giao diện IResultService, cung cấp các phương thức
 * để quản lý kết quả học tập của sinh viên trong hệ thống quản lý trường đại học,
 * bao gồm nhập điểm, truy vấn kết quả và tính toán điểm trung bình.
 */
#ifndef IRESULTSERVICE_H
#define IRESULTSERVICE_H

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/CourseResult.h"

/**
 * @class IResultService
 * @brief Giao diện dịch vụ quản lý kết quả học tập
 * 
 * Giao diện này định nghĩa các phương thức để quản lý kết quả học tập
 * của sinh viên, bao gồm nhập điểm, truy vấn kết quả và tính toán điểm trung bình.
 */
class IResultService {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IResultService() = default;
    
    /**
     * @brief Nhập điểm cho một môn học của sinh viên
     * 
     * @param studentId ID của sinh viên
     * @param courseId ID của khóa học
     * @param marks Điểm số
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> enterMarks(const std::string& studentId, const std::string& courseId, int marks) = 0;
    
    /**
     * @brief Nhập điểm cho nhiều môn học của sinh viên
     * 
     * @param studentId ID của sinh viên
     * @param courseMarksMap Bảng ánh xạ từ ID khóa học đến điểm số
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> enterMultipleMarks(const std::string& studentId, const std::map<std::string, int>& courseMarksMap) = 0;
    
    /**
     * @brief Lấy kết quả học tập cụ thể của sinh viên cho một môn học
     * 
     * @param studentId ID của sinh viên
     * @param courseId ID của khóa học
     * @return Đối tượng CourseResult nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<CourseResult, Error> getSpecificResult(const std::string& studentId, const std::string& courseId) const = 0;
    
    /**
     * @brief Lấy kết quả học tập của tất cả môn học cho một sinh viên
     * 
     * @param studentId ID của sinh viên
     * @return Danh sách kết quả học tập nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<CourseResult>, Error> getResultsByStudent(const std::string& studentId) const = 0;
    
    /**
     * @brief Lấy kết quả học tập của tất cả sinh viên cho một môn học
     * 
     * @param courseId ID của khóa học
     * @return Danh sách kết quả học tập nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<CourseResult>, Error> getResultsByCourse(const std::string& courseId) const = 0;
    
    /**
     * @brief Tạo báo cáo kết quả học tập của sinh viên
     * 
     * @param studentId ID của sinh viên
     * @return Chuỗi báo cáo nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::string, Error> generateStudentResultReport(const std::string& studentId) const = 0;
    
    /**
     * @brief Tính toán điểm trung bình tích lũy (CGPA) của sinh viên
     * 
     * @param studentId ID của sinh viên
     * @return Điểm trung bình tích lũy nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<double, Error> calculateCGPA(const std::string& studentId) const = 0;
};

#endif // IRESULTSERVICE_H