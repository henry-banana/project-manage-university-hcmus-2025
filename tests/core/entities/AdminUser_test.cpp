// test_AdminUser.cpp
#include <gtest/gtest.h>
#include "../../src/core/entities/AdminUser.h"
#include "../../src/common/ValidationResult.h"
#include "../../src/common/LoginStatus.h"
#include "../../src/common/UserRole.h"

// Helper to extract all error messages in a single string (for test output)
std::string allErrorMessages(const ValidationResult& result) {
    return result.getErrorMessagesCombined(" | ");
}

TEST(AdminUserTest, Constructor_InitializesCorrectly) {
    AdminUser admin("A001", "John", "Doe", LoginStatus::ACTIVE);

    EXPECT_EQ(admin.getId(), "A001");
    EXPECT_EQ(admin.getFirstName(), "John");
    EXPECT_EQ(admin.getLastName(), "Doe");
    EXPECT_EQ(admin.getStatus(), LoginStatus::ACTIVE);
    EXPECT_EQ(admin.getRole(), UserRole::ADMIN);
}

TEST(AdminUserTest, Display_ReturnsCorrectFormat) {
    AdminUser admin("A001", "John", "Doe");
    std::string expected = "ADMIN - [A001] Doe John";
    EXPECT_EQ(admin.display(), expected);
}

TEST(AdminUserTest, ValidateBasic_ReturnsValid_WithCorrectData) {
    AdminUser admin("A001", "John", "Doe");
    ValidationResult result = admin.validateBasic();

    EXPECT_TRUE(result.isValid) << allErrorMessages(result);
    EXPECT_TRUE(result.errors.empty());
}

TEST(AdminUserTest, ValidateBasic_ReturnsInvalid_WhenIdEmpty) {
    AdminUser admin("", "John", "Doe");
    ValidationResult result = admin.validateBasic();

    EXPECT_FALSE(result.isValid);
    ASSERT_FALSE(result.errors.empty());
    EXPECT_NE(allErrorMessages(result).find("id"), std::string::npos);
}

TEST(AdminUserTest, ValidateBasic_ReturnsInvalid_WhenFirstNameEmpty) {
    AdminUser admin("A001", "", "Doe");
    ValidationResult result = admin.validateBasic();

    EXPECT_FALSE(result.isValid);
    ASSERT_FALSE(result.errors.empty());
    EXPECT_NE(allErrorMessages(result).find("first name"), std::string::npos);
}

TEST(AdminUserTest, ValidateBasic_ReturnsInvalid_WhenLastNameEmpty) {
    AdminUser admin("A001", "John", "");
    ValidationResult result = admin.validateBasic();

    EXPECT_FALSE(result.isValid);
    ASSERT_FALSE(result.errors.empty());
    EXPECT_NE(allErrorMessages(result).find("last name"), std::string::npos);
}

TEST(AdminUserTest, ValidateBasic_ReturnsInvalid_WhenMultipleErrors) {
    AdminUser admin("", "", "");
    ValidationResult result = admin.validateBasic();

    EXPECT_FALSE(result.isValid);
    EXPECT_GE(result.errors.size(), 3); // Có ít nhất 3 lỗi
    std::string combined = allErrorMessages(result);
    EXPECT_NE(combined.find("id"), std::string::npos);
    EXPECT_NE(combined.find("first name"), std::string::npos);
    EXPECT_NE(combined.find("last name"), std::string::npos);
}
