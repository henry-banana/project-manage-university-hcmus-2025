#include "ConsoleUtils.h"
#include <cstdlib> // For system()
#include <iomanip> // For std::setw

void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    // ANSI escape code for clearing screen
    std::cout << "\033[2J\033[1;1H";
    // Hoặc std::system("clear"); nhưng không portable bằng ANSI escape code
#endif
}

void pauseExecution(const std::string& message) {
    std::cout << message << std::flush;
    // Chờ người dùng nhấn Enter
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (std::cin.gcount() == 0) { // Nếu trước đó không có gì trong buffer (ví dụ sau khi đọc số)
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void drawHeader(const std::string& title, char borderChar, size_t width) {
    if (width < title.length() + 4) { // Đảm bảo đủ chỗ cho title và border
        width = title.length() + 4;
    }
    std::string borderLine(width, borderChar);
    size_t titlePadding = (width - 2 - title.length()) / 2;
    size_t remainderPadding = (width - 2 - title.length()) % 2; // Nếu không chia hết

    std::cout << "\n" << borderLine << std::endl;
    std::cout << borderChar << std::string(titlePadding, ' ') << title 
              << std::string(titlePadding + remainderPadding, ' ') << borderChar << std::endl;
    std::cout << borderLine << std::endl;
}

void drawBox(const std::vector<std::string>& content, char borderChar, size_t width) {
    std::string horizontalBorder = "+";
    for(size_t i = 0; i < width - 2; ++i) horizontalBorder += borderChar;
    horizontalBorder += "+";

    std::cout << horizontalBorder << std::endl;
    for (const std::string& line : content) {
        std::cout << "| " << std::left << std::setw(static_cast<int>(width) - 3) << line << "|" << std::endl;
    }
    std::cout << horizontalBorder << std::endl;
}