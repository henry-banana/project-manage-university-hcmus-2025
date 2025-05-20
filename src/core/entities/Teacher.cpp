#include "Teacher.h"
#include <sstream>
#include "../../utils/StringUtils.h"

Teacher::Teacher(const std::string& id,
                 const std::string& firstName,
                 const std::string& lastName,
                 const std::string& facultyId,
                 LoginStatus status)
    : User(id, firstName, lastName, UserRole::TEACHER, status),
      _facultyId(facultyId), _experienceYears(0) {} // Mặc định kinh nghiệm 0 năm

const std::string& Teacher::getFacultyId() const { return _facultyId; }
const std::string& Teacher::getQualification() const { return _qualification; }
const std::string& Teacher::getSpecializationSubjects() const { return _specializationSubjects; }
const std::string& Teacher::getDesignation() const { return _designation; }
int Teacher::getExperienceYears() const { return _experienceYears; }

bool Teacher::setFacultyId(const std::string& facultyId) {
    std::string trimmed = StringUtils::trim(facultyId);
    if (trimmed.empty() || trimmed.length() > 10) return false;
    _facultyId = trimmed;
    return true;
}

bool Teacher::setQualification(const std::string& qualification) {
    std::string trimmed = StringUtils::trim(qualification);
    if (trimmed.length() > 100) return false; // Giới hạn độ dài
    _qualification = trimmed;
    return true;
}

bool Teacher::setSpecializationSubjects(const std::string& subjects) {
    std::string trimmed = StringUtils::trim(subjects);
    if (trimmed.length() > 255) return false;
    _specializationSubjects = trimmed;
    return true;
}

bool Teacher::setDesignation(const std::string& designation) {
    std::string trimmed = StringUtils::trim(designation);
    if (trimmed.length() > 50) return false;
    _designation = trimmed;
    return true;
}

bool Teacher::setExperienceYears(int years) {
    if (years < 0 || years > 70) { // Giới hạn hợp lý
        return false;
    }
    _experienceYears = years;
    return true;
}

std::string Teacher::display() const {
    std::ostringstream oss;
    oss << "--- Teacher Information ---\n"
        << "ID            : " << _id << "\n"
        << "Full Name     : " << getFullName() << "\n"
        << "Birthday      : " << _birthday.toString_ddmmyyyy() << "\n"
        << "Address       : " << (_address.empty() ? "N/A" : _address) << "\n"
        << "Citizen ID    : " << (_citizenId.empty() ? "N/A" : _citizenId) << "\n"
        << "Email         : " << (_email.empty() ? "N/A" : _email) << "\n"
        << "Phone         : " << (_phoneNumber.empty() ? "N/A" : _phoneNumber) << "\n"
        << "Faculty ID    : " << _facultyId << "\n"
        << "Qualification : " << (_qualification.empty() ? "N/A" : _qualification) << "\n"
        << "Specialization: " << (_specializationSubjects.empty() ? "N/A" : _specializationSubjects) << "\n"
        << "Designation   : " << (_designation.empty() ? "N/A" : _designation) << "\n"
        << "Experience    : " << _experienceYears << " years\n"
        << "Role          : " << toStringUserRole(_role) << "\n"
        << "Status        : " << LoginStatusUtils::toString(_status) << "\n"
        << "---------------------------";
    return oss.str();
}

ValidationResult Teacher::validateBasic() const {
    ValidationResult vr;
    // Tương tự Student, validate các trường của User trước
    if (StringUtils::trim(_id).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Teacher ID cannot be empty.");
    if (StringUtils::trim(_firstName).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "First name cannot be empty.");
    if (StringUtils::trim(_lastName).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Last name cannot be empty.");
    if (StringUtils::trim(_facultyId).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Faculty ID cannot be empty for teacher.");
    if (_experienceYears < 0) vr.addError(ErrorCode::VALIDATION_ERROR, "Experience years cannot be negative.");
    // Email (bắt buộc cho Teacher)
    if (StringUtils::trim(_email).empty()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Email is required for teachers.");
    } else {
        if (_email.find('@') == std::string::npos || _email.find('.') == std::string::npos) {
            vr.addError(ErrorCode::VALIDATION_ERROR, "Invalid email format.");
        }
    }
    // Citizen ID (bắt buộc cho Teacher)
    if (StringUtils::trim(_citizenId).empty()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID is required for teachers.");
    }
    // ... các validation khác cho qualification, designation... (chủ yếu là không quá dài)
    return vr;
}