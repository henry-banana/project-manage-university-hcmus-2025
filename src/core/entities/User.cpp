#include "User.h"
#include <sstream> // Cho getFullName, toString
#include <regex>   // Cho validation email, phone (nếu làm ở đây)
#include "../../utils/StringUtils.h" // Nếu có StringUtils::trim

// Helper để chuyển UserRole sang string (nếu chưa có ở UserRole.h)
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
      _role(role), _status(status), _birthday() // Khởi tạo Birthday mặc định
{
    // Các trường khác như address, citizenId, email, phoneNumber sẽ được set sau
    // hoặc thêm vào constructor nếu chúng là bắt buộc khi tạo User.
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

// Setters
bool User::setFirstName(const std::string& firstName) {
    // Ví dụ validation đơn giản: không rỗng, không quá dài
    std::string trimmed = StringUtils::trim(firstName); // Giả sử có StringUtils
    if (trimmed.length() > 50) { // Bỏ check empty, để GeneralValidator làm
        return false;
    }
    _firstName = trimmed;
    return true;
}

bool User::setLastName(const std::string& lastName) {
    std::string trimmed = StringUtils::trim(lastName);
    if (trimmed.length() > 50) { // Bỏ check empty
        return false;
    }
    _lastName = trimmed;
    return true;
}

bool User::setBirthday(const Birthday& birthday) {
    if (birthday.isSet() && !birthday.validate().isValid) { // Chỉ validate nếu birthday được set
        return false;
    }
    _birthday = birthday;
    return true;
}
bool User::setBirthday(int day, int month, int year) {
    return _birthday.set(day, month, year);
}

bool User::setAddress(const std::string& address) {
    std::string trimmed = StringUtils::trim(address);
    if (trimmed.length() > 200) { // Giới hạn độ dài
        return false;
    }
    _address = trimmed;
    return true;
}

bool User::setCitizenId(const std::string& citizenId) {
    std::string trimmed = StringUtils::trim(citizenId);
    // Entity chỉ nên làm validate rất cơ bản, ví dụ độ dài max
    if (trimmed.length() > 20) { 
        return false;
    }
    // Không cần check isdigit ở đây, để GeneralValidator làm.
    _citizenId = trimmed;
    return true;
}

bool User::setEmail(const std::string& email) {
    std::string trimmed = StringUtils::trim(email);
    // Entity chỉ nên làm validate rất cơ bản
    if (trimmed.length() > 100) {
         return false;
    }
    // Không cần regex ở đây, để GeneralValidator làm.
    _email = trimmed;
    return true;
}

bool User::setPhoneNumber(const std::string& phoneNumber) {
    std::string trimmed = StringUtils::trim(phoneNumber);
    // Entity chỉ nên làm validate rất cơ bản
    if (trimmed.length() > 15) { 
         return false;
    }
    // Không cần regex ở đây, để GeneralValidator làm.
    _phoneNumber = trimmed;
    return true;
}

void User::setRole(UserRole role) { _role = role; }
void User::setStatus(LoginStatus status) { _status = status; }