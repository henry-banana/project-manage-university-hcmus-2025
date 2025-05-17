#ifndef VALIDATIONRESULT_H
#define VALIDATIONRESULT_H

#include <vector>
#include <string>
#include "ErrorType.h" // Để dùng struct Error

struct ValidationResult {
    bool isValid = true;
    std::vector<Error> errors;

    void addError(int code, const std::string& message);

    void addError(const Error& error);

    // Helper để nối các message lỗi thành một chuỗi
    std::string getErrorMessagesCombined(const std::string& separator = "\n") const;
};

#endif // VALIDATIONRESULT_H