#ifndef CONSOLEUTILS_H
#define CONSOLEUTILS_H

#include <string>
#include <vector>
#include <iostream> // (➕) Cần cho cout
#include <limits>   // (➕) Cần cho numeric_limits

// Clears the console screen (platform-dependent).
void clearScreen();

// Pauses execution and waits for the user to press Enter.
void pauseExecution(const std::string& message = "\nPress Enter to continue...");

// Basic header box
void drawHeader(const std::string& title, char borderChar = '*', size_t width = 60);

// Simple box around content lines
void drawBox(const std::vector<std::string>& content, char borderChar = '-', size_t width = 60);

// Không cần các hàm prompt ở đây nữa vì đã có InputPrompter
// int getMenuChoice(int minOption, int maxOption, const std::string& prompt = "Enter your choice: ");
// std::string promptForString(const std::string& prompt, bool allowEmpty = false);
// long promptForLong(const std::string& prompt);
// bool promptForYesNo(const std::string& prompt);

#endif // CONSOLEUTILS_H