#include "Student.h"
#include <sstream> // Cho toString
#include "../../utils/StringUtils.h"

Student::Student(const std::string& id,
                 const std::string& firstName,
                 const std::string& lastName,
                 const std::string& facultyId,
                 LoginStatus status)
    : User(id, firstName, lastName, UserRole::STUDENT, status), // Role mặc định là STUDENT
      _facultyId(facultyId) {}

const std::string& Student::getFacultyId() const {
    return _facultyId;
}

bool Student::setFacultyId(const std::string& facultyId) {
    // Validation cơ bản (ví dụ: không rỗng). Việc kiểm tra facultyId có tồn tại không
    // nên được thực hiện ở tầng Service khi có Zugriff vào FacultyDao.
    std::string trimmed = StringUtils::trim(facultyId);
    if (trimmed.empty() || trimmed.length() > 10) { // Giả sử mã khoa không quá 10 ký tự
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
    if (_birthday.isSet()) { // Chỉ validate nếu ngày sinh được set
        ValidationResult bdayVr = _birthday.validate();
        if (!bdayVr.isValid) {
            for (const auto& err : bdayVr.errors) vr.addError(err);
        }
    } else {
        // Giả sử ngày sinh là bắt buộc cho Student
        vr.addError(ErrorCode::VALIDATION_ERROR, "Birthday is required for students.");
    }


    // Faculty ID
    if (StringUtils::trim(_facultyId).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Faculty ID cannot be empty.");
    else if (_facultyId.length() > 10) vr.addError(ErrorCode::VALIDATION_ERROR, "Faculty ID too long (max 10 chars).");

    // Email (bắt buộc cho Student)
    if (StringUtils::trim(_email).empty()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Email is required for students.");
    } else {
        // Tạm validate đơn giản, GeneralInputValidator sẽ làm kỹ hơn
        if (_email.find('@') == std::string::npos || _email.find('.') == std::string::npos) {
            vr.addError(ErrorCode::VALIDATION_ERROR, "Invalid email format.");
        }
    }

    // Citizen ID (bắt buộc cho Student)
    if (StringUtils::trim(_citizenId).empty()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID is required for students.");
    } else {
        // Validate citizen ID (ví dụ, 9 hoặc 12 số)
        std::string cid = StringUtils::trim(_citizenId);
        bool allDigits = true;
        for (char c : cid) if (!isdigit(c)) allDigits = false;
        if (!allDigits || (cid.length() != 9 && cid.length() != 12)) {
             vr.addError(ErrorCode::VALIDATION_ERROR, "Invalid Citizen ID format (must be 9 or 12 digits).");
        }
    }
    // Role và Status được set bởi hệ thống, không cần user validate ở đây
    return vr;
}