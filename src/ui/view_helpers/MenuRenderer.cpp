#include "MenuRenderer.h"
#include <iomanip> // For std::setw

void MenuRenderer::renderMenu(const std::string& title, const std::vector<MenuItemDisplay>& items) const {
    std::cout << "\n--- " << title << " ---" << std::endl;
    if (items.empty()) {
        std::cout << "No options available in this menu." << std::endl;
    } else {
        for (const auto& item : items) {
            // Tìm key dài nhất để căn chỉnh description (nếu cần thiết)
            // Hiện tại in đơn giản
            std::cout << item.key << ". " << item.description << std::endl;
        }
    }
    std::cout << "-------------------------" << std::endl;
}