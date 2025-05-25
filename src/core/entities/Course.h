/**
 * @file Course.h
 * @brief Định nghĩa lớp Course đại diện cho khóa học trong hệ thống
 * 
 * File này định nghĩa lớp Course, triển khai giao diện IEntity, đại diện cho thông tin 
 * khóa học trong hệ thống quản lý trường đại học. Lớp này cung cấp các thuộc tính
 * và phương thức để quản lý thông tin khóa học.
 */
#ifndef COURSE_H
#define COURSE_H

#include "IEntity.h"
#include <string>

/**
 * @class Course
 * @brief Lớp đại diện cho khóa học trong hệ thống
 * 
 * Lớp này triển khai giao diện IEntity và cung cấp các thuộc tính và phương thức
 * để quản lý thông tin của một khóa học, bao gồm mã, tên, số tín chỉ và mã khoa.
 */
class Course : public IEntity {
private:
    std::string _id;         ///< Mã môn học (e.g., "CS101")
    std::string _name;       ///< Tên môn học (e.g., "Introduction to Programming")
    int _credits;            ///< Số tín chỉ
    std::string _facultyId;  ///< Khoa quản lý môn học

public:
    /**
     * @brief Constructor tạo đối tượng Course
     * 
     * @param id Mã môn học
     * @param name Tên môn học
     * @param credits Số tín chỉ
     * @param facultyId Mã khoa quản lý môn học
     */
    Course(std::string id, std::string name, int credits, std::string facultyId);

    /**
     * @brief Lấy mã môn học
     * @return Mã môn học
     */
    const std::string& getId() const;
    
    /**
     * @brief Lấy tên môn học
     * @return Tên môn học
     */
    const std::string& getName() const;
    
    /**
     * @brief Lấy số tín chỉ của môn học
     * @return Số tín chỉ
     */
    int getCredits() const;
    
    /**
     * @brief Lấy mã khoa quản lý môn học
     * @return Mã khoa
     */
    const std::string& getFacultyId() const;

    /**
     * @brief Đặt tên cho môn học
     * @param name Tên mới
     * @return true nếu thành công, false nếu thất bại
     */
    bool setName(const std::string& name);
    
    /**
     * @brief Đặt số tín chỉ cho môn học
     * @param credits Số tín chỉ mới (phải > 0)
     * @return true nếu thành công, false nếu thất bại
     */
    bool setCredits(int credits);
    
    /**
     * @brief Đặt mã khoa quản lý môn học
     * @param facultyId Mã khoa mới
     * @return true nếu thành công, false nếu thất bại
     */
    bool setFacultyId(const std::string& facultyId);

    /**
     * @brief Lấy ID của môn học dưới dạng chuỗi
     * @return ID của môn học
     */
    std::string getStringId() const override;
    
    /**
     * @brief Tạo chuỗi hiển thị thông tin của môn học
     * @return Chuỗi chứa thông tin của môn học
     */
    std::string display() const override;
    
    /**
     * @brief Kiểm tra tính hợp lệ của các thuộc tính cơ bản
     * @return Kết quả kiểm tra tính hợp lệ
     */
    ValidationResult validateBasic() const override;
};

#endif // COURSE_H