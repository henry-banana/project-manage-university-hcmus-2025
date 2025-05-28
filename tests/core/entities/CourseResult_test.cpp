#include <gtest/gtest.h>
#include "../src/core/entities/CourseResult.h"
#include "../src/common/ValidationResult.h"
#include "../src/common/ErrorType.h"

bool hasNoErrors(const ValidationResult& result) {
    return result.errors.empty();
}

// Constructor & Getters
TEST(CourseResultTest, ConstructorAndGetters) {
    CourseResult result("SV001", "CS101", 85);
    EXPECT_EQ(result.getStudentId(), "SV001");
    EXPECT_EQ(result.getCourseId(), "CS101");
    EXPECT_EQ(result.getMarks(), 85);
    EXPECT_EQ(result.getGrade(), 'B');
}

// Grade mapping
TEST(CourseResultTest, GradeCalculation) {
    CourseResult a("S1", "C1", 95);  EXPECT_EQ(a.getGrade(), 'A');
    CourseResult b("S2", "C2", 85);  EXPECT_EQ(b.getGrade(), 'B');
    CourseResult c("S3", "C3", 70);  EXPECT_EQ(c.getGrade(), 'C');
    CourseResult d("S4", "C4", 60);  EXPECT_EQ(d.getGrade(), 'D');
    CourseResult f("S5", "C5", 40);  EXPECT_EQ(f.getGrade(), 'F');
    CourseResult none("S6", "C6");   EXPECT_EQ(none.getGrade(), '-');
}

// Setter marks & grade update
TEST(CourseResultTest, SetMarks_ValidAndInvalid) {
    CourseResult res("SV001", "CS101");

    EXPECT_TRUE(res.setMarks(90));
    EXPECT_EQ(res.getMarks(), 90);
    EXPECT_EQ(res.getGrade(), 'A');

    EXPECT_FALSE(res.setMarks(105));
    EXPECT_FALSE(res.setMarks(-2));
    EXPECT_EQ(res.getMarks(), 90);  // Không đổi
}

// Validation: valid case
TEST(CourseResultTest, Validate_Valid) {
    CourseResult res("SV001", "CS101", 75);
    auto result = res.validate();
    EXPECT_TRUE(hasNoErrors(result));
}

// Validation: invalid studentId, courseId, marks
TEST(CourseResultTest, Validate_Invalid) {
    CourseResult res1("", "CS101", 75);
    CourseResult res2("SV001", "", 75);
    CourseResult res3("SV001", "CS101", 150);
    CourseResult res4("SV001", "CS101", -2);

    EXPECT_FALSE(hasNoErrors(res1.validate()));
    EXPECT_FALSE(hasNoErrors(res2.validate()));
    EXPECT_FALSE(hasNoErrors(res3.validate()));
    EXPECT_FALSE(hasNoErrors(res4.validate()));
}

// toString output
TEST(CourseResultTest, ToStringOutput) {
    CourseResult res("SV001", "CS101", 88);
    std::string s = res.toString();
    EXPECT_NE(s.find("SV001"), std::string::npos);
    EXPECT_NE(s.find("CS101"), std::string::npos);
    EXPECT_NE(s.find("88"), std::string::npos);
    EXPECT_NE(s.find("B"), std::string::npos);
}
