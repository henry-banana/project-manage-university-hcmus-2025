#include "ValidationResult.h"
#include <sstream> // Required for std::ostringstream in getErrorMessagesCombined

void ValidationResult::addError(int code, const std::string& message) {
    isValid = false;
    errors.emplace_back(code, message);
}

void ValidationResult::addError(const Error& error) {
    isValid = false;
    errors.push_back(error);
}

std::string ValidationResult::getErrorMessagesCombined(const std::string& separator) const {
    if (errors.empty()) {
        return "";
    }
    std::ostringstream oss;
    for (size_t i = 0; i < errors.size(); ++i) {
        oss << errors[i].message;
        if (i < errors.size() - 1) {
            oss << separator;
        }
    }
    return oss.str();
}