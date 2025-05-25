#include "Course.h"
#include <sstream>
#include "../../utils/StringUtils.h"

Course::Course(std::string id, std::string name, int credits, std::string facultyId)
    : _id(std::move(id)), _name(std::move(name)), _credits(credits), _facultyId(std::move(facultyId)) {}

const std::string& Course::getId() const { return _id; }
const std::string& Course::getName() const { return _name; }
int Course::getCredits() const { return _credits; }
const std::string& Course::getFacultyId() const { return _facultyId; }

bool Course::setName(const std::string& name) {
    std::string trimmed = StringUtils::trim(name);
    if (trimmed.empty() || trimmed.length() > 150) return false;
    _name = trimmed;
    return true;
}

bool Course::setCredits(int credits) {
    if (credits <= 0 || credits > 10) { // Giả sử tín chỉ từ 1-10
        return false;
    }
    _credits = credits;
    return true;
}

bool Course::setFacultyId(const std::string& facultyId) {
    std::string trimmed = StringUtils::trim(facultyId);
    if (trimmed.empty() || trimmed.length() > 10) return false;
    _facultyId = trimmed;
    return true;
}

std::string Course::getStringId() const { return _id; }

std::string Course::display() const {
    std::ostringstream oss;
    oss << "--- Course Information ---\n"
        << "ID         : " << _id << "\n"
        << "Name       : " << _name << "\n"
        << "Credits    : " << _credits << "\n"
        << "Faculty ID : " << _facultyId << "\n"
        << "------------------------";
    return oss.str();
}

ValidationResult Course::validateBasic() const {
    ValidationResult vr;
    if (StringUtils::trim(_id).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Course ID cannot be empty.");
    else if (_id.length() > 20) vr.addError(ErrorCode::VALIDATION_ERROR, "Course ID too long (max 20 chars).");

    if (StringUtils::trim(_name).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Course name cannot be empty.");
    else if (_name.length() > 150) vr.addError(ErrorCode::VALIDATION_ERROR, "Course name too long (max 150 chars).");

    if (_credits <= 0 || _credits > 10) vr.addError(ErrorCode::VALIDATION_ERROR, "Credits must be between 1 and 10.");

    if (StringUtils::trim(_facultyId).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Faculty ID for course cannot be empty.");
    return vr;
}