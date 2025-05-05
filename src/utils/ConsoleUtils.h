#ifndef CONSOLEUTILS_H
#define CONSOLEUTILS_H

#include <string>
#include <vector>

// Clears the console screen (platform-dependent).
void clearScreen();

// Pauses execution and waits for the user to press Enter.
void pauseExecution(const std::string& message = "Press Enter to continue...");

// Basic header box
void drawHeader(const std::string& title, char borderChar = '*');

// Simple box around content lines
void drawBox(const std::vector<std::string>& content, char borderChar = '-');

// Gets an integer choice from the user within a specified range.
// Handles invalid input and reprompts.
int getMenuChoice(int minOption, int maxOption, const std::string& prompt = "Enter your choice: ");

// Prompts the user for a non-empty string.
std::string promptForString(const std::string& prompt, bool allowEmpty = false);

// Prompts the user for a long integer. Handles invalid input.
long promptForLong(const std::string& prompt);

// Prompts user for Y/N, returns true for Y, false for N.
bool promptForYesNo(const std::string& prompt);

#endif // CONSOLEUTILS_H