#include <gtest/gtest.h>
#include "../src/core/entities/Course.h"
#include "../src/common/ValidationResult.h"
#include "../src/common/ErrorType.h"

// Constructor & Getters
TEST(CourseTest, ConstructorAndGetters) {
    Course c("CS101", "Intro to Programming", 3, "CS");

    EXPECT_EQ(c.getId(), "CS101");
    EXPECT_EQ(c.getName(), "Intro to Programming");
    EXPECT_EQ(c.getCredits(), 3);
    EXPECT_EQ(c.getFacultyId(), "CS");
    EXPECT_EQ(c.getStringId(), "CS101");

    std::string display = c.display();
    EXPECT_NE(display.find("CS101"), std::string::npos);
    EXPECT_NE(display.find("Intro to Programming"), std::string::npos);
}

// setName
TEST(CourseTest, SetName) {
    Course c("CS101", "Old Name", 3, "CS");

    EXPECT_TRUE(c.setName("New Name"));
    EXPECT_EQ(c.getName(), "New Name");

    EXPECT_FALSE(c.setName(""));
    EXPECT_EQ(c.getName(), "New Name");
}

// setCredits
TEST(CourseTest, SetCredits) {
    Course c("CS101", "Programming", 3, "CS");

    EXPECT_TRUE(c.setCredits(5));
    EXPECT_EQ(c.getCredits(), 5);

    EXPECT_FALSE(c.setCredits(0));
    EXPECT_FALSE(c.setCredits(-2));
    EXPECT_EQ(c.getCredits(), 5);
}

// setFacultyId
TEST(CourseTest, SetFacultyId) {
    Course c("CS101", "Programming", 3, "CS");

    EXPECT_TRUE(c.setFacultyId("MATH"));
    EXPECT_EQ(c.getFacultyId(), "MATH");

    EXPECT_FALSE(c.setFacultyId(""));
    EXPECT_EQ(c.getFacultyId(), "MATH");
}

// validateBasic: hợp lệ
TEST(CourseTest, ValidateBasic_Valid) {
    Course c("CS101", "Intro to Programming", 3, "CS");
    ValidationResult result = c.validateBasic();
    EXPECT_TRUE(result.errors.empty());
}

// validateBasic: không hợp lệ
TEST(CourseTest, ValidateBasic_Invalid) {
    Course c1("", "Name", 3, "CS");
    Course c2("CS101", "", 3, "CS");
    Course c3("CS101", "Name", 0, "CS");
    Course c4("CS101", "Name", 3, "");

    EXPECT_FALSE(c1.validateBasic().errors.empty());
    EXPECT_FALSE(c2.validateBasic().errors.empty());
    EXPECT_FALSE(c3.validateBasic().errors.empty());
    EXPECT_FALSE(c4.validateBasic().errors.empty());
}
