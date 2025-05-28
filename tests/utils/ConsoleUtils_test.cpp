#include "gtest/gtest.h"
#include "../src/utils/ConsoleUtils.h"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // For std::getenv

// Test clearScreen
TEST(ConsoleUtilsTest, ClearScreen) {
    // This test only verifies that clearScreen() doesn't throw an exception
    // It's difficult to directly assert its behavior without mocking system calls
    ASSERT_NO_THROW(clearScreen());
}

// Test pauseExecution
TEST(ConsoleUtilsTest, PauseExecution) {
    // Redirect std::cout to a stringstream
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    // Call pauseExecution
    std::string message = "Test message";
    pauseExecution(message);

    // Restore std::cout
    std::cout.rdbuf(oldCout);

    // Assert that the correct message was printed
    ASSERT_EQ(ss.str(), message + "\nPress Enter to continue...");
}

// Test drawHeader
TEST(ConsoleUtilsTest, DrawHeader) {
    // Redirect std::cout to a stringstream
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    // Call drawHeader
    std::string title = "Test Header";
    drawHeader(title, '*', 40);

    // Restore std::cout
    std::cout.rdbuf(oldCout);

    // Assert that the correct header was printed
    std::string expectedHeader = "\n****************************************\n*             Test Header              *\n****************************************\n";
    ASSERT_EQ(ss.str(), expectedHeader);
}

// Test drawBox
TEST(ConsoleUtilsTest, DrawBox) {
    // Redirect std::cout to a stringstream
    std::stringstream ss;
    std::streambuf* oldCout = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    // Call drawBox
    std::vector<std::string> content = {"Line 1", "Line 2", "A longer line"};
    drawBox(content, '-', 30);

    // Restore std::cout
    std::cout.rdbuf(oldCout);

    // Assert that the correct box was printed
    std::string expectedBox = "+----------------------------+\n| Line 1                       |\n| Line 2                       |\n| A longer line                |\n+----------------------------+\n";
    ASSERT_EQ(ss.str(), expectedBox);
}