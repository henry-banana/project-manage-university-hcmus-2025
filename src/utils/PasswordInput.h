#ifndef PASSWORDINPUT_H
#define PASSWORDINPUT_H

#include <string>

// Namespace hoặc lớp static cho tiện ích nhập password
namespace Utils {
    // Nhập password từ console, hiển thị dấu '*'
    // maxLen: Độ dài tối đa của password
    std::string getMaskedPassword(int maxLen = 10);
} // namespace Utils

#endif // PASSWORDINPUT_H