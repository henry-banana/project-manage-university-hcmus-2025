#include "MenuRenderer.h"
#include <iomanip> // For std::setw

/**
 * @brief Hiển thị menu với tiêu đề và các mục tùy chọn
 * 
 * Phương thức này hiển thị một menu với tiêu đề và danh sách các mục tùy chọn.
 * Mỗi mục tùy chọn bao gồm một khóa (key) và mô tả tương ứng.
 * 
 * @param title Tiêu đề của menu
 * @param items Mảng các mục tùy chọn menu, mỗi mục bao gồm khóa và mô tả
 */
void MenuRenderer::renderMenu(const std::string& title, const std::vector<MenuItemDisplay>& items) const {
    std::cout << "\n--- " << title << " ---\n";
    if (items.empty()) {
        std::cout << "No options available in this menu.\n";
    } else {
        for (const auto& item : items) {
            // Tìm key dài nhất để căn chỉnh description (nếu cần thiết)
            // Hiện tại in đơn giản
            std::cout << item.key << ". " << item.description << "\n";
        }
    }
    std::cout << "-------------------------\n";
}