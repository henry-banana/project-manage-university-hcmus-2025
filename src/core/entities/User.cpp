#include "User.h"
#include <sstream> 
#include "../../utils/StringUtils.h" 

std::string toStringUserRole(UserRole role) {
    switch (role) {
        case UserRole::ADMIN: return "Admin";
        case UserRole::STUDENT: return "Student";
        case UserRole::TEACHER: return "Teacher";
        case UserRole::PENDING_STUDENT: return "Pending Student";
        case UserRole::UNKNOWN: return "Unknown";
        default: return "Invalid Role";
    }
}


User::User(std::string id, std::string firstName, std::string lastName, UserRole role, LoginStatus status)
    : _id(std::move(id)), _firstName(std::move(firstName)), _lastName(std::move(lastName)),
      _role(role), _status(status), _birthday() 
{
}

std::string User::getStringId() const {
    return _id;
}

const std::string& User::getId() const { return _id; }
const std::string& User::getFirstName() const { return _firstName; }
const std::string& User::getLastName() const { return _lastName; }

std::string User::getFullName() const {
    std::ostringstream oss;
    oss << _firstName;
    if (!_firstName.empty() && !_lastName.empty()) {
        oss << " ";
    }
    oss << _lastName;
    return oss.str();
}

const Birthday& User::getBirthday() const { return _birthday; }
const std::string& User::getAddress() const { return _address; }
const std::string& User::getCitizenId() const { return _citizenId; }
const std::string& User::getEmail() const { return _email; }
const std::string& User::getPhoneNumber() const { return _phoneNumber; }
UserRole User::getRole() const { return _role; }
LoginStatus User::getStatus() const { return _status; }

bool User::setFirstName(const std::string& firstName) {
    std::string trimmed = StringUtils::trim(firstName);
    if (trimmed.length() > 50) { // Max length check
        return false;
    }
    _firstName = trimmed;
    return true;
}

bool User::setLastName(const std::string& lastName) {
    std::string trimmed = StringUtils::trim(lastName);
    if (trimmed.length() > 50) { // Max length check
        return false;
    }
    _lastName = trimmed;
    return true;
}

bool User::setBirthday(const Birthday& birthday) {
    // Birthday object handles its own basic validation internally via set()
    _birthday = birthday; 
    return _birthday.isSet() ? _birthday.validate().isValid : true; // Allow unsetting, or if set, ensure it's valid
}
bool User::setBirthday(int day, int month, int year) {
    return _birthday.set(day, month, year);
}

bool User::setAddress(const std::string& address) {
    std::string trimmed = StringUtils::trim(address);
    if (trimmed.length() > 200) { 
        return false;
    }
    _address = trimmed;
    return true;
}

bool User::setCitizenId(const std::string& citizenId) {
    std::string trimmed = StringUtils::trim(citizenId);
    if (trimmed.length() > 20) {  // Max length check
        return false;
    }
    _citizenId = trimmed;
    return true;
}

bool User::setEmail(const std::string& email) {
    std::string trimmed = StringUtils::trim(email);
    if (trimmed.length() > 100) { // Max length check
         return false;
    }
    _email = trimmed;
    return true;
}

bool User::setPhoneNumber(const std::string& phoneNumber) {
    std::string trimmed = StringUtils::trim(phoneNumber);
    if (trimmed.length() > 15) { // Max length check
         return false;
    }
    _phoneNumber = trimmed;
    return true;
}

void User::setRole(UserRole role) { _role = role; }
void User::setStatus(LoginStatus status) { _status = status; }