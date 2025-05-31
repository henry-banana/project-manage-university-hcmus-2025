#include "Faculty.h"
#include <sstream>
#include "../../utils/StringUtils.h" 

Faculty::Faculty(std::string id, std::string name)
    : _id(std::move(id)), _name(std::move(name)) {}

const std::string& Faculty::getId() const { return _id; }
const std::string& Faculty::getName() const { return _name; }

bool Faculty::setName(const std::string& name) {
    std::string trimmed = StringUtils::trim(name);
    if (trimmed.empty() || trimmed.length() > 100) { // Thêm check empty
        return false;
    }
    _name = trimmed;
    return true;
}

std::string Faculty::getStringId() const { return _id; }

std::string Faculty::display() const {
    std::ostringstream oss;
    oss << "--- Faculty Information ---\n"
        << "ID   : " << _id << "\n"
        << "Name : " << _name << "\n"
        << "-------------------------";
    return oss.str();
}

ValidationResult Faculty::validateBasic() const {
    ValidationResult vr;
    if (StringUtils::trim(_id).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Faculty ID cannot be empty.");
    else if (_id.length() > 10) vr.addError(ErrorCode::VALIDATION_ERROR, "Faculty ID too long (max 10 chars).");

    if (StringUtils::trim(_name).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Faculty name cannot be empty.");
    else if (_name.length() > 100) vr.addError(ErrorCode::VALIDATION_ERROR, "Faculty name too long (max 100 chars).");
    return vr;
}