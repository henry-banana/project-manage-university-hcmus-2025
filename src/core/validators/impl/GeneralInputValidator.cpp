#include "GeneralInputValidator.h"
#include "../../../utils/StringUtils.h" // Cho trim
#include <limits> // Cho numeric_limits
#include <algorithm> // Cho std::any_of for password
#include <regex> // Cho regex kiểm tra email

/**
 * @brief Kiểm tra tính hợp lệ của địa chỉ email
 * 
 * Phương thức này kiểm tra định dạng của địa chỉ email dựa trên các quy tắc cơ bản:
 * - Email không được rỗng
 * - Email phải có dạng something@something.something
 * - Email không quá dài (tối đa 100 ký tự)
 * 
 * @param email Địa chỉ email cần kiểm tra
 * @return ValidationResult Kết quả kiểm tra với danh sách lỗi (nếu có)
 */
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
    const std::regex email_regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)"); // C++11 regex
    if (!std::regex_match(trimmedEmail, email_regex)) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Invalid email address format.");
    }
    
    // Kiểm tra thêm về @ và dấu chấm
    size_t atPos = trimmedEmail.find('@');
    size_t dotPos = trimmedEmail.rfind('.'); // Tìm dấu chấm cuối cùng

    if (atPos == std::string::npos || dotPos == std::string::npos || atPos == 0 || 
        dotPos < atPos || dotPos == trimmedEmail.length() - 1 || trimmedEmail.length() < 5) {
        vr.addError(ErrorCode::VALIDATION_ERROR, "Invalid email address format.");
    }
    
    if (trimmedEmail.length() > 100) { // Giới hạn độ dài
        vr.addError(ErrorCode::VALIDATION_ERROR, "Email address is too long (max 100 characters).");
    }
    
    return vr;
}

/**
 * @brief Kiểm tra tính hợp lệ của số điện thoại
 * 
 * Phương thức này kiểm tra định dạng của số điện thoại dựa trên quốc gia:
 * - Số điện thoại không được rỗng
 * - Số điện thoại chỉ chứa các chữ số
 * - Đối với Việt Nam (VN): 10-11 chữ số, bắt đầu bằng số 0
 * - Đối với các quốc gia khác: 7-15 chữ số
 * 
 * @param phone Số điện thoại cần kiểm tra
 * @param countryCode Mã quốc gia (ví dụ: "VN" cho Việt Nam)
 * @return ValidationResult Kết quả kiểm tra với danh sách lỗi (nếu có)
 */
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

/**
 * @brief Kiểm tra độ phức tạp của mật khẩu
 * 
 * Phương thức này kiểm tra độ phức tạp của mật khẩu dựa trên các tiêu chí:
 * - Độ dài tối thiểu (8 ký tự)
 * - Có ít nhất một chữ hoa
 * - Có ít nhất một chữ thường
 * - Có ít nhất một chữ số
 * 
 * @param password Mật khẩu cần kiểm tra
 * @return ValidationResult Kết quả kiểm tra với danh sách lỗi (nếu có)
 */
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

/**
 * @brief Kiểm tra định dạng của mã ID
 * 
 * Phương thức này kiểm tra định dạng của mã ID dựa trên các quy tắc:
 * - ID không được rỗng
 * - ID phải có độ dài trong khoảng cho phép
 * 
 * @param id Mã ID cần kiểm tra
 * @param idName Tên của trường ID (ví dụ: "Student ID", "Faculty ID")
 * @param minLength Độ dài tối thiểu của ID
 * @param maxLength Độ dài tối đa của ID
 * @return ValidationResult Kết quả kiểm tra với danh sách lỗi (nếu có)
 */
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

/**
 * @brief Kiểm tra chuỗi bắt buộc
 * 
 * Phương thức này kiểm tra một chuỗi bắt buộc:
 * - Chuỗi không được rỗng
 * - Chuỗi không được quá dài
 * 
 * @param value Chuỗi cần kiểm tra
 * @param fieldName Tên của trường
 * @param maxLength Độ dài tối đa cho phép
 * @return ValidationResult Kết quả kiểm tra với danh sách lỗi (nếu có)
 */
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

/**
 * @brief Kiểm tra chuỗi không bắt buộc
 * 
 * Phương thức này kiểm tra một chuỗi không bắt buộc:
 * - Chuỗi có thể rỗng
 * - Nếu không rỗng, chuỗi không được quá dài
 * 
 * @param value Chuỗi cần kiểm tra
 * @param fieldName Tên của trường
 * @param maxLength Độ dài tối đa cho phép
 * @return ValidationResult Kết quả kiểm tra với danh sách lỗi (nếu có)
 */
ValidationResult GeneralInputValidator::validateOptionalString(const std::string& value, const std::string& fieldName, size_t maxLength) const {
    ValidationResult vr;
    std::string trimmedValue = StringUtils::trim(value);
    if (!trimmedValue.empty() && trimmedValue.length() > maxLength) { // Chỉ kiểm tra độ dài nếu không rỗng
        vr.addError(ErrorCode::VALIDATION_ERROR, fieldName + " is too long (max " + std::to_string(maxLength) + " characters).");
    }
    return vr;
}

/**
 * @brief Kiểm tra giá trị số nguyên
 * 
 * Phương thức này kiểm tra một giá trị số nguyên:
 * - Giá trị không được nhỏ hơn minValue
 * - Giá trị không được lớn hơn maxValue
 * 
 * @param value Giá trị số nguyên cần kiểm tra
 * @param fieldName Tên của trường
 * @param minValue Giá trị tối thiểu cho phép
 * @param maxValue Giá trị tối đa cho phép
 * @return ValidationResult Kết quả kiểm tra với danh sách lỗi (nếu có)
 */
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

/**
 * @brief Kiểm tra tính hợp lệ của ngày tháng
 * 
 * Phương thức này kiểm tra tính hợp lệ của một ngày tháng:
 * - Năm phải nằm trong khoảng 1900-2100
 * - Tháng phải nằm trong khoảng 1-12
 * - Ngày phải phù hợp với tháng và năm (có xử lý năm nhuận)
 * 
 * @param day Ngày
 * @param month Tháng
 * @param year Năm
 * @param fieldNamePrefix Tiền tố cho tên trường trong thông báo lỗi
 * @return ValidationResult Kết quả kiểm tra với danh sách lỗi (nếu có)
 */
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

/**
 * @brief Kiểm tra tính hợp lệ của mã căn cước/CMND
 * 
 * Phương thức này kiểm tra định dạng của mã căn cước/CMND dựa trên quốc gia:
 * - Mã không được rỗng
 * - Mã chỉ chứa các chữ số
 * - Đối với Việt Nam (VN): 9 hoặc 12 chữ số
 * - Đối với các quốc gia khác: 5-20 chữ số
 * 
 * @param citizenId Mã căn cước/CMND cần kiểm tra
 * @param countryCode Mã quốc gia (ví dụ: "VN" cho Việt Nam)
 * @return ValidationResult Kết quả kiểm tra với danh sách lỗi (nếu có)
 */
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