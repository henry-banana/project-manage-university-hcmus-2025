#include "gtest/gtest.h"
#include "../src/common/LoginStatus.h"
#include <stdexcept> // Required for ASSERT_THROW

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
    ASSERT_THROW(LoginStatusUtils::fromString("INVALID"), std::invalid_argument);
}