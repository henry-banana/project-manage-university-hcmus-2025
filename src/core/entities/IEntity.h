/**
 * @file IEntity.h
 * @brief Định nghĩa giao diện cho tất cả các thực thể trong hệ thống
 * 
 * File này định nghĩa giao diện IEntity, được sử dụng làm cơ sở cho tất cả các
 * thực thể trong hệ thống quản lý trường đại học. Giao diện này cung cấp các
 * phương thức chung mà tất cả các thực thể phải triển khai.
 */
#ifndef IENTITY_H
#define IENTITY_H

#include <string>
#include <vector>
#include "../../common/ErrorType.h" // For Error struct
#include "../../common/ValidationResult.h" // For ValidationResult

/**
 * @class IEntity
 * @brief Giao diện cơ sở cho tất cả các thực thể trong hệ thống
 * 
 * Giao diện này định nghĩa các phương thức mà tất cả các thực thể phải triển khai,
 * bao gồm khả năng lấy ID dưới dạng chuỗi, hiển thị thông tin thực thể,
 * và tự kiểm tra tính hợp lệ của các thuộc tính cơ bản.
 */
class IEntity {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IEntity() = default;

    /**
     * @brief Lấy ID của thực thể dưới dạng chuỗi
     * 
     * Bắt buộc phải có ID, nhưng kiểu ID có thể khác nhau, nên tạm để là string
     * Các lớp con cụ thể có thể có getter ID với kiểu mạnh hơn
     * 
     * @return ID của thực thể dưới dạng chuỗi
     */
    virtual std::string getStringId() const = 0;

    /**
     * @brief Tạo chuỗi hiển thị thông tin của thực thể
     * 
     * Được sử dụng để debug và hiển thị cơ bản thông tin của thực thể
     * 
     * @return Chuỗi chứa thông tin của thực thể
     */
    virtual std::string display() const = 0;

    /**
     * @brief Kiểm tra tính hợp lệ của các thuộc tính cơ bản
     * 
     * Mỗi entity có thể tự validate các thuộc tính cơ bản của nó.
     * Validator bên ngoài sẽ validate các rule phức tạp hơn (liên quan DB, logic nghiệp vụ)
     * 
     * @return Kết quả kiểm tra tính hợp lệ
     */
    virtual ValidationResult validateBasic() const = 0;
};

#endif // IENTITY_H