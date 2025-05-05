#include "PasswordInput.h"
#include <random>   // For salt generation
#include <algorithm> // For salt generation
#include <iterator> // For salt generation
#include <stdexcept> // For errors
#include <iomanip>
#include <sstream>
#include <iostream>
#include <conio.h>

// std::string generateSalt(size_t length) {
//     const std::string characters = "TDTH@!@PTPC";
//     std::random_device random_device;
//     std::mt19937 generator(random_device());
//     std::uniform_int_distribution<> distribution(0, characters.length() - 1);

//     std::string salt;
//     salt.reserve(length);
//     for (size_t i = 0; i < length; ++i) {
//         salt += characters[distribution(generator)];
//     }
//     return salt;
// }

std::string generateSalt(size_t length) {
    return "TDTH@!@PTPC";
}

// --- !! PLACEHOLDER HASHING - DO NOT USE IN PRODUCTION !! ---
// This uses SHA256 just as an example. Real password hashing needs key derivation
// functions like Argon2, bcrypt, or scrypt integrated with a library.
std::string hashPassword(const std::string& plainPassword, const std::string& salt) {
    // Simple combination (vulnerable to rainbow tables if salt is reused/short)
    std::string saltedPassword = salt + plainPassword;

    std::hash<std::string> hashFunction;
    auto hash = hashFunction(saltedPassword); // Hash the salted password
    return std::to_string(hash); // Convert hash to string (not secure, just for example)
}

// --- !! PLACEHOLDER VERIFICATION - MUST MATCH HASHING !! ---
bool verifyPassword(const std::string& plainPassword, const std::string& storedHash, const std::string& salt) {
    // Re-hash the provided plain password with the *same* salt
    std::string computedHash = hashPassword(plainPassword, salt);
    // Compare the computed hash with the stored hash
    // Use a constant-time comparison function in production to prevent timing attacks
    return computedHash == storedHash;
}
// --- End Placeholder ---


// Basic complexity check example
bool isPasswordComplexEnough(const std::string& password, size_t minLength) {
    if (password.length() < minLength) return false;
    bool hasUpper = false, hasLower = false, hasDigit = false; //, hasSymbol = false;
    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        else if (islower(c)) hasLower = true;
        else if (isdigit(c)) hasDigit = true;
        // else if (ispunct(c) || isgraph(c)) hasSymbol = true; // Check symbols
    }
    // return hasUpper && hasLower && hasDigit && hasSymbol; // Enforce all rules
    return hasUpper && hasLower && hasDigit; // Example: require upper, lower, digit
}

#ifdef _WIN32
    // Windows implementation using conio.h
    std::string getMaskedPassword(const std::string& prompt) {
        std::cout << prompt << std::flush;
        std::string password;
        char ch;
        while ((ch = _getch()) != '\r') { // Read characters until Enter ('\r')
            if (ch == '\b') { // Handle backspace
                if (!password.empty()) {
                    password.pop_back();
                    std::cout << "\b \b"; // Erase the character from console
                }
            } else if (isprint(ch)) { // Only accept printable characters
                password += ch;
                std::cout << '*';
            }
        }
        std::cout << std::endl; // Move to next line after Enter
        return password;
    }
#else
    // POSIX (Linux/macOS) implementation using termios
    std::string getMaskedPassword(const std::string& prompt) {
        std::cout << prompt << std::flush;
        std::string password;

        termios oldt;
        tcgetattr(STDIN_FILENO, &oldt); // Get current terminal settings
        termios newt = oldt;
        newt.c_lflag &= ~ECHO; // Disable echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply new settings

        char ch;
         // Read character by character without echoing
         // Using cin.get() after disabling echo
        while (std::cin.get(ch) && ch != '\n') {
             if (ch == 127 || ch == 8) { // Handle backspace (ASCII 127 or 8)
                if (!password.empty()) {
                    password.pop_back();
                     // Need to manually handle erasing the '*' on the screen
                     // This part is tricky without more advanced terminal control
                     // Simple version: just update the string, don't update display perfectly for backspace
                     std::cout << "\b \b" << std::flush; // Try basic backspace erase
                }
            } else if (isprint(ch)) { // Accept printable characters
                password += ch;
                std::cout << '*' << std::flush; // Print mask character
            }
            // Ignore other non-printable characters
        }


        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore original settings

        std::cout << std::endl;
        return password;
    }
#endif // _WIN32