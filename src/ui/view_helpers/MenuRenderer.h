/**
 * @file MenuRenderer.h
 * @brief Định nghĩa lớp hiển thị menu trong giao diện console
 * 
 * Lớp MenuRenderer cung cấp phương thức để hiển thị menu với
 * các tùy chọn có định dạng thống nhất trong giao diện console.
 */
#ifndef MENURENDERER_H
#define MENURENDERER_H

#include <vector>
#include <string>
#include <iostream>
#include <utility> // For std::pair

/**
 * @struct MenuItemDisplay
 * @brief Cấu trúc lưu trữ thông tin một mục trong menu
 * 
 * Cấu trúc này định nghĩa một mục menu với khóa (key) là ký tự hoặc số
 * để người dùng nhập vào, và mô tả (description) là nội dung hiển thị.
 */
struct MenuItemDisplay { // (➕) Để định nghĩa một mục trong menu
    std::string key; ///< Phím tắt cho mục menu, ví dụ "1", "a"
    std::string description; ///< Mô tả cho mục menu
};

/**
 * @class MenuRenderer
 * @brief Lớp hiển thị menu trong giao diện console
 * 
 * Lớp này cung cấp các phương thức để hiển thị menu với các tùy chọn
 * có định dạng thống nhất, giúp người dùng dễ dàng tương tác.
 */
class MenuRenderer {
public:
    /**
     * @brief Hàm khởi tạo mặc định
     */
    MenuRenderer() = default;
    
    /**
     * @brief Hiển thị menu với tiêu đề và danh sách các mục
     * @param title Tiêu đề của menu
     * @param items Danh sách các mục trong menu
     */
    void renderMenu(const std::string& title, const std::vector<MenuItemDisplay>& items) const;
};

#endif