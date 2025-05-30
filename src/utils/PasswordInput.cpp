#include "PasswordInput.h"
#include <random>   // For salt generation
#include <algorithm> // For salt generation
#include <iterator> // For salt generation
#include <stdexcept> // For errors
#include <iomanip>
#include <sstream>
#include <iostream>
#include <conio.h>

/**
 * @namespace PasswordUtils
 * @brief Không gian tên chứa các hàm tiện ích xử lý mật khẩu
 * 
 * Namespace này cung cấp các hàm tiện ích để xử lý mật khẩu, bao gồm 
 * tạo salt, băm mật khẩu, xác minh mật khẩu, kiểm tra độ phức tạp
 * và nhập mật khẩu có che dấu.
 */
namespace PasswordUtils { 
    /**
     * @brief Tạo chuỗi salt ngẫu nhiên với độ dài xác định
     * 
     * Salt được sử dụng trong quá trình băm mật khẩu để tăng cường bảo mật
     * bằng cách ngăn chặn các cuộc tấn công bảng băm (rainbow table).
     * 
     * @param length Độ dài của chuỗi salt cần tạo
     * @return std::string Chuỗi salt ngẫu nhiên
     */
    std::string generateSalt(size_t length) {
        const std::string characters = "TDTH@!@PTPC";
        std::random_device random_device;
        std::mt19937 generator(random_device());
        std::uniform_int_distribution<> distribution(0, characters.length() - 1);
    
        std::string salt;
        salt.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            salt += characters[distribution(generator)];
        }
        return salt;
    }
    
    // --- !! PLACEHOLDER HASHING - DO NOT USE IN PRODUCTION !! ---
    /**
     * @brief Băm mật khẩu với chuỗi salt
     * 
     * @warning ĐÂY CHỈ LÀ PLACEHOLDER - KHÔNG SỬ DỤNG TRONG MÔI TRƯỜNG THỰC TẾ
     * Phương thức này sử dụng hàm băm đơn giản (không đủ an toàn cho thực tế).
     * Trong môi trường thực tế, nên sử dụng các hàm tạo khóa như Argon2, bcrypt hoặc scrypt.
     * 
     * @param plainPassword Mật khẩu dạng văn bản thường
     * @param salt Chuỗi salt để kết hợp với mật khẩu
     * @return std::string Chuỗi băm của mật khẩu đã thêm salt
     */
    std::string hashPassword(const std::string& plainPassword, const std::string& salt) {
        // Simple combination (vulnerable to rainbow tables if salt is reused/short)
        std::string saltedPassword = salt + plainPassword;
    
        std::hash<std::string> hashFunction;
        auto hash = hashFunction(saltedPassword); // Hash the salted password
        return std::to_string(hash); // Convert hash to string (not secure, just for example)
    }
    
    /**
     * @brief Xác minh mật khẩu bằng cách so sánh với chuỗi băm đã lưu
     * 
     * @warning ĐÂY CHỈ LÀ PLACEHOLDER - KHÔNG SỬ DỤNG TRONG MÔI TRƯỜNG THỰC TẾ
     * Phương thức này phải khớp với phương thức băm và có cùng các vấn đề bảo mật.
     * 
     * @param plainPassword Mật khẩu dạng văn bản thường cần kiểm tra
     * @param storedHash Chuỗi băm đã lưu trữ
     * @param salt Chuỗi salt đã sử dụng khi băm mật khẩu
     * @return true Nếu mật khẩu chính xác
     * @return false Nếu mật khẩu không chính xác
     */
    bool verifyPassword(const std::string& plainPassword, const std::string& storedHash, const std::string& salt) {
        // Re-hash the provided plain password with the *same* salt
        std::string computedHash = hashPassword(plainPassword, salt);
        // Compare the computed hash with the stored hash
        // Use a constant-time comparison function in production to prevent timing attacks
        return computedHash == storedHash;
    }
    // --- End Placeholder ---
    
    
    /**
     * @brief Kiểm tra độ phức tạp của mật khẩu
     * 
     * Kiểm tra xem mật khẩu có đủ phức tạp không dựa trên các tiêu chí:
     * - Độ dài tối thiểu
     * - Có ít nhất một chữ hoa
     * - Có ít nhất một chữ thường
     * - Có ít nhất một chữ số
     * 
     * @param password Mật khẩu cần kiểm tra
     * @param minLength Độ dài tối thiểu của mật khẩu
     * @return true Nếu mật khẩu đủ phức tạp
     * @return false Nếu mật khẩu không đủ phức tạp
     */
    bool isPasswordComplexEnough(const std::string& password, size_t minLength) {
        if (password.length() < minLength) return false;
        bool hasUpper = false, hasLower = false, hasDigit = false; //, hasSymbol = false;
        for (char c : password) {
            if (isupper(c)) hasUpper = true;
            else if (islower(c)) hasLower = true;
            else if (isdigit(c)) hasDigit = true;
            // else if (ispunct(c) || isgraph(c)) hasSymbol = true; // Check symbols
        }
        // return hasUpper && hasLower && hasDigit && hasSymbol; // Enforce all rules
        return hasUpper && hasLower && hasDigit; // Example: require upper, lower, digit
    }
    
    #ifdef _WIN32
        /**
         * @brief Nhập mật khẩu có che dấu (hiển thị dấu *) - Phiên bản Windows
         * 
         * Phương thức này cho phép người dùng nhập mật khẩu mà không hiển thị 
         * ký tự thực tế, thay vào đó hiển thị dấu * cho mỗi ký tự.
         * 
         * @param prompt Thông báo để hiển thị trước khi nhập mật khẩu
         * @return std::string Mật khẩu người dùng đã nhập
         */
        std::string getMaskedPassword(const std::string& prompt) {
            std::cout << prompt << std::flush;
            std::string password;
            char ch;
            while ((ch = _getch()) != '\r') { // Read characters until Enter ('\r')
                if (ch == '\b') { // Handle backspace
                    if (!password.empty()) {
                        password.pop_back();
                        std::cout << "\b \b"; // Erase the character from console
                    }
                } else if (isprint(ch)) { // Only accept printable characters
                    password += ch;
                    std::cout << '*';
                }
            }
            std::cout << "\n"; // Move to next line after Enter
            return password;
        }
    #else
        /**
         * @brief Nhập mật khẩu có che dấu (hiển thị dấu *) - Phiên bản POSIX (Linux/macOS)
         * 
         * Phương thức này cho phép người dùng nhập mật khẩu mà không hiển thị 
         * ký tự thực tế, thay vào đó hiển thị dấu * cho mỗi ký tự.
         * 
         * @param prompt Thông báo để hiển thị trước khi nhập mật khẩu
         * @return std::string Mật khẩu người dùng đã nhập
         */
        std::string getMaskedPassword(const std::string& prompt) {
            std::cout << prompt << std::flush;
            std::string password;
    
            termios oldt;
            tcgetattr(STDIN_FILENO, &oldt); // Get current terminal settings
            termios newt = oldt;
            newt.c_lflag &= ~ECHO; // Disable echo
            tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new settings
    
            char ch;
             // Read character by character without echoing
             // Using cin.get() after disabling echo
            while (std::cin.get(ch) && ch != '\n') {
                 if (ch == 127 || ch == 8) { // Handle backspace (ASCII 127 or 8)
                    if (!password.empty()) {
                        password.pop_back();
                         // Need to manually handle erasing the '*' on the screen
                         // This part is tricky without more advanced terminal control
                         // Simple version: just update the string, don't update display perfectly for backspace
                         std::cout << "\b \b" << std::flush; // Try basic backspace erase
                    }
                } else if (isprint(ch)) { // Accept printable characters
                    password += ch;
                    std::cout << '*' << std::flush; // Print mask character
                }
                // Ignore other non-printable characters
            }
    
    
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore original settings
    
            std::cout << "\n";
            return password;
        }
    #endif // _WIN32
}

