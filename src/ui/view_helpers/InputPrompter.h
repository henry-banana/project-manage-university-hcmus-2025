/**
 * @file InputPrompter.h
 * @brief Định nghĩa lớp nhận đầu vào từ người dùng qua giao diện console
 * 
 * Lớp InputPrompter cung cấp các phương thức để nhận và xác thực đầu vào
 * từ người dùng qua giao diện console với các kiểu dữ liệu khác nhau.
 */
#ifndef INPUTPROMPTER_H
#define INPUTPROMPTER_H

#include <string>
#include <limits> 
#include <iostream> 
// #include "../../common/ValidationResult.h" // Loại bỏ nếu không dùng trong prompter

/**
 * @class InputPrompter
 * @brief Lớp nhận đầu vào từ người dùng với nhiều kiểu dữ liệu khác nhau
 * 
 * Lớp này cung cấp các phương thức để nhận và xác thực đầu vào từ người dùng
 * với các kiểu dữ liệu như chuỗi, số nguyên, số thực, và lựa chọn yes/no.
 */
class InputPrompter {
public:
    /**
     * @brief Hàm khởi tạo mặc định
     */
    InputPrompter() = default;

    /**
     * @brief Nhận chuỗi từ người dùng
     * @param promptMessage Thông báo hiển thị khi yêu cầu nhập
     * @param allowEmpty Cho phép chuỗi rỗng hay không
     * @return Chuỗi đã nhập
     */
    std::string promptForString(const std::string& promptMessage, bool allowEmpty = false) const;
    
    /**
     * @brief Nhận số nguyên kiểu long từ người dùng
     * @param promptMessage Thông báo hiển thị khi yêu cầu nhập
     * @param minVal Giá trị tối thiểu cho phép
     * @param maxVal Giá trị tối đa cho phép
     * @return Số nguyên kiểu long đã nhập
     */
    long promptForLong(const std::string& promptMessage, long minVal = std::numeric_limits<long>::min(), long maxVal = std::numeric_limits<long>::max()) const;
    
    /**
     * @brief Nhận số nguyên kiểu int từ người dùng
     * @param promptMessage Thông báo hiển thị khi yêu cầu nhập
     * @param minVal Giá trị tối thiểu cho phép
     * @param maxVal Giá trị tối đa cho phép
     * @return Số nguyên kiểu int đã nhập
     */
    int promptForInt(const std::string& promptMessage, int minVal = std::numeric_limits<int>::min(), int maxVal = std::numeric_limits<int>::max()) const;
    
    /**
     * @brief Nhận số thực từ người dùng
     * @param promptMessage Thông báo hiển thị khi yêu cầu nhập
     * @param minVal Giá trị tối thiểu cho phép
     * @param maxVal Giá trị tối đa cho phép
     * @return Số thực đã nhập
     */
    double promptForDouble(const std::string& promptMessage, double minVal = std::numeric_limits<double>::lowest(), double maxVal = std::numeric_limits<double>::max()) const;
    
    /**
     * @brief Nhận lựa chọn yes/no từ người dùng
     * @param promptMessage Thông báo hiển thị khi yêu cầu nhập
     * @param yesChar Ký tự đại diện cho lựa chọn "yes"
     * @param noChar Ký tự đại diện cho lựa chọn "no"
     * @return true nếu người dùng chọn "yes", false nếu chọn "no"
     */
    bool promptForYesNo(const std::string& promptMessage, char yesChar = 'y', char noChar = 'n') const;
    
    /**
     * @brief Nhận mật khẩu từ người dùng
     * @param promptMessage Thông báo hiển thị khi yêu cầu nhập
     * @param masked Có ẩn mật khẩu khi nhập không
     * @return Mật khẩu đã nhập
     */
    std::string promptForPassword(const std::string& promptMessage, bool masked = true) const;
    
    /**
     * @brief Nhận địa chỉ email từ người dùng
     * @param promptMessage Thông báo hiển thị khi yêu cầu nhập
     * @return Địa chỉ email đã nhập
     */
    std::string promptForEmail(const std::string& promptMessage) const;
};

#endif