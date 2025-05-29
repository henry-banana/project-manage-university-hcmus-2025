#include <gtest/gtest.h>
#include "../src/core/data_access/mock/MockCourseDao.h"
#include "../src/core/entities/Course.h"
#include "../src/common/ErrorType.h"

// Test fixture
class MockCourseDaoTest : public ::testing::Test {
protected:
    MockCourseDao dao;

    Course sampleCourse = Course("NEW100", "Sample Course", 3, "CS");
};

TEST_F(MockCourseDaoTest, GetById_ExistingCourse_ReturnsCourse) {
    auto result = dao.getById("CS101");
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getId(), "CS101");
    EXPECT_EQ(result->getName(), "Introduction to Programming");
}

TEST_F(MockCourseDaoTest, GetById_NonExistingCourse_ReturnsError) {
    auto result = dao.getById("NOT_EXIST");
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockCourseDaoTest, GetAll_ReturnsAllCourses) {
    auto result = dao.getAll();
    EXPECT_TRUE(result.has_value());
    EXPECT_GE(result->size(), 2); // vì mock có ít nhất 2 course
}

TEST_F(MockCourseDaoTest, Add_NewCourse_Succeeds) {
    auto result = dao.add(sampleCourse);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result->getId(), sampleCourse.getId());
}

TEST_F(MockCourseDaoTest, Add_DuplicateCourse_ReturnsError) {
    Course existing("CS101", "Duplicate", 3, "CS");
    auto result = dao.add(existing);
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS);
}

TEST_F(MockCourseDaoTest, Update_ExistingCourse_Succeeds) {
    Course updated("CS101", "Updated Name", 4, "CS");
    auto result = dao.update(updated);
    EXPECT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
    
    auto getResult = dao.getById("CS101");
    EXPECT_TRUE(getResult.has_value());
    EXPECT_EQ(getResult->getName(), "Updated Name");
}

TEST_F(MockCourseDaoTest, Update_NonExistingCourse_ReturnsError) {
    auto result = dao.update(sampleCourse);
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockCourseDaoTest, Remove_ExistingCourse_Succeeds) {
    auto result = dao.remove("CS101");
    EXPECT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto getResult = dao.getById("CS101");
    EXPECT_FALSE(getResult.has_value());
}

TEST_F(MockCourseDaoTest, Remove_NonExistingCourse_ReturnsError) {
    auto result = dao.remove("NOT_EXIST");
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockCourseDaoTest, Exists_ExistingCourse_ReturnsTrue) {
    auto result = dao.exists("CS101");
    EXPECT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(MockCourseDaoTest, Exists_NonExistingCourse_ReturnsFalse) {
    auto result = dao.exists("NOT_EXIST");
    EXPECT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}

TEST_F(MockCourseDaoTest, FindByFacultyId_ReturnsCourses) {
    auto result = dao.findByFacultyId("CS");
    EXPECT_TRUE(result.has_value());
    EXPECT_FALSE(result->empty());
    for (const auto& c : *result) {
        EXPECT_EQ(c.getFacultyId(), "CS");
    }
}

TEST_F(MockCourseDaoTest, FindByFacultyId_NoCourses_ReturnsEmptyVector) {
    auto result = dao.findByFacultyId("NON_EXISTENT_FACULTY");
    EXPECT_TRUE(result.has_value());
    EXPECT_TRUE(result->empty());
}
