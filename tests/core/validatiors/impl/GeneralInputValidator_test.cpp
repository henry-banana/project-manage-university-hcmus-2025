#include "gtest/gtest.h"
#include "../src/core/validators/impl/GeneralInputValidator.h"

TEST(GeneralInputValidatorTest, ValidateEmail) {
    GeneralInputValidator validator;

    // Valid email addresses
    ASSERT_TRUE(validator.validateEmail("test@example.com").isValid);
    ASSERT_TRUE(validator.validateEmail("test.test@example.com").isValid);
    ASSERT_TRUE(validator.validateEmail("test_test@example.com").isValid);
    ASSERT_TRUE(validator.validateEmail("test@example.co.uk").isValid);

    // Invalid email addresses
    ASSERT_FALSE(validator.validateEmail("").isValid);
    ASSERT_FALSE(validator.validateEmail("test").isValid);
    ASSERT_FALSE(validator.validateEmail("test@").isValid);
    ASSERT_FALSE(validator.validateEmail("@example.com").isValid);
    ASSERT_FALSE(validator.validateEmail("test@example").isValid);
    ASSERT_FALSE(validator.validateEmail("test@.com").isValid);
    ASSERT_FALSE(validator.validateEmail("test@example.c").isValid);
    ASSERT_FALSE(validator.validateEmail("test@example.toolongtoolongtoolongtoolongtoolongtoolongtoolongtoolongtoolongtoolong.com").isValid);
}

TEST(GeneralInputValidatorTest, ValidatePhoneNumber) {
    GeneralInputValidator validator;

    // Valid Vietnamese phone numbers
    ASSERT_TRUE(validator.validatePhoneNumber("0912345678", "VN").isValid);
    ASSERT_TRUE(validator.validatePhoneNumber("01234567890", "VN").isValid);

    // Invalid Vietnamese phone numbers
    ASSERT_FALSE(validator.validatePhoneNumber("", "VN").isValid);
    ASSERT_FALSE(validator.validatePhoneNumber("123456789", "VN").isValid);
    ASSERT_FALSE(validator.validatePhoneNumber("091234567890", "VN").isValid);
    ASSERT_FALSE(validator.validatePhoneNumber("091234567a", "VN").isValid);

    // Valid other phone numbers
    ASSERT_TRUE(validator.validatePhoneNumber("1234567", "US").isValid);
    ASSERT_TRUE(validator.validatePhoneNumber("123456789012345", "US").isValid);

    // Invalid other phone numbers
    ASSERT_FALSE(validator.validatePhoneNumber("", "US").isValid);
    ASSERT_FALSE(validator.validatePhoneNumber("123456", "US").isValid);
    ASSERT_FALSE(validator.validatePhoneNumber("1234567890123456", "US").isValid);
    ASSERT_FALSE(validator.validatePhoneNumber("1234567a", "US").isValid);
}

TEST(GeneralInputValidatorTest, ValidatePasswordComplexity) {
    GeneralInputValidator validator;

    // Valid passwords
    ASSERT_TRUE(validator.validatePasswordComplexity("Test123!").isValid);
    ASSERT_TRUE(validator.validatePasswordComplexity("LongPassword123").isValid);

    // Invalid passwords
    ASSERT_FALSE(validator.validatePasswordComplexity("").isValid);
    ASSERT_FALSE(validator.validatePasswordComplexity("Test").isValid);
    ASSERT_FALSE(validator.validatePasswordComplexity("test1234").isValid);
    ASSERT_FALSE(validator.validatePasswordComplexity("TEST1234").isValid);
    ASSERT_FALSE(validator.validatePasswordComplexity("TestTest").isValid);
    ASSERT_FALSE(validator.validatePasswordComplexity("12345678").isValid);
}

TEST(GeneralInputValidatorTest, ValidateIdFormat) {
    GeneralInputValidator validator;

    // Valid IDs
    ASSERT_TRUE(validator.validateIdFormat("12345", "ID", 5, 10).isValid);
    ASSERT_TRUE(validator.validateIdFormat("1234567890", "ID", 5, 10).isValid);

    // Invalid IDs
    ASSERT_FALSE(validator.validateIdFormat("", "ID", 5, 10).isValid);
    ASSERT_FALSE(validator.validateIdFormat("1234", "ID", 5, 10).isValid);
    ASSERT_FALSE(validator.validateIdFormat("12345678901", "ID", 5, 10).isValid);
}

TEST(GeneralInputValidatorTest, ValidateRequiredString) {
    GeneralInputValidator validator;

    // Valid strings
    ASSERT_TRUE(validator.validateRequiredString("test", "String", 10).isValid);
    ASSERT_TRUE(validator.validateRequiredString("test test", "String", 10).isValid);

    // Invalid strings
    ASSERT_FALSE(validator.validateRequiredString("", "String", 10).isValid);
    ASSERT_FALSE(validator.validateRequiredString("toolongtoolongtoolong", "String", 10).isValid);
}

TEST(GeneralInputValidatorTest, ValidateOptionalString) {
    GeneralInputValidator validator;

    // Valid strings
    ASSERT_TRUE(validator.validateOptionalString("", "String", 10).isValid);
    ASSERT_TRUE(validator.validateOptionalString("test", "String", 10).isValid);
    ASSERT_TRUE(validator.validateOptionalString("test test", "String", 10).isValid);

    // Invalid strings
    ASSERT_FALSE(validator.validateOptionalString("toolongtoolongtoolong", "String", 10).isValid);
}

TEST(GeneralInputValidatorTest, ValidateInteger) {
    GeneralInputValidator validator;

    // Valid integers
    ASSERT_TRUE(validator.validateInteger(5, "Integer", 0, 10).isValid);
    ASSERT_TRUE(validator.validateInteger(0, "Integer", 0, 10).isValid);
    ASSERT_TRUE(validator.validateInteger(10, "Integer", 0, 10).isValid);

    // Invalid integers
    ASSERT_FALSE(validator.validateInteger(-1, "Integer", 0, 10).isValid);
    ASSERT_FALSE(validator.validateInteger(11, "Integer", 0, 10).isValid);
}

TEST(GeneralInputValidatorTest, ValidateDate) {
    GeneralInputValidator validator;

    // Valid dates
    ASSERT_TRUE(validator.validateDate(1, 1, 2020, "Date").isValid);
    ASSERT_TRUE(validator.validateDate(29, 2, 2020, "Date").isValid);
    ASSERT_TRUE(validator.validateDate(31, 12, 2020, "Date").isValid);

    // Invalid dates
    ASSERT_FALSE(validator.validateDate(0, 1, 2020, "Date").isValid);
    ASSERT_FALSE(validator.validateDate(1, 0, 2020, "Date").isValid);
    ASSERT_FALSE(validator.validateDate(1, 1, 2026, "Date").isValid);
    ASSERT_FALSE(validator.validateDate(30, 2, 2020, "Date").isValid);
    ASSERT_FALSE(validator.validateDate(31, 4, 2020, "Date").isValid);
}

TEST(GeneralInputValidatorTest, ValidateCitizenId) {
    GeneralInputValidator validator;

    // Valid Vietnamese citizen IDs
    ASSERT_TRUE(validator.validateCitizenId("123456789", "VN").isValid);
    ASSERT_TRUE(validator.validateCitizenId("123456789012", "VN").isValid);

    // Invalid Vietnamese citizen IDs
    ASSERT_FALSE(validator.validateCitizenId("", "VN").isValid);
    ASSERT_FALSE(validator.validateCitizenId("12345678", "VN").isValid);
    ASSERT_FALSE(validator.validateCitizenId("1234567890123", "VN").isValid);
    ASSERT_FALSE(validator.validateCitizenId("12345678a", "VN").isValid);

    // Valid other citizen IDs
    ASSERT_TRUE(validator.validateCitizenId("12345", "US").isValid);
    ASSERT_TRUE(validator.validateCitizenId("12345678901234567890", "US").isValid);

    // Invalid other citizen IDs
    ASSERT_FALSE(validator.validateCitizenId("", "US").isValid);
    ASSERT_FALSE(validator.validateCitizenId("1234", "US").isValid);
    ASSERT_FALSE(validator.validateCitizenId("123456789012345678901", "US").isValid);
    ASSERT_FALSE(validator.validateCitizenId("12345a", "US").isValid);
}