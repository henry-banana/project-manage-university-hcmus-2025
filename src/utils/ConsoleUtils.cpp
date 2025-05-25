#include "ConsoleUtils.h"
#include <cstdlib> // For system()
#include <iomanip> // For std::setw

/**
 * @brief Xóa màn hình console
 * 
 * Hàm này sử dụng lệnh xóa màn hình phù hợp với hệ điều hành,
 * "cls" cho Windows và ANSI escape code cho các hệ điều hành khác.
 */
void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    // ANSI escape code for clearing screen
    std::cout << "\033[2J\033[1;1H";
    // Hoặc std::system("clear"); nhưng không portable bằng ANSI escape code
#endif
}

/**
 * @brief Tạm dừng chương trình cho đến khi người dùng nhấn Enter
 * 
 * Hàm này hiển thị một thông báo và chờ người dùng nhấn phím Enter
 * để tiếp tục thực thi chương trình.
 * 
 * @param message Thông báo hiển thị cho người dùng
 */
void pauseExecution(const std::string& message) {
    std::cout << message << std::flush;
    // Chờ người dùng nhấn Enter
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (std::cin.gcount() == 0) { // Nếu trước đó không có gì trong buffer (ví dụ sau khi đọc số)
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

/**
 * @brief Vẽ tiêu đề có viền
 * 
 * Hàm này tạo một khung tiêu đề với viền xung quanh,
 * tiêu đề được căn giữa trong khung.
 * 
 * @param title Nội dung tiêu đề
 * @param borderChar Ký tự sử dụng làm viền
 * @param width Độ rộng của khung tiêu đề
 */
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

/**
 * @brief Vẽ hộp chứa nội dung với viền
 * 
 * Hàm này tạo một khung hộp có viền xung quanh và hiển thị 
 * các dòng nội dung bên trong hộp.
 * 
 * @param content Mảng các dòng nội dung cần hiển thị trong hộp
 * @param borderChar Ký tự sử dụng làm viền ngang
 * @param width Độ rộng tổng thể của hộp
 */
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