/**
 * @file PasswordInput.h
 * @brief Định nghĩa các hàm tiện ích liên quan đến mật khẩu
 * 
 * Namespace PasswordUtils cung cấp các hàm tiện ích để nhập, mã hóa, 
 * và xác thực mật khẩu một cách an toàn.
 */
#ifndef PASSWORDINPUT_H
#define PASSWORDINPUT_H

#include <string>

/**
 * @namespace PasswordUtils
 * @brief Namespace chứa các hàm tiện ích liên quan đến mật khẩu
 */
namespace PasswordUtils {
    /**
     * @brief Nhận mật khẩu từ người dùng với việc ẩn ký tự bằng dấu '*'
     * 
     * @note Triển khai phụ thuộc vào nền tảng hệ điều hành
     * @param prompt Thông báo hiển thị khi yêu cầu nhập mật khẩu
     * @return Chuỗi mật khẩu đã nhập
     */
    std::string getMaskedPassword(const std::string& prompt = "Enter Password: ");

    /**
     * @brief Tạo một chuỗi salt ngẫu nhiên
     * 
     * @param length Độ dài của chuỗi salt
     * @return Chuỗi salt đã tạo
     */
    std::string generateSalt(size_t length = 16);

    /**
     * @brief Mã hóa mật khẩu sử dụng salt
     * 
     * @note Triển khai hiện tại chỉ là giữ chỗ. Nên thay thế bằng thuật toán
     *       mã hóa mạnh như Argon2, bcrypt, hoặc scrypt. Không bao giờ sử dụng
     *       các hàm băm đơn giản như SHA-1/MD5 trực tiếp cho mật khẩu.
     * 
     * @param plainPassword Mật khẩu dạng văn bản thường
     * @param salt Chuỗi salt
     * @return Chuỗi mật khẩu đã mã hóa
     */
    std::string hashPassword(const std::string& plainPassword, const std::string& salt);

    /**
     * @brief Xác thực mật khẩu so với hash đã lưu trữ
     * 
     * @note Triển khai hiện tại chỉ là giữ chỗ. Phải khớp với thuật toán mã hóa
     *       được sử dụng trong hàm hashPassword.
     * 
     * @param plainPassword Mật khẩu dạng văn bản thường cần xác thực
     * @param storedHash Chuỗi hash đã lưu trữ
     * @param salt Chuỗi salt đã sử dụng khi mã hóa
     * @return true nếu mật khẩu hợp lệ, false nếu không
     */
    bool verifyPassword(const std::string& plainPassword, const std::string& storedHash, const std::string& salt);

    // Optional: Basic password complexity check
    bool isPasswordComplexEnough(const std::string& password, size_t minLength = 8);

}

#endif // PASSWORDINPUT_H