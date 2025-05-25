/**
 * @file StudentValidator.h
 * @brief Định nghĩa lớp kiểm tra tính hợp lệ của đối tượng sinh viên
 * 
 * Lớp StudentValidator triển khai giao diện IEntityValidator để kiểm tra
 * tính hợp lệ của các đối tượng sinh viên trước khi thêm hoặc cập nhật.
 */
#ifndef STUDENTVALIDATOR_H
#define STUDENTVALIDATOR_H

#include "../interface/IValidator.h"
#include "../../entities/Student.h"
#include <memory> // For std::shared_ptr for IGeneralInputValidator

/**
 * @class StudentValidator
 * @brief Lớp kiểm tra tính hợp lệ của đối tượng sinh viên
 * 
 * Lớp này triển khai giao diện IEntityValidator<Student> để kiểm tra
 * tính hợp lệ của các thuộc tính trong đối tượng sinh viên.
 */
class StudentValidator : public IEntityValidator<Student> {
private:
    std::shared_ptr<IGeneralInputValidator> _generalValidator; ///< Validator kiểm tra các giá trị đầu vào chung
    // (Optional) Có thể inject các DAO interface nếu cần kiểm tra sự tồn tại của FacultyId
    // std::shared_ptr<IFacultyDao> _facultyDao; 
public:
    // StudentValidator(std::shared_ptr<IGeneralInputValidator> generalValidator, std::shared_ptr<IFacultyDao> facultyDao);
    
    /**
     * @brief Hàm khởi tạo
     * @param generalValidator Validator kiểm tra các giá trị đầu vào chung
     */
    explicit StudentValidator(std::shared_ptr<IGeneralInputValidator> generalValidator);
    
    /**
     * @brief Hàm hủy mặc định
     */
    ~StudentValidator() override = default;

    /**
     * @brief Kiểm tra tính hợp lệ của đối tượng sinh viên
     * @param student Đối tượng sinh viên cần kiểm tra
     * @return Kết quả kiểm tra
     */
    ValidationResult validateEntity(const Student& student) const override;
};

#endif // STUDENTVALIDATOR_H