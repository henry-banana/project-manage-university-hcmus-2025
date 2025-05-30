/**
 * @file Faculty.h
 * @brief Định nghĩa lớp Faculty đại diện cho khoa trong hệ thống
 * 
 * File này định nghĩa lớp Faculty, triển khai giao diện IEntity, đại diện cho thông tin 
 * khoa trong hệ thống quản lý trường đại học. Lớp này cung cấp các thuộc tính
 * và phương thức để quản lý thông tin khoa.
 */
#ifndef FACULTY_H
#define FACULTY_H

#include "IEntity.h"
#include <string>

/**
 * @class Faculty
 * @brief Lớp đại diện cho khoa trong hệ thống
 * 
 * Lớp này triển khai giao diện IEntity và cung cấp các thuộc tính và phương thức
 * để quản lý thông tin của một khoa, bao gồm mã khoa và tên khoa.
 */
class Faculty : public IEntity {
private:
    std::string _id;   ///< Mã khoa (e.g., "IT", "EE")
    std::string _name; ///< Tên khoa (e.g., "Information Technology")

public:
    /**
     * @brief Constructor tạo đối tượng Faculty
     * 
     * @param id Mã khoa
     * @param name Tên khoa
     */
    Faculty(std::string id, std::string name);

    /**
     * @brief Lấy mã khoa
     * @return Mã khoa
     */
    const std::string& getId() const;
    
    /**
     * @brief Lấy tên khoa
     * @return Tên khoa
     */
    const std::string& getName() const;

    /**
     * @brief Đặt tên cho khoa
     * @param name Tên mới (không được rỗng)
     * @return true nếu thành công, false nếu thất bại
     */
    bool setName(const std::string& name);

    /**
     * @brief Lấy ID của khoa dưới dạng chuỗi
     * @return ID của khoa
     */
    std::string getStringId() const override;
    
    /**
     * @brief Tạo chuỗi hiển thị thông tin của khoa
     * @return Chuỗi chứa thông tin của khoa
     */
    std::string display() const override;
    
    /**
     * @brief Kiểm tra tính hợp lệ của các thuộc tính cơ bản
     * @return Kết quả kiểm tra tính hợp lệ
     */
    ValidationResult validateBasic() const override;
};

#endif // FACULTY_H