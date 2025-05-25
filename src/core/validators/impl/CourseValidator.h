/**
 * @file CourseValidator.h
 * @brief Định nghĩa lớp kiểm tra tính hợp lệ của đối tượng khóa học
 * 
 * Lớp CourseValidator triển khai giao diện IEntityValidator để kiểm tra
 * tính hợp lệ của các đối tượng khóa học trước khi thêm hoặc cập nhật.
 */
#ifndef COURSEVALIDATOR_H
#define COURSEVALIDATOR_H

#include "../interface/IValidator.h"
#include "../../entities/Course.h"
#include <memory>

/**
 * @class CourseValidator
 * @brief Lớp kiểm tra tính hợp lệ của đối tượng khóa học
 * 
 * Lớp này triển khai giao diện IEntityValidator<Course> để kiểm tra
 * tính hợp lệ của các thuộc tính trong đối tượng khóa học.
 */
class CourseValidator : public IEntityValidator<Course> {
private:
    std::shared_ptr<IGeneralInputValidator> _generalValidator; ///< Validator kiểm tra các giá trị đầu vào chung
public:
    /**
     * @brief Hàm khởi tạo
     * @param generalValidator Validator kiểm tra các giá trị đầu vào chung
     */
    explicit CourseValidator(std::shared_ptr<IGeneralInputValidator> generalValidator);
    
    /**
     * @brief Hàm hủy mặc định
     */
    ~CourseValidator() override = default;

    /**
     * @brief Kiểm tra tính hợp lệ của đối tượng khóa học
     * @param course Đối tượng khóa học cần kiểm tra
     * @return Kết quả kiểm tra
     */
    ValidationResult validateEntity(const Course& course) const override;
};

#endif // COURSEVALIDATOR_H