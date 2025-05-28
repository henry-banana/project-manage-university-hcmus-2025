#include "gtest/gtest.h"
#include "../src/utils/PasswordInput.h"
#include <string>
#include <vector>
#include <algorithm>

TEST(PasswordUtilsTest, GenerateSalt) {
    // Verify that the generated salt has the correct length
    size_t length = 20;
    std::string salt = PasswordUtils::generateSalt(length);
    ASSERT_EQ(salt.length(), length);

    // Verify that the generated salt is different each time the function is called
    std::string salt2 = PasswordUtils::generateSalt(length);
    ASSERT_NE(salt, salt2);
}

TEST(PasswordUtilsTest, HashPassword) {
    // Verify that the hashed password is not the same as the plain password
    std::string plainPassword = "password123";
    std::string salt = PasswordUtils::generateSalt();
    std::string hashedPassword = PasswordUtils::hashPassword(plainPassword, salt);
    ASSERT_NE(hashedPassword, plainPassword);

    // Verify that the hashed password is the same for the same plain password and salt
    std::string hashedPassword2 = PasswordUtils::hashPassword(plainPassword, salt);
    ASSERT_EQ(hashedPassword, hashedPassword2);
}

TEST(PasswordUtilsTest, VerifyPassword) {
    // Verify that the function returns true for a valid password
    std::string plainPassword = "password123";
    std::string salt = PasswordUtils::generateSalt();
    std::string hashedPassword = PasswordUtils::hashPassword(plainPassword, salt);
    ASSERT_TRUE(PasswordUtils::verifyPassword(plainPassword, hashedPassword, salt));

    // Verify that the function returns false for an invalid password
    std::string invalidPassword = "wrongpassword";
    ASSERT_FALSE(PasswordUtils::verifyPassword(invalidPassword, hashedPassword, salt));
}

TEST(PasswordUtilsTest, IsPasswordComplexEnough) {
    // Verify that the function returns true for a complex password
    std::string complexPassword = "Password123!";
    ASSERT_TRUE(PasswordUtils::isPasswordComplexEnough(complexPassword));

    // Verify that the function returns false for a simple password
    std::string simplePassword = "password";
    ASSERT_FALSE(PasswordUtils::isPasswordComplexEnough(simplePassword));

    // Test with a custom minimum length
    ASSERT_FALSE(PasswordUtils::isPasswordComplexEnough("Pass1!", 10));
    ASSERT_TRUE(PasswordUtils::isPasswordComplexEnough("Pass12345!", 8));
}

TEST(PasswordUtilsTest, GetMaskedPasswordDoesNotCrash) {
    // This is a very basic test to ensure that the function doesn't crash.
    // Due to the platform-specific nature and reliance on user input,
    // it's difficult to test this function directly.
    ASSERT_NO_THROW(PasswordUtils::getMaskedPassword("Test Prompt: "));
    // Manual verification is needed to ensure that the password is actually masked.
}