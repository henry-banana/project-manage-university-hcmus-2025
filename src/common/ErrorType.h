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
    constexpr int PARSING_ERROR = 1008; // Lỗi phân tích cú pháp
    
    constexpr int INVALID_STUDENT_DATA = 1101;
    constexpr int INVALID_TEACHER_DATA = 1102;
    constexpr int INVALID_COURSE_DATA = 1103;
    constexpr int INVALID_ENROLLMENT_DATA = 1104;
    constexpr int INVALID_RESULT_DATA = 1105;
    constexpr int INVALID_FEE_DATA = 1106; 
    constexpr int INVALID_SALARY_DATA = 1107;

    constexpr int COURSE_ENROLLMENT_CLOSED = 1201;
    constexpr int COURSE_PREREQUISITE_NOT_MET = 1202;
    constexpr int COURSE_CAPACITY_REACHED = 1203;
    constexpr int STUDENT_ALREADY_ENROLLED = 1204;
    constexpr int FEE_PAYMENT_REQUIRED = 1205;
    constexpr int GRADING_PERIOD_CLOSED = 1206;

    constexpr int FILE_NOT_FOUND = 1301;
    constexpr int FILE_ACCESS_DENIED = 1302;
    constexpr int FILE_FORMAT_ERROR = 1303;
    constexpr int FILE_WRITE_ERROR = 1304;
    constexpr int CSV_PARSING_ERROR = 1305;

    constexpr int DB_CONNECTION_ERROR = 1401;
    constexpr int DB_QUERY_ERROR = 1402;
    constexpr int DB_TRANSACTION_ERROR = 1403;
    constexpr int DB_CONSTRAINT_ERROR = 1404;
    constexpr int DB_FOREIGN_KEY_ERROR = 1405;
    
    constexpr int TRANSACTION_BEGIN_FAILED = 1501;
    constexpr int TRANSACTION_COMMIT_FAILED = 1502;
    constexpr int TRANSACTION_ROLLBACK_FAILED = 1503;
    constexpr int TRANSACTION_NESTED_ERROR = 1504;
}

#endif // ERRORTYPE_H