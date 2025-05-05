#include "ConsoleUtils.h"
#include <iostream>
#include <cstdlib> // For system()
#include <limits> // For numeric_limits
#include <algorithm> // For std::max
#include <iomanip> // For std::setw
#include <format>
#include <conio.h>

void clearScreen() {
    #ifdef _WIN32 // Check for Windows
        std::system("cls");
    #else
        // Assume POSIX-like system (Linux, macOS)
        std::system("clear");
    #endif
}

void pauseExecution(const std::string& message) {
    std::cout << std::format(
        "\n{} Press Enter to continue...\n", message
    );
    _getch(); // Wait for a key press (Enter)
    std::cout << std::endl;
}

void drawHeader(const std::string& title, char borderChar) {
    std::string border(title.length() + 4, borderChar);
    std::cout << border << std::endl;
    std::cout << borderChar << " " << title << " " << borderChar << std::endl;
    std::cout << border << std::endl;
}

    void drawBox(const std::vector<std::string>& content, char borderChar) {
    if (content.empty()) return;

    size_t maxWidth = 0;
    for (const auto& line : content) {
        maxWidth = std::max(maxWidth, line.length());
    }

    std::string border(maxWidth + 4, borderChar);
    std::cout << border << std::endl;
    for (const auto& line : content) {
            std::cout << borderChar << " " << std::left << std::setw(maxWidth) << line << " " << borderChar << std::endl;
    }
    std::cout << border << std::endl;
    }


int getMenuChoice(int minOption, int maxOption, const std::string& prompt) {
    int choice = -1;
    while (true) {
        std::cout << prompt;
        std::cin >> choice;

        if (std::cin.fail() || choice < minOption || choice > maxOption) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter a number between "
                        << minOption << " and " << maxOption << "." << std::endl;
        } else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard the rest of the line (e.g., newline)
            break; // Valid input
        }
    }
    return choice;
}

std::string promptForString(const std::string& prompt, bool allowEmpty) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input); // Read whole line
        if (!input.empty() || allowEmpty) {
                break;
        } else {
                std::cout << "Input cannot be empty. Please try again." << std::endl;
        }
    }
    return input;
}

long promptForLong(const std::string& prompt) {
    long value = 0;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break; // Valid input
        }
    }
    return value;
}

bool promptForYesNo(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt << " (y/n): ";
        std::getline(std::cin, input);
        if (input == "y" || input == "Y") return true;
        if (input == "n" || input == "N") return false;
        std::cout << "Invalid input. Please enter 'y' or 'n'." << std::endl;
    }
}