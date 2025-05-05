#include "User.h"

#include <sstream> // std::istringstream

User::User(std::string id,
    std::string firstName,
    std::string lastName,
    std::string address,
    std::string citizenID,
    std::string email, 
    std::string phoneNumber, 
    UserRole role)  
    : _id(std::move(id)),
      _first_name(std::move(firstName)),
      _last_name(std::move(lastName)),
      _birth_day(),
      _address(),
      _citizen_id(std::move(citizenID)),
      _email(std::move(email)), // Khởi tạo email
      _phone_number(std::move(phoneNumber)), // Khởi tạo số điện thoại
      _password_hash(), // Mã băm mật khẩu chưa được khởi tạo
      _salt(), // Muối chưa được khởi tạo
      _role(role) {} // Khởi tạo vai trò người dùng


User::User(const User& other) = default;
// User::User(const User& other)
//     : _id(other._id),
//       _first_name(other._first_name),
//       _last_name(other._last_name),
//       _birth_day(other._birth_day),
//       _address(other._address),
//       _citizen_id(other._citizen_id),
//       _password_hash(other._password_hash), // Sao chép hash và salt đã có
//       _salt(other._salt),
//       _role(other._role)
// {}

User& User::operator=(const User& other) = default;
// Hoặc triển khai thủ công
// User& User::operator=(const User& other) {
//     if (this != &other) { // Tránh tự gán
//         _id = other._id;
//         _first_name = other._first_name;
//         _last_name = other._last_name;
//         _birth_day = other._birth_day;
//         _address = other._address;
//         _citizen_id = other._citizen_id;
//         _password_hash = other._password_hash;
//         _salt = other._salt;
//         _role = other._role;
//     }
//     return *this;
// }


void User::setPasswordHash(const std::string& passwordHash) {
    _password_hash = passwordHash;
}

void User::setSalt(const std::string& salt) {
    _salt = salt;
}

// --- Triển khai các Getters ---
const std::string& User::id() const { return _id; }
const std::string& User::firstName() const { return _first_name; }
const std::string& User::lastName() const { return _last_name; }
std::string User::fullName() const { return _first_name + " " + _last_name; }
Birthday User::birthday() { return _birth_day; } // Trả về bản sao

// Changed return type from reference to value to avoid returning a reference to a temporary
std::string User::Birthday() const { return _birth_day.birthday(); } // Trả về chuỗi định dạng "dd/mm/yyyy"

const std::string& User::address() const { return _address; }
const std::string& User::citizenID() const { return _citizen_id; }
const std::string& User::passwordHash() const { return _password_hash; }
const std::string& User::salt() const { return _salt; } // Getter cho salt
UserRole User::role() const { return _role; }
const std::string& User::email() const { return _email; }
const std::string& User::phoneNumber() const { return _phone_number; }

// --- Triển khai các Setters khác ---
void User::setFirstName(const std::string& firstName) { _first_name = firstName; }
void User::setLastName(const std::string& lastName) { _last_name = lastName; }
void User::setCitizenId(const std::string& citizenId) { _citizen_id = citizenId; }
void User::setAddress(const std::string& address) { _address = address; }
void User::setBirthday(int day, int month, int year) { _birth_day.setBirthday(day, month, year); }
void User::setBirthday(const std::string& date) { _birth_day.setBirthday(date); }
void User::setEmail(const std::string& email) { _email = email; } // Setter cho email
void User::setPhoneNumber(const std::string& phoneNumber) { _phone_number = phoneNumber; } // Setter cho số điện thoại

// Hàm display() là virtual thuần túy nên không có định nghĩa trong lớp User.cpp
// Các lớp con (Student, Teacher/Faculty) PHẢI định nghĩa hàm display() của riêng chúng.

// --- Triển khai lớp Birthday (nên nằm trong Birthday.cpp) ---
Birthday::Birthday() : _day(1), _month(1), _year(1) {} // Constructor mặc định

Birthday::Birthday(int day, int month, int year) {
    _day = day;
    _month = month;
    _year = year;
}

bool Birthday::setBirthday(int day, int month, int year) {
    // Kiểm tra tính hợp lệ của ngày tháng năm
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900) {
        return false; // Ngày tháng năm không hợp lệ
    }

    // Kiểm tra số ngày trong tháng
    if ((month == 2 && day > 29) || // Tháng 2 không có 30 hoặc 31
        (month == 4 && day > 30) || // Tháng 4 không có 31
        (month == 6 && day > 30) || // Tháng 6 không có 31
        (month == 9 && day > 30) || // Tháng 9 không có 31
        (month == 11 && day > 30)) { // Tháng 11 không có 31
        return false; // Ngày tháng năm không hợp lệ
    }

    // Kiểm tra năm nhuận cho tháng 2
    if (month == 2 && day == 29) {
        if ((year % 4 != 0) || (year % 100 == 0 && year % 400 != 0)) {
            return false; // Năm không phải năm nhuận
        }
    }

     _day = day;
    _month = month;
    _year = year;
    return true; // Ngày tháng năm hợp lệ
}

bool Birthday::setBirthday(std::string date) {
    // Giả sử định dạng ngày tháng năm là "dd/mm/yyyy"
    std::istringstream ss(date);
    std::string token;
    int day, month, year;

    // Tách chuỗi theo dấu "/"
    std::getline(ss, token, '/');
    day = std::stoi(token);
    std::getline(ss, token, '/');
    month = std::stoi(token);
    std::getline(ss, token, '/');
    year = std::stoi(token);

    return setBirthday(day, month, year); // Gọi hàm setBirthday(int, int, int)
}

std::string Birthday::birthday() const {
    // Có thể format đẹp hơn (thêm số 0 nếu cần)
    return std::to_string(_day) + "/" + std::to_string(_month) + "/" + std::to_string(_year);
}
int Birthday::getDay() const { return _day; }
int Birthday::getMonth() const { return _month; }
int Birthday::getYear() const { return _year; }
