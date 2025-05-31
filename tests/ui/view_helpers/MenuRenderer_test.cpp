// --- START OF NEW FILE tests/MenuRenderer_test.cpp ---
#include "gtest/gtest.h"
#include "../src/ui/view_helpers/MenuRenderer.h"
#include <sstream> // Để bắt output

TEST(MenuRendererTest, RenderEmptyMenu) {
    MenuRenderer renderer;
    std::vector<MenuItemDisplay> items = {};
    
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf()); // Redirect std::cout

    renderer.renderMenu("Empty Menu", items);

    std::cout.rdbuf(oldCout); // Restore std::cout

    std::string expectedOutput = "\n--- Empty Menu ---\nNo options available in this menu.\n-------------------------\n";
    EXPECT_EQ(ss.str(), expectedOutput);
}

TEST(MenuRendererTest, RenderMenuWithItems) {
    MenuRenderer renderer;
    std::vector<MenuItemDisplay> items = {
        {"1", "Option One"},
        {"2", "Option Two"},
        {"0", "Exit"}
    };

    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    renderer.renderMenu("Main Menu", items);

    std::cout.rdbuf(oldCout);
    
    std::string expectedOutput = "\n--- Main Menu ---\n1. Option One\n2. Option Two\n0. Exit\n-------------------------\n";
    EXPECT_EQ(ss.str(), expectedOutput);
}
// --- END OF NEW FILE tests/MenuRenderer_test.cpp ---