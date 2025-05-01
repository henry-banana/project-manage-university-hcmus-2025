#ifndef CONSOLEUTILS_H
#define CONSOLEUTILS_H

#include <string>
#include <vector>

namespace Utils {
    void clearScreen();
    void pauseExecution(const std::string& message = "Press Enter to continue...");

    // Hàm vẽ khung (có thể phức tạp hơn để dùng các ký tự đặc biệt)
    void drawHeaderBox(const std::string& title);
    void drawSimpleBox(const std::vector<std::string>& content);

    // Hàm đặt vị trí con trỏ (phụ thuộc hệ điều hành) - ví dụ
    // void gotoxy(int x, int y);

} // namespace Utils

#endif // CONSOLEUTILS_H