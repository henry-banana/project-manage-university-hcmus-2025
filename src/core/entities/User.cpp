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
    if (trimmed.empty() || trimmed.length() > 50) {
        return false;
    }
    _firstName = trimmed;
    return true;
}

bool User::setLastName(const std::string& lastName) {
    std::string trimmed = StringUtils::trim(lastName);
    if (trimmed.empty() || trimmed.length() > 50) {
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
    // Ví dụ: CCCD Việt Nam thường có 12 số
    std::string trimmed = StringUtils::trim(citizenId);
    if (trimmed.length() != 9 && trimmed.length() != 12) { // Cho phép cả CMND cũ và CCCD
         for(char c : trimmed) if (!isdigit(c)) return false; // Phải là số
        //return false;
    }
    _citizenId = trimmed;
    return true;
}

bool User::setEmail(const std::string& email) {
    std::string trimmed = StringUtils::trim(email);
    // Regex đơn giản cho email, bạn có thể dùng regex phức tạp hơn
    const std::regex email_regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)"); // C++11 regex
    if (trimmed.empty() || !std::regex_match(trimmed, email_regex)) {
        return false;
    }
    // Tạm thời chấp nhận nếu có @ và .
    if (trimmed.find('@') == std::string::npos || trimmed.find('.') == std::string::npos || trimmed.length() < 5) {
        if (!trimmed.empty()) return false; // Cho phép email rỗng nếu không bắt buộc
    }
    _email = trimmed;
    return true;
}

bool User::setPhoneNumber(const std::string& phoneNumber) {
    std::string trimmed = StringUtils::trim(phoneNumber);
    // Regex cho SĐT Việt Nam (ví dụ đơn giản: 10-11 số, bắt đầu bằng 0)
    const std::regex phone_regex(R"(0\d{9,10})");
    if (trimmed.empty() || !std::regex_match(trimmed, phone_regex)) {
        return false;
    }
    // Tạm thời: phải là số, dài 10-11 ký tự
    if (!trimmed.empty()) {
        if (trimmed.length() < 10 || trimmed.length() > 11) return false;
        for(char c : trimmed) if (!isdigit(c)) return false;
    }
    _phoneNumber = trimmed;
    return true;
}

void User::setRole(UserRole role) { _role = role; }
void User::setStatus(LoginStatus status) { _status = status; }