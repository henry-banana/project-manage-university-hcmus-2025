#ifndef VALIDATIONRESULT_H
#define VALIDATIONRESULT_H

#include <vector>
#include <string>
#include "ErrorType.h" // Để dùng struct Error

/**
 * @struct ValidationResult
 * @brief Cấu trúc lưu trữ kết quả kiểm tra tính hợp lệ
 * 
 * Cấu trúc này được sử dụng để lưu trữ kết quả kiểm tra tính hợp lệ của dữ liệu,
 * bao gồm trạng thái hợp lệ và danh sách các lỗi (nếu có).
 */
struct ValidationResult {
    bool isValid = true;             ///< Cờ đánh dấu dữ liệu có hợp lệ không
    std::vector<Error> errors;       ///< Danh sách các lỗi phát hiện được

    /**
     * @brief Thêm lỗi mới vào kết quả kiểm tra
     * @param code Mã lỗi
     * @param message Thông điệp mô tả lỗi
     */
    void addError(int code, const std::string& message);

    /**
     * @brief Thêm đối tượng lỗi vào kết quả kiểm tra
     * @param error Đối tượng lỗi cần thêm
     */
    void addError(const Error& error);

    /**
     * @brief Kết hợp tất cả thông điệp lỗi thành một chuỗi
     * @param separator Chuỗi dùng để phân cách giữa các thông điệp lỗi
     * @return Chuỗi kết hợp của tất cả thông điệp lỗi
     */
    std::string getErrorMessagesCombined(const std::string& separator = "\n") const;
};

#endif // VALIDATIONRESULT_H