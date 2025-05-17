#ifndef INPUTPROMPTER_H
#define INPUTPROMPTER_H

#include <string>
#include <limits> // For numeric_limits
#include <iostream> // For cin, cout
#include "../../common/ValidationResult.h" // (Optional) để trả về lỗi input

class InputPrompter {
public:
    InputPrompter() = default;

    std::string promptForString(const std::string& promptMessage, bool allowEmpty = false) const;
    long promptForLong(const std::string& promptMessage, long minVal = std::numeric_limits<long>::min(), long maxVal = std::numeric_limits<long>::max()) const;
    int promptForInt(const std::string& promptMessage, int minVal = std::numeric_limits<int>::min(), int maxVal = std::numeric_limits<int>::max()) const;
    double promptForDouble(const std::string& promptMessage, double minVal = std::numeric_limits<double>::lowest(), double maxVal = std::numeric_limits<double>::max()) const;
    bool promptForYesNo(const std::string& promptMessage, char yesChar = 'y', char noChar = 'n') const;
    std::string promptForPassword(const std::string& promptMessage) const; // Sẽ gọi PasswordUtils
    std::string promptForEmail(const std::string& promptMessage) const; // Có thể tích hợp GeneralInputValidator
};

#endif