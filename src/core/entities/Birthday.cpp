#include "Birthday.h"
#include <sstream>  // For std::istringstream
#include <iomanip>  // For std::setfill, std::setw
#include <stdexcept> // For potential exceptions in parsing (though we return bool)

Birthday::Birthday() : _day(0), _month(0), _year(0), _isSet(false) {}

Birthday::Birthday(int day, int month, int year) : _isSet(false) {
    set(day, month, year);
}

bool Birthday::set(int day, int month, int year) {
    // Basic range check
    if (year < 0 || month < 1 || month > 12 || day < 1) {
        _isSet = false;
        return false;
    }

    // Days in month check
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // Leap year check for February
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonth[2] = 29;
    }

    if (day > daysInMonth[month]) {
        _isSet = false;
        return false;
    }

    _day = day;
    _month = month;
    _year = year;
    _isSet = true;
    return true;
}

bool Birthday::setFromString(const std::string& dateStr_ddmmyyyy, char delimiter) {
    std::istringstream ss(dateStr_ddmmyyyy);
    std::string segment;
    int d, m, y;

    // Parse day
    if (!std::getline(ss, segment, delimiter)) { _isSet = false; return false; }
    try { d = std::stoi(segment); } catch (const std::exception&) { _isSet = false; return false; }

    // Parse month
    if (!std::getline(ss, segment, delimiter)) { _isSet = false; return false; }
    try { m = std::stoi(segment); } catch (const std::exception&) { _isSet = false; return false; }

    // Parse year
    if (!std::getline(ss, segment)) { _isSet = false; return false; } // No delimiter for last part
    try { y = std::stoi(segment); } catch (const std::exception&) { _isSet = false; return false; }

    return set(d, m, y);
}

int Birthday::getDay() const { return _isSet ? _day : 0; }
int Birthday::getMonth() const { return _isSet ? _month : 0; }
int Birthday::getYear() const { return _isSet ? _year : 0; }
bool Birthday::isSet() const { return _isSet; }

std::string Birthday::toString_ddmmyyyy(char delimiter) const {
    if (!_isSet) {
        return "Not Set";
    }
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << _day << delimiter
        << std::setfill('0') << std::setw(2) << _month << delimiter
        << _year;
    return oss.str();
}

ValidationResult Birthday::validate() const {
    ValidationResult result;
    if (!_isSet) {
        // Coi như không set là không valid, hoặc tùy logic
        // result.addError(ErrorCode::VALIDATION_ERROR, "Birthday is not set.");
        return result; // Hoặc trả về valid nếu "not set" là chấp nhận được
    }

    // Các check đã có trong hàm set(), gọi lại set để tận dụng
    Birthday temp;
    if (!temp.set(_day, _month, _year)) { // Nếu ngày hiện tại không hợp lệ khi set lại
        result.addError(ErrorCode::VALIDATION_ERROR, "Invalid date values for birthday.");
    }
    // Thêm các rule khác nếu cần, ví dụ năm sinh không được ở tương lai
    // time_t t = time(0);   // get time now
    // tm* now = localtime(&t);
    // if (_year > (now->tm_year + 1900)) {
    //     result.addError(ErrorCode::VALIDATION_ERROR, "Birth year cannot be in the future.");
    // }

    return result;
}