#include "gtest/gtest.h"
#include "../src/common/UserRole.h"

TEST(UserRoleTest, EnumValues) {
    ASSERT_EQ(static_cast<int>(UserRole::ADMIN), 0);
    ASSERT_EQ(static_cast<int>(UserRole::STUDENT), 1);
    ASSERT_EQ(static_cast<int>(UserRole::TEACHER), 2);
    ASSERT_EQ(static_cast<int>(UserRole::PENDING_STUDENT), 3);
    ASSERT_EQ(static_cast<int>(UserRole::UNKNOWN), 4);
}