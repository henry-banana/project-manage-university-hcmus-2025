#ifndef MENURENDERER_H
#define MENURENDERER_H

#include <vector>
#include <string>
#include <iostream>
#include <utility> // For std::pair

struct MenuItemDisplay { // (➕) Để định nghĩa một mục trong menu
    std::string key; // Ví dụ "1", "a"
    std::string description;
};

class MenuRenderer {
public:
    MenuRenderer() = default;
    void renderMenu(const std::string& title, const std::vector<MenuItemDisplay>& items) const;
};

#endif