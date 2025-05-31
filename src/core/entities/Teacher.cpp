#include "Teacher.h"
#include <sstream>
#include "../../utils/StringUtils.h"
#include "../../common/LoginStatus.h"

Teacher::Teacher(const std::string& id,
                 const std::string& firstName,
                 const std::string& lastName,
                 const std::string& facultyId,
                 LoginStatus status)
    : User(id, firstName, lastName, UserRole::TEACHER, status),
      _facultyId(facultyId), _experienceYears(0) {} 

const std::string& Teacher::getFacultyId() const { return _facultyId; }
const std::string& Teacher::getQualification() const { return _qualification; }
const std::string& Teacher::getSpecializationSubjects() const { return _specializationSubjects; }
const std::string& Teacher::getDesignation() const { return _designation; }
int Teacher::getExperienceYears() const { return _experienceYears; }

bool Teacher::setFacultyId(const std::string& facultyId) {
    std::string trimmed = StringUtils::trim(facultyId);
    if (trimmed.empty() || trimmed.length() > 10) { 
        return false;
    }
    _facultyId = trimmed;
    return true;
}

bool Teacher::setQualification(const std::string& qualification) {
    std::string trimmed = StringUtils::trim(qualification);
    if (trimmed.length() > 100) return false; 
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
    if (years < 0 || years > 70) { 
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
    if (StringUtils::trim(_id).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Teacher ID cannot be empty.");
    else if (_id.length() > 20) vr.addError(ErrorCode::VALIDATION_ERROR, "Teacher ID too long (max 20 chars).");

    if (StringUtils::trim(_firstName).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "First name cannot be empty.");
    else if (_firstName.length() > 50) vr.addError(ErrorCode::VALIDATION_ERROR, "First name too long (max 50 chars).");

    if (StringUtils::trim(_lastName).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Last name cannot be empty.");
    else if (_lastName.length() > 50) vr.addError(ErrorCode::VALIDATION_ERROR, "Last name too long (max 50 chars).");
    
    if (_birthday.isSet()) { // Birthday is optional for Teacher on creation by Admin, for example
        ValidationResult bdayVr = _birthday.validate();
        if (!bdayVr.isValid) {
            for (const auto& err : bdayVr.errors) vr.addError(err);
        }
    }

    if (StringUtils::trim(_facultyId).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Faculty ID cannot be empty for teacher.");
    else if (_facultyId.length() > 10) vr.addError(ErrorCode::VALIDATION_ERROR, "Faculty ID too long (max 10 chars).");
    
    if (_experienceYears < 0 || _experienceYears > 70) vr.addError(ErrorCode::VALIDATION_ERROR, "Experience years must be between 0 and 70.");

    if (StringUtils::trim(_email).empty()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Email is required for teachers.");
    } else if (_email.length() > 100) {
         vr.addError(ErrorCode::VALIDATION_ERROR, "Email is too long (max 100 chars).");
    }

    if (StringUtils::trim(_citizenId).empty()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID is required for teachers.");
    } else if (_citizenId.length() > 20) {
         vr.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID is too long (max 20 chars).");
    }
    
    if (!_qualification.empty() && _qualification.length() > 100) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Qualification is too long (max 100 chars).");
    }
    if (!_specializationSubjects.empty() && _specializationSubjects.length() > 255) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Specialization Subjects is too long (max 255 chars).");
    }
    if (!_designation.empty() && _designation.length() > 50) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Designation is too long (max 50 chars).");
    }
     if (!_phoneNumber.empty() && _phoneNumber.length() > 15) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Phone number is too long (max 15 chars).");
    }
    if (!_address.empty() && _address.length() > 200) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Address is too long (max 200 chars).");
    }

    if (_role != UserRole::TEACHER) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Invalid role for a teacher object.");
    }
    return vr;
}