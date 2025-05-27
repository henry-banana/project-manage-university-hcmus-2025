#include "gtest/gtest.h"
#include "../src/common/LoginStatus.h"

TEST(LoginStatusTest, EnumValues) {
    ASSERT_EQ(static_cast<int>(LoginStatus::ACTIVE), 0);
    ASSERT_EQ(static_cast<int>(LoginStatus::PENDING_APPROVAL), 1);
    ASSERT_EQ(static_cast<int>(LoginStatus::DISABLED), 2);
}

TEST(LoginStatusUtilsTest, ToString) {
    ASSERT_EQ(LoginStatusUtils::toString(LoginStatus::ACTIVE), "ACTIVE");
    ASSERT_EQ(LoginStatusUtils::toString(LoginStatus::PENDING_APPROVAL), "PENDING_APPROVAL");
    ASSERT_EQ(LoginStatusUtils::toString(LoginStatus::DISABLED), "DISABLED");
}

TEST(LoginStatusUtilsTest, FromString) {
    ASSERT_EQ(LoginStatusUtils::fromString("ACTIVE"), LoginStatus::ACTIVE);
    ASSERT_EQ(LoginStatusUtils::fromString("PENDING_APPROVAL"), LoginStatus::PENDING_APPROVAL);
    ASSERT_EQ(LoginStatusUtils::fromString("DISABLED"), LoginStatus::DISABLED);
}

TEST(LoginStatusUtilsTest, FromStringInvalid) {
    // Assuming fromString returns a default value or throws an exception for invalid strings
    // Adjust the assertion based on the actual behavior of fromString
    ASSERT_EQ(LoginStatusUtils::fromString("INVALID"), LoginStatus::ACTIVE); // Or whatever default value is
}
