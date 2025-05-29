#include "Student.h"
#include <sstream> 
#include "../../utils/StringUtils.h"
#include "../../common/LoginStatus.h"

Student::Student(const std::string& id,
                 const std::string& firstName,
                 const std::string& lastName,
                 const std::string& facultyId,
                 LoginStatus status)
    : User(id, firstName, lastName, UserRole::STUDENT, status), 
      _facultyId(facultyId) {}

const std::string& Student::getFacultyId() const {
    return _facultyId;
}

bool Student::setFacultyId(const std::string& facultyId) {
    std::string trimmed = StringUtils::trim(facultyId);
    if (trimmed.length() > 10) { 
        return false;
    }
    _facultyId = trimmed;
    return true;
}

std::string Student::display() const { 
    std::ostringstream oss;
    oss << "--- Student Information ---\n"
        << "ID          : " << _id << "\n"
        << "Full Name   : " << getFullName() << "\n"
        << "Birthday    : " << _birthday.toString_ddmmyyyy() << "\n"
        << "Address     : " << (_address.empty() ? "N/A" : _address) << "\n"
        << "Citizen ID  : " << (_citizenId.empty() ? "N/A" : _citizenId) << "\n"
        << "Email       : " << (_email.empty() ? "N/A" : _email) << "\n"
        << "Phone       : " << (_phoneNumber.empty() ? "N/A" : _phoneNumber) << "\n"
        << "Faculty ID  : " << _facultyId << "\n"
        << "Role        : " << toStringUserRole(_role) << "\n"
        << "Status      : " << LoginStatusUtils::toString(_status) << "\n"
        << "---------------------------";
    return oss.str();
}

ValidationResult Student::validateBasic() const {
    ValidationResult vr;
    // ID
    if (StringUtils::trim(_id).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Student ID cannot be empty.");
    else if (_id.length() > 20) vr.addError(ErrorCode::VALIDATION_ERROR, "Student ID too long (max 20 chars).");

    // First Name
    if (StringUtils::trim(_firstName).empty()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "First name cannot be empty.");
    } else if (_firstName.length() > 50) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "First name too long (max 50 chars).");
    }
    // Last Name
    if (StringUtils::trim(_lastName).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Last name cannot be empty.");
    else if (_lastName.length() > 50) vr.addError(ErrorCode::VALIDATION_ERROR, "Last name too long (max 50 chars).");

    // Birthday
    if (!_birthday.isSet()) { 
        vr.addError(ErrorCode::VALIDATION_ERROR, "Birthday is required for students.");
    } else {
        ValidationResult bdayVr = _birthday.validate(); // Birthday tự validate ngày tháng
        if (!bdayVr.isValid) {
            for (const auto& err : bdayVr.errors) vr.addError(err);
        }
    }

    // Faculty ID
    if (StringUtils::trim(_facultyId).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Faculty ID cannot be empty for student.");
    else if (_facultyId.length() > 10) vr.addError(ErrorCode::VALIDATION_ERROR, "Faculty ID too long (max 10 chars).");

    // Email (bắt buộc cho Student)
    if (StringUtils::trim(_email).empty()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Email is required for students.");
    } else if (_email.length() > 100) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Email is too long (max 100 chars).");
    }
    // Không check format email ở đây, để GeneralValidator làm

    // Citizen ID (bắt buộc cho Student)
    if (StringUtils::trim(_citizenId).empty()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID is required for students.");
    } else if (_citizenId.length() > 20) {
         vr.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID is too long (max 20 chars).");
    }
    // Không check format Citizen ID ở đây

    // Phone Number (optional)
    if (!_phoneNumber.empty() && _phoneNumber.length() > 15) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Phone number is too long (max 15 chars).");
    }
    // Address (optional)
    if (!_address.empty() && _address.length() > 200) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Address is too long (max 200 chars).");
    }


    if (_role != UserRole::STUDENT && _role != UserRole::PENDING_STUDENT) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Invalid role for a student object.");
    }
    return vr;
}