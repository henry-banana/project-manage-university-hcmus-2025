#include "GeneralInputValidator.h"
#include "../../../utils/StringUtils.h" // Cho trim
#include <limits> // Cho numeric_limits
#include <algorithm> // Cho std::any_of for password

ValidationResult GeneralInputValidator::validateEmail(const std::string& email) const {
    ValidationResult vr;
    std::string trimmedEmail = StringUtils::trim(email);

    if (trimmedEmail.empty()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Email address cannot be empty.");
        return vr;
    }
    // Regex này khá cơ bản, có thể cần một regex phức tạp hơn cho production
    // Tham khảo: RFC 5322 nhưng nó rất phức tạp.
    // Regex đơn giản: something@something.something
    // const std::regex emailRegex(R"(^[\w-\.]+@([\w-]+\.)+[\w-]{2,4}$)"); // C++11
    // Đơn giản hơn nữa để tránh lỗi biên dịch regex trên một số compiler cũ/config thiếu:
    size_t atPos = trimmedEmail.find('@');
    size_t dotPos = trimmedEmail.rfind('.'); // Tìm dấu chấm cuối cùng

    if (atPos == std::string::npos || dotPos == std::string::npos || atPos == 0 || dotPos < atPos || dotPos == trimmedEmail.length() - 1) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Invalid email address format.");
    }
    if (trimmedEmail.length() > 100) { // Giới hạn độ dài
         vr.addError(ErrorCode::VALIDATION_ERROR, "Email address is too long (max 100 characters).");
    }
    return vr;
}

ValidationResult GeneralInputValidator::validatePhoneNumber(const std::string& phone, const std::string& countryCode) const {
    ValidationResult vr;
    std::string trimmedPhone = StringUtils::trim(phone);

    if (trimmedPhone.empty()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Phone number cannot be empty.");
        return vr;
    }

    bool allDigits = true;
    for (char c : trimmedPhone) {
        if (!isdigit(c)) {
            allDigits = false;
            break;
        }
    }
    if (!allDigits) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Phone number must contain only digits.");
    }

    if (countryCode == "VN") { // Ví dụ cho Việt Nam
        if (trimmedPhone.length() < 10 || trimmedPhone.length() > 11) { // VD: 09... (10 số), 01... (11 số - cũ)
            vr.addError(ErrorCode::VALIDATION_ERROR, "Vietnamese phone number must be 10 or 11 digits long.");
        }
        if (trimmedPhone[0] != '0') {
            vr.addError(ErrorCode::VALIDATION_ERROR, "Vietnamese phone number must start with 0.");
        }
    } else {
        // General check for other countries (có thể đơn giản hơn)
        if (trimmedPhone.length() < 7 || trimmedPhone.length() > 15) {
            vr.addError(ErrorCode::VALIDATION_ERROR, "Phone number length seems invalid.");
        }
    }
    return vr;
}

ValidationResult GeneralInputValidator::validatePasswordComplexity(const std::string& password) const {
    ValidationResult vr;
    const size_t minLength = 8;

    if (password.length() < minLength) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Password must be at least " + std::to_string(minLength) + " characters long.");
    }

    bool hasUpper = std::any_of(password.begin(), password.end(), ::isupper);
    bool hasLower = std::any_of(password.begin(), password.end(), ::islower);
    bool hasDigit = std::any_of(password.begin(), password.end(), ::isdigit);
    // bool hasSymbol = std::any_of(password.begin(), password.end(), [](char c){ return std::ispunct(c) || (std::isgraph(c) && !std::isalnum(c)); });


    if (!hasUpper) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Password must contain at least one uppercase letter.");
    }
    if (!hasLower) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Password must contain at least one lowercase letter.");
    }
    if (!hasDigit) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Password must contain at least one digit.");
    }
    // if (!hasSymbol) {
    //     vr.addError(ErrorCode::VALIDATION_ERROR, "Password must contain at least one special symbol.");
    // }
    return vr;
}

ValidationResult GeneralInputValidator::validateIdFormat(const std::string& id, const std::string& idName, size_t minLength, size_t maxLength) const {
    ValidationResult vr;
    std::string trimmedId = StringUtils::trim(id);
    if (trimmedId.empty()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, idName + " cannot be empty.");
    } else {
        if (trimmedId.length() < minLength) {
            vr.addError(ErrorCode::VALIDATION_ERROR, idName + " is too short (min " + std::to_string(minLength) + " characters).");
        }
        if (trimmedId.length() > maxLength) {
            vr.addError(ErrorCode::VALIDATION_ERROR, idName + " is too long (max " + std::to_string(maxLength) + " characters).");
        }
        // Kiểm tra ký tự không hợp lệ (ví dụ: chỉ cho phép chữ và số)
        // bool validChars = std::all_of(trimmedId.begin(), trimmedId.end(), ::isalnum);
        // if (!validChars) {
        //    vr.addError(ErrorCode::VALIDATION_ERROR, idName + " contains invalid characters (only alphanumeric allowed).");
        // }
    }
    return vr;
}

ValidationResult GeneralInputValidator::validateRequiredString(const std::string& value, const std::string& fieldName, size_t maxLength) const {
    ValidationResult vr;
    std::string trimmedValue = StringUtils::trim(value);
    if (trimmedValue.empty()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, fieldName + " cannot be empty.");
    } else if (trimmedValue.length() > maxLength) {
        vr.addError(ErrorCode::VALIDATION_ERROR, fieldName + " is too long (max " + std::to_string(maxLength) + " characters).");
    }
    return vr;
}

ValidationResult GeneralInputValidator::validateOptionalString(const std::string& value, const std::string& fieldName, size_t maxLength) const {
    ValidationResult vr;
    std::string trimmedValue = StringUtils::trim(value);
    if (!trimmedValue.empty() && trimmedValue.length() > maxLength) { // Chỉ kiểm tra độ dài nếu không rỗng
        vr.addError(ErrorCode::VALIDATION_ERROR, fieldName + " is too long (max " + std::to_string(maxLength) + " characters).");
    }
    return vr;
}

ValidationResult GeneralInputValidator::validateInteger(long long value, const std::string& fieldName, long long minValue, long long maxValue) const {
    ValidationResult vr;
    if (value < minValue) {
        vr.addError(ErrorCode::VALIDATION_ERROR, fieldName + " cannot be less than " + std::to_string(minValue) + ".");
    }
    if (value > maxValue) {
        vr.addError(ErrorCode::VALIDATION_ERROR, fieldName + " cannot be greater than " + std::to_string(maxValue) + ".");
    }
    return vr;
}

ValidationResult GeneralInputValidator::validateDate(int day, int month, int year, const std::string& fieldNamePrefix) const {
    ValidationResult vr;
    if (year < 0 || year > 2025) { // Giới hạn năm hợp lý
        vr.addError(ErrorCode::VALIDATION_ERROR, fieldNamePrefix + " year (" + std::to_string(year) + ") is out of valid range (1900-2100).");
    }
    if (month < 1 || month > 12) {
        vr.addError(ErrorCode::VALIDATION_ERROR, fieldNamePrefix + " month (" + std::to_string(month) + ") must be between 1 and 12.");
    }
    if (day < 1 || day > 31) { // Check cơ bản, check kỹ hơn theo tháng
        vr.addError(ErrorCode::VALIDATION_ERROR, fieldNamePrefix + " day (" + std::to_string(day) + ") must be between 1 and 31.");
    } else {
        int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) { // Leap year
            daysInMonth[2] = 29;
        }
        if (month >=1 && month <=12 && day > daysInMonth[month]) {
             vr.addError(ErrorCode::VALIDATION_ERROR, fieldNamePrefix + " day (" + std::to_string(day) + ") is invalid for month " + std::to_string(month) + " in year " + std::to_string(year) + ".");
        }
    }
    return vr;
}

ValidationResult GeneralInputValidator::validateCitizenId(const std::string& citizenId, const std::string& countryCode) const {
    ValidationResult vr;
    std::string trimmedId = StringUtils::trim(citizenId);

    if (trimmedId.empty()) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID cannot be empty.");
        return vr;
    }

    bool allDigits = true;
    for (char c : trimmedId) {
        if (!isdigit(c)) {
            allDigits = false;
            break;
        }
    }
    if (!allDigits) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID must contain only digits.");
    }

    if (countryCode == "VN") {
        if (trimmedId.length() != 9 && trimmedId.length() != 12) {
            vr.addError(ErrorCode::VALIDATION_ERROR, "Vietnamese Citizen ID must be 9 or 12 digits long.");
        }
    } else {
        // Có thể thêm rule cho các quốc gia khác hoặc rule chung chung hơn
        if (trimmedId.length() < 5 || trimmedId.length() > 20) {
             vr.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID length seems invalid for the specified country code.");
        }
    }
    return vr;
}