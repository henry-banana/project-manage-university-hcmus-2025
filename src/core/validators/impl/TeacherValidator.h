/**
 * @file TeacherValidator.h
 * @brief Định nghĩa lớp kiểm tra tính hợp lệ của đối tượng giáo viên
 * 
 * Lớp TeacherValidator triển khai giao diện IEntityValidator để kiểm tra
 * tính hợp lệ của các đối tượng giáo viên trước khi thêm hoặc cập nhật.
 */
#ifndef TEACHERVALIDATOR_H
#define TEACHERVALIDATOR_H

#include "../interface/IValidator.h"
#include "../../entities/Teacher.h"
#include <memory>

/**
 * @class TeacherValidator
 * @brief Lớp kiểm tra tính hợp lệ của đối tượng giáo viên
 * 
 * Lớp này triển khai giao diện IEntityValidator<Teacher> để kiểm tra
 * tính hợp lệ của các thuộc tính trong đối tượng giáo viên.
 */
class TeacherValidator : public IEntityValidator<Teacher> {
private:
    std::shared_ptr<IGeneralInputValidator> _generalValidator; ///< Validator kiểm tra các giá trị đầu vào chung
public:
    /**
     * @brief Hàm khởi tạo
     * @param generalValidator Validator kiểm tra các giá trị đầu vào chung
     */
    explicit TeacherValidator(std::shared_ptr<IGeneralInputValidator> generalValidator);
    
    /**
     * @brief Hàm hủy mặc định
     */
    ~TeacherValidator() override = default;

    /**
     * @brief Kiểm tra tính hợp lệ của đối tượng giáo viên
     * @param teacher Đối tượng giáo viên cần kiểm tra
     * @return Kết quả kiểm tra
     */
    ValidationResult validateEntity(const Teacher& teacher) const override;
};

#endif // TEACHERVALIDATOR_H