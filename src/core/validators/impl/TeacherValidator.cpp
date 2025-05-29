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
    auto appendErrors = [&](const ValidationResult& r) {
        for(const auto& err : r.errors) vr.addError(err);
    };

    appendErrors(_generalValidator->validateIdFormat(teacher.getId(), "Teacher ID", 3, 20));
    appendErrors(_generalValidator->validateRequiredString(teacher.getFirstName(), "First Name", 50));
    appendErrors(_generalValidator->validateRequiredString(teacher.getLastName(), "Last Name", 50));

    if (teacher.getBirthday().isSet()) { // Birthday might be optional for teachers initially
        appendErrors(_generalValidator->validateDate(
            teacher.getBirthday().getDay(), 
            teacher.getBirthday().getMonth(), 
            teacher.getBirthday().getYear(),
            "Birthday"
        ));
    }
    
    if (StringUtils::trim(teacher.getEmail()).empty()){
        vr.addError(ErrorCode::VALIDATION_ERROR, "Email is required for teachers.");
    } else {
        appendErrors(_generalValidator->validateEmail(teacher.getEmail()));
    }

    if (StringUtils::trim(teacher.getCitizenId()).empty()){
        vr.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID is required for teachers.");
    } else {
        appendErrors(_generalValidator->validateCitizenId(teacher.getCitizenId()));
    }

    appendErrors(_generalValidator->validateOptionalString(teacher.getAddress(), "Address", 200));
    if (!StringUtils::trim(teacher.getPhoneNumber()).empty()){
         appendErrors(_generalValidator->validatePhoneNumber(teacher.getPhoneNumber()));
    }

    appendErrors(_generalValidator->validateRequiredString(teacher.getFacultyId(), "Faculty ID", 10));
    appendErrors(_generalValidator->validateOptionalString(teacher.getQualification(), "Qualification", 100));
    appendErrors(_generalValidator->validateOptionalString(teacher.getSpecializationSubjects(), "Specialization Subjects", 255));
    appendErrors(_generalValidator->validateOptionalString(teacher.getDesignation(), "Designation", 50));
    appendErrors(_generalValidator->validateInteger(teacher.getExperienceYears(), "Experience Years", 0, 70));

    if (teacher.getRole() != UserRole::TEACHER) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Invalid role for a teacher object.");
    }
    return vr;
}