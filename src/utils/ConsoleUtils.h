/**
 * @file ConsoleUtils.h
 * @brief Định nghĩa các hàm tiện ích cho giao diện console
 * 
 * File này cung cấp các hàm tiện ích để tương tác với giao diện console như
 * xóa màn hình, tạm dừng thực thi, và vẽ các khung hiển thị.
 */
#ifndef CONSOLEUTILS_H
#define CONSOLEUTILS_H

#include <string>
#include <vector>
#include <iostream> // (➕) Cần cho cout
#include <limits>   // (➕) Cần cho numeric_limits

/**
 * @brief Xóa màn hình console
 * 
 * Hàm này xóa nội dung hiển thị trên màn hình console,
 * phụ thuộc vào nền tảng hệ điều hành.
 */
void clearScreen();

/**
 * @brief Tạm dừng thực thi chương trình và chờ người dùng nhấn Enter
 * 
 * @param message Thông báo hiển thị khi tạm dừng
 */
void pauseExecution(const std::string& message = "\nPress Enter to continue...");

/**
 * @brief Vẽ tiêu đề trong khung viền
 * 
 * @param title Nội dung tiêu đề
 * @param borderChar Ký tự viền
 * @param width Chiều rộng của khung
 */
void drawHeader(const std::string& title, char borderChar = '*', size_t width = 60);

/**
 * @brief Vẽ nội dung trong khung viền
 * 
 * @param content Mảng các dòng nội dung
 * @param borderChar Ký tự viền
 * @param width Chiều rộng của khung
 */
void drawBox(const std::vector<std::string>& content, char borderChar = '-', size_t width = 60);

// Không cần các hàm prompt ở đây nữa vì đã có InputPrompter
// int getMenuChoice(int minOption, int maxOption, const std::string& prompt = "Enter your choice: ");
// std::string promptForString(const std::string& prompt, bool allowEmpty = false);
// long promptForLong(const std::string& prompt);
// bool promptForYesNo(const std::string& prompt);

#endif // CONSOLEUTILS_H