#include "gtest/gtest.h"
#include "../src/common/DataSourceType.h"

TEST(DataSourceTypeTest, EnumValues) {
    ASSERT_EQ(static_cast<int>(DataSourceType::MOCK), 0); // Assuming MOCK is the first value
    ASSERT_EQ(static_cast<int>(DataSourceType::CSV), 1);  // Assuming CSV is the second value
    ASSERT_EQ(static_cast<int>(DataSourceType::SQL), 2);  // Assuming SQL is the third value
}