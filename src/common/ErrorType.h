#ifndef ERRORTYPE_H
#define ERRORTYPE_H

#include <string>

struct Error {
    int code; // Mã lỗi, ví dụ 404 (Not Found), 400 (Bad Request), 500 (Server Error)
    std::string message;

    // Constructor tiện lợi
    Error(int c, std::string msg) : code(c), message(std::move(msg)) {}
};

// Định nghĩa sẵn một số mã lỗi phổ biến (tùy chọn)
namespace ErrorCode {
    constexpr int VALIDATION_ERROR = 1001;
    constexpr int NOT_FOUND = 1002;
    constexpr int ALREADY_EXISTS = 1003;
    constexpr int DATA_ACCESS_ERROR = 1004;
    constexpr int AUTHENTICATION_FAILED = 1005;
    constexpr int PERMISSION_DENIED = 1006;
    constexpr int OPERATION_FAILED = 1007; // Lỗi chung
}

#endif // ERRORTYPE_H