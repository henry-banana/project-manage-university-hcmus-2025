#include "TeacherValidator.h"
#include "../../../utils/StringUtils.h"

/**
 * @brief Khởi tạo đối tượng TeacherValidator với validator đầu vào chung
 * 
 * @param generalValidator Con trỏ đến đối tượng validator chung
 * @throw std::invalid_argument Nếu generalValidator là nullptr
 */
TeacherValidator::TeacherValidator(std::shared_ptr<IGeneralInputValidator> generalValidator)
    : _generalValidator(std::move(generalValidator)) {
    if (!_generalValidator) {
        throw std::invalid_argument("GeneralInputValidator cannot be null for TeacherValidator.");
    }
}

/**
 * @brief Kiểm tra tính hợp lệ của đối tượng giảng viên
 * 
 * Phương thức này kiểm tra tất cả các trường của đối tượng giảng viên:
 * - Thông tin cơ bản: ID, họ tên, ngày sinh, email, CMND/CCCD
 * - Thông tin liên hệ: địa chỉ, số điện thoại
 * - Thông tin chuyên môn: mã khoa, bằng cấp, chuyên môn, chức danh, số năm kinh nghiệm
 * - Vai trò tài khoản
 * 
 * @param teacher Đối tượng giảng viên cần kiểm tra
 * @return ValidationResult Kết quả kiểm tra với danh sách lỗi (nếu có)
 */
ValidationResult TeacherValidator::validateEntity(const Teacher& teacher) const {
    ValidationResult vr;

    // Validate User fields
    ValidationResult idVr = _generalValidator->validateIdFormat(teacher.getId(), "Teacher ID", 3, 20);
    for(const auto& err : idVr.errors) vr.addError(err);

    ValidationResult firstNameVr = _generalValidator->validateRequiredString(teacher.getFirstName(), "First Name", 50);
    for(const auto& err : firstNameVr.errors) vr.addError(err);

    ValidationResult lastNameVr = _generalValidator->validateRequiredString(teacher.getLastName(), "Last Name", 50);
    for(const auto& err : lastNameVr.errors) vr.addError(err);

    if (teacher.getBirthday().isSet()) { // Birthday có thể không bắt buộc với Teacher
        ValidationResult bdayVr = _generalValidator->validateDate(
            teacher.getBirthday().getDay(), 
            teacher.getBirthday().getMonth(), 
            teacher.getBirthday().getYear(),
            "Birthday"
        );
        for(const auto& err : bdayVr.errors) vr.addError(err);
    }
    
    ValidationResult emailVr = _generalValidator->validateEmail(teacher.getEmail());
    if (StringUtils::trim(teacher.getEmail()).empty()){ // Email bắt buộc
        vr.addError(ErrorCode::VALIDATION_ERROR, "Email is required for teachers.");
    } else {
        for(const auto& err : emailVr.errors) vr.addError(err);
    }

    ValidationResult citizenIdVr = _generalValidator->validateCitizenId(teacher.getCitizenId(), "VN");
     if (StringUtils::trim(teacher.getCitizenId()).empty()){ // CitizenID bắt buộc
        vr.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID is required for teachers.");
    } else {
        for(const auto& err : citizenIdVr.errors) vr.addError(err);
    }

    ValidationResult addressVr = _generalValidator->validateOptionalString(teacher.getAddress(), "Address", 200);
    for(const auto& err : addressVr.errors) vr.addError(err);
    
    ValidationResult phoneVr = _generalValidator->validatePhoneNumber(teacher.getPhoneNumber(), "VN");
    if (!StringUtils::trim(teacher.getPhoneNumber()).empty()){
         for(const auto& err : phoneVr.errors) vr.addError(err);
    }

    // Validate Teacher specific fields
    ValidationResult facultyIdVr = _generalValidator->validateRequiredString(teacher.getFacultyId(), "Faculty ID", 10);
    for(const auto& err : facultyIdVr.errors) vr.addError(err);

    ValidationResult qualificationVr = _generalValidator->validateOptionalString(teacher.getQualification(), "Qualification", 100);
    for(const auto& err : qualificationVr.errors) vr.addError(err);

    ValidationResult specSubjectsVr = _generalValidator->validateOptionalString(teacher.getSpecializationSubjects(), "Specialization Subjects", 255);
    for(const auto& err : specSubjectsVr.errors) vr.addError(err);

    ValidationResult designationVr = _generalValidator->validateOptionalString(teacher.getDesignation(), "Designation", 50);
    for(const auto& err : designationVr.errors) vr.addError(err);

    ValidationResult expYearsVr = _generalValidator->validateInteger(teacher.getExperienceYears(), "Experience Years", 0, 70);
    for(const auto& err : expYearsVr.errors) vr.addError(err);

    if (teacher.getRole() != UserRole::TEACHER) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Invalid role for a teacher object.");
    }
    return vr;
}