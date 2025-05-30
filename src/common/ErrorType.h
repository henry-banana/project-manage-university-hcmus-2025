#ifndef ERRORTYPE_H
#define ERRORTYPE_H

#include <string>

/**
 * @struct Error
 * @brief Cấu trúc lưu trữ thông tin về lỗi
 * 
 * Cấu trúc này được sử dụng trong toàn bộ ứng dụng để biểu diễn các lỗi
 * với mã lỗi và thông điệp mô tả.
 */
struct Error {
    int code; // Mã lỗi, ví dụ 404 (Not Found), 400 (Bad Request), 500 (Server Error)
    std::string message;

    /**
     * @brief Constructor tạo đối tượng Error
     * 
     * @param c Mã lỗi
     * @param msg Thông điệp mô tả lỗi
     */
    Error(int c, std::string msg) : code(c), message(std::move(msg)) {}
};

/**
 * @namespace ErrorCode
 * @brief Không gian tên chứa các hằng số định nghĩa các mã lỗi
 * 
 * Namespace này cung cấp các hằng số đại diện cho các loại lỗi khác nhau
 * trong ứng dụng, được phân nhóm theo chức năng và mục đích.
 */
namespace ErrorCode {
    // Lỗi chung
    constexpr int VALIDATION_ERROR = 1001;
    constexpr int NOT_FOUND = 1002;
    constexpr int ALREADY_EXISTS = 1003;
    constexpr int DATA_ACCESS_ERROR = 1004;
    constexpr int AUTHENTICATION_FAILED = 1005;
    constexpr int PERMISSION_DENIED = 1006;
    constexpr int OPERATION_FAILED = 1007; // Lỗi chung
    constexpr int PARSING_ERROR = 1008; // Lỗi phân tích cú pháp
    
    // Lỗi dữ liệu thực thể
    constexpr int INVALID_STUDENT_DATA = 1101;
    constexpr int INVALID_TEACHER_DATA = 1102;
    constexpr int INVALID_COURSE_DATA = 1103;
    constexpr int INVALID_ENROLLMENT_DATA = 1104;
    constexpr int INVALID_RESULT_DATA = 1105;
    constexpr int INVALID_FEE_DATA = 1106; 
    constexpr int INVALID_SALARY_DATA = 1107;

    // Lỗi liên quan đến khóa học
    constexpr int COURSE_ENROLLMENT_CLOSED = 1201;
    constexpr int COURSE_PREREQUISITE_NOT_MET = 1202;
    constexpr int COURSE_CAPACITY_REACHED = 1203;
    constexpr int STUDENT_ALREADY_ENROLLED = 1204;
    constexpr int FEE_PAYMENT_REQUIRED = 1205;
    constexpr int GRADING_PERIOD_CLOSED = 1206;

    // Lỗi liên quan đến file
    constexpr int FILE_NOT_FOUND = 1301;
    constexpr int FILE_ACCESS_DENIED = 1302;
    constexpr int FILE_FORMAT_ERROR = 1303;
    constexpr int FILE_WRITE_ERROR = 1304;
    constexpr int CSV_PARSING_ERROR = 1305;

    // Lỗi liên quan đến cơ sở dữ liệu
    constexpr int DB_CONNECTION_ERROR = 1401;
    constexpr int DB_QUERY_ERROR = 1402;
    constexpr int DB_TRANSACTION_ERROR = 1403;
    constexpr int DB_CONSTRAINT_ERROR = 1404;
    constexpr int DB_FOREIGN_KEY_ERROR = 1405;
    
    // Lỗi liên quan đến giao dịch
    constexpr int TRANSACTION_BEGIN_FAILED = 1501;
    constexpr int TRANSACTION_COMMIT_FAILED = 1502;
    constexpr int TRANSACTION_ROLLBACK_FAILED = 1503;
    constexpr int TRANSACTION_NESTED_ERROR = 1504;

    // Lỗi liên quan đến ràng buộc dữ liệu
    constexpr int CONSTRAINT_VIOLATION = 1601;
}

#endif // ERRORTYPE_H