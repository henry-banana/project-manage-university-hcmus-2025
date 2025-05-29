#include "StudentValidator.h"
#include "../../../utils/StringUtils.h" // For trim

/**
 * @brief Khởi tạo đối tượng StudentValidator với validator đầu vào chung
 * 
 * @param generalValidator Con trỏ đến đối tượng validator chung
 * @throw std::invalid_argument Nếu generalValidator là nullptr
 */
StudentValidator::StudentValidator(std::shared_ptr<IGeneralInputValidator> generalValidator)
    : _generalValidator(std::move(generalValidator)) {
    if (!_generalValidator) {
        throw std::invalid_argument("GeneralInputValidator cannot be null for StudentValidator.");
    }
}

// StudentValidator::StudentValidator(std::shared_ptr<IGeneralInputValidator> generalValidator, std::shared_ptr<IFacultyDao> facultyDao)
//    : _generalValidator(std::move(generalValidator)), _facultyDao(std::move(facultyDao)) {
//     if (!_generalValidator) { /* ... */ }
//     if (!_facultyDao) { /* ... */ }
// }

/**
 * @brief Kiểm tra tính hợp lệ của đối tượng sinh viên
 * 
 * Phương thức này kiểm tra tất cả các trường của đối tượng sinh viên:
 * - Thông tin cơ bản: ID, họ tên, ngày sinh, email, CMND/CCCD
 * - Thông tin liên hệ: địa chỉ, số điện thoại
 * - Thông tin học tập: mã khoa
 * - Vai trò và trạng thái tài khoản
 * 
 * @param student Đối tượng sinh viên cần kiểm tra
 * @return ValidationResult Kết quả kiểm tra với danh sách lỗi (nếu có)
 */
/**
 * @brief Kiểm tra tính hợp lệ của đối tượng sinh viên
 * 
 * Phương thức này kiểm tra tất cả các trường của đối tượng sinh viên:
 * - Thông tin cơ bản: ID, họ tên, ngày sinh, email, CMND/CCCD
 * - Thông tin liên hệ: địa chỉ, số điện thoại
 * - Thông tin học tập: mã khoa
 * - Vai trò và trạng thái tài khoản
 * 
 * @param student Đối tượng sinh viên cần kiểm tra
 * @return ValidationResult Kết quả kiểm tra với danh sách lỗi (nếu có)
 */
ValidationResult StudentValidator::validateEntity(const Student& student) const {
    ValidationResult vr;
    auto appendErrors = [&](const ValidationResult& r) {
        for(const auto& err : r.errors) vr.addError(err);
    };

    appendErrors(_generalValidator->validateIdFormat(student.getId(), "Student ID", 3, 20));
    appendErrors(_generalValidator->validateRequiredString(student.getFirstName(), "First Name", 50));
    appendErrors(_generalValidator->validateRequiredString(student.getLastName(), "Last Name", 50));

    if (!student.getBirthday().isSet()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Birthday is required for students.");
    } else {
        appendErrors(_generalValidator->validateDate(
            student.getBirthday().getDay(),
            student.getBirthday().getMonth(),
            student.getBirthday().getYear(),
            "Birthday"
        ));
        // (Optional) Age check:
        // time_t t = time(0); 
        // tm* now = localtime(&t);
        // int currentYear = now->tm_year + 1900;
        // if (student.getBirthday().isSet() && (currentYear - student.getBirthday().getYear() < 16)){
        //    appendErrors(_generalValidator->validateInteger(currentYear - student.getBirthday().getYear(), "Age", 16, 100));
        // }
    }

    if (StringUtils::trim(student.getEmail()).empty()){
        vr.addError(ErrorCode::VALIDATION_ERROR, "Email is required for students.");
    } else {
        appendErrors(_generalValidator->validateEmail(student.getEmail()));
    }

    if (StringUtils::trim(student.getCitizenId()).empty()){
        vr.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID is required for students.");
    } else {
        appendErrors(_generalValidator->validateCitizenId(student.getCitizenId()));
    }

    appendErrors(_generalValidator->validateOptionalString(student.getAddress(), "Address", 200));
    if (!StringUtils::trim(student.getPhoneNumber()).empty()){ // Phone number is optional for student
         appendErrors(_generalValidator->validatePhoneNumber(student.getPhoneNumber()));
    }
    appendErrors(_generalValidator->validateRequiredString(student.getFacultyId(), "Faculty ID", 10));

    // Role and Status are usually set by the system logic, but can be validated here if needed
    if (student.getRole() != UserRole::STUDENT && student.getRole() != UserRole::PENDING_STUDENT) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Invalid role for a student object.");
    }
    // LoginStatus is an enum, its value itself doesn't need validation unless there are specific allowed statuses.
    return vr;
}