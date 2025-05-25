#include "StudentValidator.h"
#include "../../../utils/StringUtils.h" // For trim

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

ValidationResult StudentValidator::validateEntity(const Student& student) const {
    ValidationResult vr;

    // 1. Validate các trường kế thừa từ User thông qua GeneralInputValidator (hoặc UserValidator nếu có)
    ValidationResult idVr = _generalValidator->validateIdFormat(student.getId(), "Student ID", 3, 20); // Ví dụ: S01, T001
    for(const auto& err : idVr.errors) vr.addError(err);

    ValidationResult firstNameVr = _generalValidator->validateRequiredString(student.getFirstName(), "First Name", 50);
    for(const auto& err : firstNameVr.errors) vr.addError(err);

    ValidationResult lastNameVr = _generalValidator->validateRequiredString(student.getLastName(), "Last Name", 50);
    for(const auto& err : lastNameVr.errors) vr.addError(err);
    
    // Birthday
    if (!student.getBirthday().isSet()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Birthday is required for students.");
    } else {
        ValidationResult bdayVr = _generalValidator->validateDate(
            student.getBirthday().getDay(), 
            student.getBirthday().getMonth(), 
            student.getBirthday().getYear(),
            "Birthday"
        );
        for(const auto& err : bdayVr.errors) vr.addError(err);
        // Kiểm tra tuổi hợp lý (ví dụ: > 16)
        // time_t t = time(0); 
        // tm* now = localtime(&t);
        // int currentYear = now->tm_year + 1900;
        // if (student.getBirthday().isSet() && (currentYear - student.getBirthday().getYear() < 16)){
        //    vr.addError(ErrorCode::VALIDATION_ERROR, "Student must be at least 16 years old.");
        // }
    }

    ValidationResult emailVr = _generalValidator->validateEmail(student.getEmail());
     if (StringUtils::trim(student.getEmail()).empty()){ // Email bắt buộc
        vr.addError(ErrorCode::VALIDATION_ERROR, "Email is required for students.");
    } else {
        for(const auto& err : emailVr.errors) vr.addError(err);
    }


    ValidationResult citizenIdVr = _generalValidator->validateCitizenId(student.getCitizenId(), "VN");
    if (StringUtils::trim(student.getCitizenId()).empty()){ // CitizenID bắt buộc
        vr.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID is required for students.");
    } else {
        for(const auto& err : citizenIdVr.errors) vr.addError(err);
    }
    
    // Các trường tùy chọn
    ValidationResult addressVr = _generalValidator->validateOptionalString(student.getAddress(), "Address", 200);
    for(const auto& err : addressVr.errors) vr.addError(err);
    
    ValidationResult phoneVr = _generalValidator->validatePhoneNumber(student.getPhoneNumber(), "VN");
    if (!StringUtils::trim(student.getPhoneNumber()).empty()){ // Chỉ validate nếu có nhập
         for(const auto& err : phoneVr.errors) vr.addError(err);
    }


    // 2. Validate các trường riêng của Student
    ValidationResult facultyIdVr = _generalValidator->validateRequiredString(student.getFacultyId(), "Faculty ID", 10);
    for(const auto& err : facultyIdVr.errors) vr.addError(err);

    // Kiểm tra sự tồn tại của FacultyId (Nếu IFacultyDao được inject)
    // if (_facultyDao) {
    //     auto facultyExists = _facultyDao->exists(student.getFacultyId());
    //     if (!facultyExists.has_value() || !facultyExists.value()) {
    //         vr.addError(ErrorCode::VALIDATION_ERROR, "Faculty ID '" + student.getFacultyId() + "' does not exist.");
    //     }
    // }

    // 3. Validate Role và Status (thường không cần vì chúng được hệ thống set, nhưng có thể kiểm tra logic)
    if (student.getRole() != UserRole::STUDENT && student.getRole() != UserRole::PENDING_STUDENT) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Invalid role for a student object.");
    }
    // Các trạng thái LoginStatus đã được định nghĩa, không cần validate giá trị enum ở đây trừ khi có logic đặc biệt.

    return vr;
}