#include <gtest/gtest.h>
#include "../src/core/entities/Faculty.h"
#include "../src/common/ValidationResult.h"
#include "../src/common/ErrorType.h"

// Test constructor và getter
TEST(FacultyTest, ConstructorAndGetters) {
    Faculty f("IT", "Information Technology");

    EXPECT_EQ(f.getId(), "IT");
    EXPECT_EQ(f.getName(), "Information Technology");
}

// Test setName
TEST(FacultyTest, SetName) {
    Faculty f("IT", "Old Name");

    EXPECT_TRUE(f.setName("New Name"));
    EXPECT_EQ(f.getName(), "New Name");

    EXPECT_FALSE(f.setName(""));
    EXPECT_EQ(f.getName(), "New Name");  // Không đổi
}

// Test getStringId
TEST(FacultyTest, GetStringId) {
    Faculty f("IT", "Information Technology");
    EXPECT_EQ(f.getStringId(), "IT");
}

// Test display
TEST(FacultyTest, Display) {
    Faculty f("IT", "Information Technology");
    std::string display = f.display();

    EXPECT_NE(display.find("IT"), std::string::npos);
    EXPECT_NE(display.find("Information Technology"), std::string::npos);
}

// Test validateBasic: hợp lệ
TEST(FacultyTest, ValidateBasic_Valid) {
    Faculty f("IT", "Information Technology");
    ValidationResult result = f.validateBasic();
    EXPECT_TRUE(result.errors.empty());
}

// Test validateBasic: không hợp lệ
TEST(FacultyTest, ValidateBasic_Invalid) {
    Faculty f1("", "Valid Name");
    Faculty f2("IT", "");

    EXPECT_FALSE(f1.validateBasic().errors.empty());
    EXPECT_FALSE(f2.validateBasic().errors.empty());
}
