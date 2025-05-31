// --- START OF MODIFIED FILE tests/core/data_access/mock/MockCourseDao_test.cpp ---
#include <gtest/gtest.h>
#include "../../../../src/core/data_access/mock/MockCourseDao.h" // (➕)
#include "../../../../src/core/entities/Course.h"      // (➕)
#include "../../../../src/common/ErrorType.h"          // (➕)


class MockCourseDaoTest : public ::testing::Test {
protected:
    MockCourseDao dao;

    Course sampleCourse = Course("NEW100_Test", "Sample Course For Test", 3, "CS_Test");

    void SetUp() override {
        MockCourseDao::clearMockData();
        MockCourseDao::initializeDefaultMockData(); // Gọi data mặc định
    }

    void TearDown() override {
        MockCourseDao::clearMockData();
    }
};

TEST_F(MockCourseDaoTest, GetById_ExistingCourse_ReturnsCourse) {
    // Dữ liệu đã được init trong SetUp
    auto result = dao.getById("CS101"); // Giả sử CS101 có trong initializeDefaultMockData
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_EQ(result->getId(), "CS101");
    EXPECT_EQ(result->getName(), "Introduction to Programming");
}

TEST_F(MockCourseDaoTest, GetById_NonExistingCourse_ReturnsError) {
    auto result = dao.getById("NOT_EXIST_COURSE");
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockCourseDaoTest, GetAll_ReturnsAllCourses) {
    // MockCourseDao::initializeDefaultMockData() đã thêm 5 courses
    // Thêm 1 course nữa
    dao.add(Course("TEST999", "TestGetAll", 1, "IT"));
    auto result = dao.getAll();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 5 + 1); 
}

TEST_F(MockCourseDaoTest, Add_NewCourse_Succeeds) {
    auto result = dao.add(sampleCourse); // sampleCourse là "NEW100_Test"
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_EQ(result->getId(), sampleCourse.getId());

    auto fetched = dao.getById(sampleCourse.getId());
    ASSERT_TRUE(fetched.has_value());
    EXPECT_EQ(fetched->getName(), sampleCourse.getName());
}

TEST_F(MockCourseDaoTest, Add_DuplicateCourseId_ReturnsError) {
    Course existing("CS101", "Duplicate Name", 3, "CS_Test_Dup"); // CS101 đã có trong default data
    auto result = dao.add(existing);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS);
}

TEST_F(MockCourseDaoTest, Update_ExistingCourse_Succeeds) {
    Course updated("IT202", "Updated DSA Name", 4, "IT_Updated"); // IT202 có trong default
    auto result = dao.update(updated);
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_TRUE(result.value());
    
    auto getResult = dao.getById("IT202");
    ASSERT_TRUE(getResult.has_value());
    EXPECT_EQ(getResult->getName(), "Updated DSA Name");
    EXPECT_EQ(getResult->getFacultyId(), "IT_Updated");
}

TEST_F(MockCourseDaoTest, Update_NonExistingCourse_ReturnsError) {
    // sampleCourse("NEW100_Test") chưa được add
    auto result = dao.update(sampleCourse);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockCourseDaoTest, Remove_ExistingCourse_Succeeds) {
    auto result = dao.remove("EE301"); // EE301 có trong default
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto getResult = dao.getById("EE301");
    EXPECT_FALSE(getResult.has_value());
}

TEST_F(MockCourseDaoTest, Remove_NonExistingCourse_ReturnsError) {
    auto result = dao.remove("NOT_EXIST_REMOVE");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockCourseDaoTest, Exists_ExistingCourse_ReturnsTrue) {
    auto result = dao.exists("POT101"); // POT101 có trong default
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(MockCourseDaoTest, Exists_NonExistingCourse_ReturnsFalse) {
    auto result = dao.exists("NON_EXISTENT_COURSE_EXISTS");
    ASSERT_TRUE(result.has_value()); // exists không nên trả error nếu không tìm thấy
    EXPECT_FALSE(result.value());
}

TEST_F(MockCourseDaoTest, FindByFacultyId_ReturnsCourses) {
    // initializeDefaultMockData có CS101 (CS), IT202 (IT), EE301 (EE), POT101(CHEM), TRN101(TRAN)
    auto result = dao.findByFacultyId("CS");
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 1); // Chỉ có CS101
    for (const auto& c : *result) {
        EXPECT_EQ(c.getFacultyId(), "CS");
    }

    auto resultCHEM = dao.findByFacultyId("CHEM");
    ASSERT_TRUE(resultCHEM.has_value());
    ASSERT_EQ(resultCHEM->size(), 1);
    EXPECT_EQ(resultCHEM->at(0).getId(), "POT101");
}

TEST_F(MockCourseDaoTest, FindByFacultyId_NoCourses_ReturnsEmptyVector) {
    auto result = dao.findByFacultyId("NON_EXISTENT_FACULTY_COURSE");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->empty());
}
// --- END OF MODIFIED FILE tests/core/data_access/mock/MockCourseDao_test.cpp ---