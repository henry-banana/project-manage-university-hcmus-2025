#ifndef PASSWORDINPUT_H
#define PASSWORDINPUT_H

#include <string>

// Gets password input, masking characters with '*'
// !! Implementation is platform-dependent !!
std::string getMaskedPassword(const std::string& prompt = "Enter Password: ");

// Generates a random salt (adjust length as needed)
std::string generateSalt(size_t length = 16);

// Hashes a plain password using the provided salt.
// !! Placeholder Implementation: Replace with a strong hashing algorithm (e.g., Argon2, bcrypt, scrypt) !!
// Never use simple hashes like SHA-1/MD5 for passwords directly.
std::string hashPassword(const std::string& plainPassword, const std::string& salt);

// Verifies a plain password against a stored hash and salt.
// !! Placeholder Implementation: Must match the hashing algorithm used above !!
bool verifyPassword(const std::string& plainPassword, const std::string& storedHash, const std::string& salt);

// Optional: Basic password complexity check
bool isPasswordComplexEnough(const std::string& password, size_t minLength = 8);

#endif // PASSWORDINPUT_H