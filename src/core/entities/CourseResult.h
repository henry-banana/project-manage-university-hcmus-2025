/**
 * @file CourseResult.h
 * @brief Định nghĩa lớp CourseResult đại diện cho kết quả học tập của sinh viên
 * 
 * File này định nghĩa lớp CourseResult, đại diện cho kết quả học tập của sinh viên
 * trong một khóa học cụ thể. Lớp này cung cấp các thuộc tính và phương thức để quản lý
 * điểm số và xếp loại của sinh viên.
 */
#ifndef COURSERESULT_H
#define COURSERESULT_H

#include <string>
#include "../../common/ValidationResult.h"

/**
 * @class CourseResult
 * @brief Lớp đại diện cho kết quả học tập của sinh viên trong một khóa học
 * 
 * Lớp này cung cấp các thuộc tính và phương thức để quản lý kết quả học tập
 * của sinh viên, bao gồm điểm số và xếp loại.
 */
class CourseResult {
private:
    std::string _studentId; ///< Mã sinh viên
    std::string _courseId; ///< Mã khóa học
    int _marks;   ///< Điểm số (từ -1 đến 100, -1 là chưa có điểm)
    char _grade;  ///< Xếp loại (A, B, C, D, F, - (chưa xếp loại))

    /**
     * @brief Tính toán xếp loại dựa trên điểm số
     * 
     * Phương thức hỗ trợ nội bộ để tính toán xếp loại (grade) từ điểm số (marks)
     */
    void calculateGrade();

public:
    /**
     * @brief Constructor tạo đối tượng CourseResult
     * 
     * @param studentId Mã sinh viên
     * @param courseId Mã khóa học
     * @param marks Điểm số (mặc định là -1, nghĩa là chưa có điểm)
     */
    CourseResult(std::string studentId, std::string courseId, int marks = -1);

    /**
     * @brief Lấy mã sinh viên
     * @return Mã sinh viên
     */
    const std::string& getStudentId() const;
    
    /**
     * @brief Lấy mã khóa học
     * @return Mã khóa học
     */
    const std::string& getCourseId() const;
    
    /**
     * @brief Lấy điểm số
     * @return Điểm số
     */
    int getMarks() const;
    
    /**
     * @brief Lấy xếp loại
     * @return Xếp loại (A, B, C, D, F, -)
     */
    char getGrade() const;

    /**
     * @brief Đặt điểm số
     * 
     * Phương thức này đặt điểm số và tự động tính toán lại xếp loại
     * 
     * @param marks Điểm số mới
     * @return true nếu thành công, false nếu thất bại
     */
    bool setMarks(int marks);

    /**
     * @brief Kiểm tra tính hợp lệ của các thuộc tính
     * @return Kết quả kiểm tra tính hợp lệ
     */
    ValidationResult validate() const;
    
    /**
     * @brief Tạo chuỗi hiển thị thông tin kết quả học tập
     * @return Chuỗi chứa thông tin kết quả học tập
     */
    std::string toString() const;
};

#endif // COURSERESULT_H