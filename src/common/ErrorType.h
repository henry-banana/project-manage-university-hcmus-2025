#ifndef ERROR_TYPE_H
#define ERROR_TYPE_H

#include <string>
#include <vector> // Để Validator có thể trả về nhiều lỗi

struct Error {
    int code; // Mã lỗi (ví dụ: 404 - Not Found, 400 - Bad Request, 500 - Internal Server Error)
    std::string message; // Mô tả lỗi chi tiết
};

struct ValidationResult {
    bool isValid = true;
    std::vector<Error> errors;

    void addError(int code, const std::string& message) {
        isValid = false;
        errors.push_back({code, message});
    }
};

#endif // ERROR_TYPE_H