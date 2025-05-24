#include "ValidationResult.h"

/**
 * @brief Thêm lỗi mới vào kết quả kiểm tra
 * 
 * Phương thức này thêm một lỗi mới vào danh sách lỗi và đánh dấu
 * kết quả kiểm tra là không hợp lệ.
 * 
 * @param code Mã lỗi
 * @param message Thông điệp mô tả lỗi
 */
void ValidationResult::addError(int code, const std::string& message) {
    isValid = false;
    errors.emplace_back(code, message);
}

/**
 * @brief Thêm đối tượng lỗi vào kết quả kiểm tra
 * 
 * Phương thức này thêm một đối tượng Error vào danh sách lỗi
 * và đánh dấu kết quả kiểm tra là không hợp lệ.
 * 
 * @param error Đối tượng lỗi cần thêm
 */
void ValidationResult::addError(const Error& error) {
    isValid = false;
    errors.push_back(error);
}

/**
 * @brief Kết hợp tất cả thông điệp lỗi thành một chuỗi
 * 
 * Phương thức này kết hợp tất cả các thông điệp lỗi trong danh sách
 * thành một chuỗi duy nhất, với ký tự phân cách giữa các thông điệp.
 * 
 * @param separator Chuỗi dùng để phân cách giữa các thông điệp lỗi
 * @return std::string Chuỗi kết hợp của tất cả thông điệp lỗi
 */
std::string ValidationResult::getErrorMessagesCombined(const std::string& separator) const {
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