#include "ValidationResult.h"

void ValidationResult::addError(int code, const std::string& message) {
    isValid = false;
    errors.emplace_back(code, message);
}

void ValidationResult::addError(const Error& error) {
    isValid = false;
    errors.push_back(error);
}

// Helper để nối các message lỗi thành một chuỗi
std::string ValidationResult::getErrorMessagesCombined(const std::string& separator = "\n") const {
    if (errors.empty()) {
        return "";
    }
    std::string combined;
    for (size_t i = 0; i < errors.size(); ++i) {
        combined += errors[i].message;
        if (i < errors.size() - 1) {
            combined += separator;
        }
    }
    return combined;
}