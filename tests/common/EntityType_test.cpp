#include "gtest/gtest.h"
#include "../src/common/EntityType.h"

TEST(EntityTypeTest, EnumValues) {
    ASSERT_EQ(static_cast<int>(EntityType::STUDENT), 0);
    ASSERT_EQ(static_cast<int>(EntityType::TEACHER), 1);
    ASSERT_EQ(static_cast<int>(EntityType::FACULTY), 2);
    ASSERT_EQ(static_cast<int>(EntityType::COURSE), 3);
    ASSERT_EQ(static_cast<int>(EntityType::ENROLLMENT), 4);
    ASSERT_EQ(static_cast<int>(EntityType::COURSERESULT), 5);
    ASSERT_EQ(static_cast<int>(EntityType::FEERECORD), 6);
    ASSERT_EQ(static_cast<int>(EntityType::SALARYRECORD), 7);
    ASSERT_EQ(static_cast<int>(EntityType::LOGIN), 8);
}