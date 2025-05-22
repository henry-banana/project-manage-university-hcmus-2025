#include "CourseValidator.h"

CourseValidator::CourseValidator(std::shared_ptr<IGeneralInputValidator> generalValidator)
    : _generalValidator(std::move(generalValidator)) {
    if (!_generalValidator) {
        throw std::invalid_argument("GeneralInputValidator cannot be null for CourseValidator.");
    }
}

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