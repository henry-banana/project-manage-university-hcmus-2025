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

TEST(AdminUserTest, ValidateBasic_ReturnsValid_WithCorrectData) {
    AdminUser admin("A001", "John", "Doe");
    ValidationResult result = admin.validateBasic();

    EXPECT_TRUE(result.isValid) << allErrorMessages(result);
    EXPECT_TRUE(result.errors.empty());
}
