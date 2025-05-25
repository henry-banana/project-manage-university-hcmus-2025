#include "CourseValidator.h"

/**
 * @brief Khởi tạo đối tượng CourseValidator với validator đầu vào chung
 * 
 * @param generalValidator Con trỏ đến đối tượng validator chung
 * @throw std::invalid_argument Nếu generalValidator là nullptr
 */
CourseValidator::CourseValidator(std::shared_ptr<IGeneralInputValidator> generalValidator)
    : _generalValidator(std::move(generalValidator)) {
    if (!_generalValidator) {
        throw std::invalid_argument("GeneralInputValidator cannot be null for CourseValidator.");
    }
}

/**
 * @brief Kiểm tra tính hợp lệ của đối tượng khóa học
 * 
 * Phương thức này kiểm tra tất cả các trường của đối tượng khóa học:
 * - ID khóa học
 * - Tên khóa học
 * - Số tín chỉ
 * - ID khoa
 * 
 * @param course Đối tượng khóa học cần kiểm tra
 * @return ValidationResult Kết quả kiểm tra với danh sách lỗi (nếu có)
 */
ValidationResult CourseValidator::validateEntity(const Course& course) const {
    ValidationResult vr;

    ValidationResult idVr = _generalValidator->validateIdFormat(course.getId(), "Course ID", 3, 20);
    for(const auto& err : idVr.errors) vr.addError(err);

    ValidationResult nameVr = _generalValidator->validateRequiredString(course.getName(), "Course Name", 150);
    for(const auto& err : nameVr.errors) vr.addError(err);

    ValidationResult creditsVr = _generalValidator->validateInteger(course.getCredits(), "Credits", 1, 10); // Giả sử tín chỉ từ 1-10
    for(const auto& err : creditsVr.errors) vr.addError(err);
    
    ValidationResult facultyIdVr = _generalValidator->validateRequiredString(course.getFacultyId(), "Faculty ID for Course", 10);
    for(const auto& err : facultyIdVr.errors) vr.addError(err);

    return vr;
}