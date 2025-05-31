// --- START OF MODIFIED FILE tests/core/data_access/mock/MockEnrollmentDao_test.cpp ---
#include <gtest/gtest.h>
#include "../../../../src/core/data_access/mock/MockEnrollmentDao.h" // (➕)
#include "../../../../src/common/ErrorType.h"                      // (➕)
#include <algorithm>

// EnrollmentRecord đã được define trong IEnrollmentDao.h

class MockEnrollmentDaoTest : public ::testing::Test { // (➕)
protected:
    MockEnrollmentDao dao;

    void SetUp() override {
        MockEnrollmentDao::clearMockData();
        // MockEnrollmentDao::initializeDefaultMockData(); // Gọi nếu muốn data mặc định
    }
    void TearDown() override {
        MockEnrollmentDao::clearMockData();
    }
};

TEST_F(MockEnrollmentDaoTest, EnrollAndIsEnrolled) { // Gộp EnrollAndFind
    auto result = dao.addEnrollment("sv01_test", "cs101_test");
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_TRUE(result.value());

    auto isEnrolledResult = dao.isEnrolled("sv01_test", "cs101_test");
    ASSERT_TRUE(isEnrolledResult.has_value());
    EXPECT_TRUE(isEnrolledResult.value()); 
}

TEST_F(MockEnrollmentDaoTest, Enroll_Duplicate_ShouldFail) {
    dao.addEnrollment("sv_dup", "cs_dup"); // Add lần 1

    auto second_result = dao.addEnrollment("sv_dup", "cs_dup"); // Add lần 2
    ASSERT_FALSE(second_result.has_value()); 
    EXPECT_EQ(second_result.error().code, ErrorCode::ALREADY_EXISTS);
}

TEST_F(MockEnrollmentDaoTest, IsEnrolled_NotFound_ReturnsFalse) { // Đổi tên từ FindEnrollment_NotFound
    auto isEnrolledResult = dao.isEnrolled("non_exist_stud", "no_course_exist");
    ASSERT_TRUE(isEnrolledResult.has_value());
    EXPECT_FALSE(isEnrolledResult.value());
}


TEST_F(MockEnrollmentDaoTest, RemoveEnrollment_Success) {
    dao.addEnrollment("sv_remove", "cs_remove");
    
    auto removeResult = dao.removeEnrollment("sv_remove", "cs_remove");
    ASSERT_TRUE(removeResult.has_value()) << removeResult.error().message;
    EXPECT_TRUE(removeResult.value());

    auto isEnrolledAfterRemove = dao.isEnrolled("sv_remove", "cs_remove");
    ASSERT_TRUE(isEnrolledAfterRemove.has_value());
    EXPECT_FALSE(isEnrolledAfterRemove.value());
}

TEST_F(MockEnrollmentDaoTest, RemoveEnrollment_NotExist_ShouldFail) {
    auto removeResult = dao.removeEnrollment("noone_remove", "nothing_remove");
    ASSERT_FALSE(removeResult.has_value());
    EXPECT_EQ(removeResult.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockEnrollmentDaoTest, GetAllEnrollments_Populated) {
    dao.addEnrollment("sv_all_1", "cs_all_1");
    dao.addEnrollment("sv_all_1", "cs_all_2");
    dao.addEnrollment("sv_all_2", "cs_all_1");

    auto all = dao.getAllEnrollments();
    ASSERT_TRUE(all.has_value());
    EXPECT_EQ(all->size(), 3);
}

TEST_F(MockEnrollmentDaoTest, GetAllEnrollments_Empty) {
    auto all = dao.getAllEnrollments();
    ASSERT_TRUE(all.has_value());
    EXPECT_TRUE(all->empty());
}


TEST_F(MockEnrollmentDaoTest, FindCourseIdsByStudentId_Success) { // Đổi tên từ GetEnrollmentsByStudentId
    dao.addEnrollment("sv_find_c_1", "cs_find_c_101");
    dao.addEnrollment("sv_find_c_1", "cs_find_c_102");
    dao.addEnrollment("sv_find_c_2", "cs_find_c_101");

    auto courses = dao.findCourseIdsByStudentId("sv_find_c_1");
    ASSERT_TRUE(courses.has_value());
    EXPECT_EQ(courses->size(), 2);
    EXPECT_NE(std::find(courses->begin(), courses->end(), "cs_find_c_101"), courses->end());
    EXPECT_NE(std::find(courses->begin(), courses->end(), "cs_find_c_102"), courses->end());
}

TEST_F(MockEnrollmentDaoTest, FindCourseIdsByStudentId_NoEnrollments_ReturnsEmpty) {
    auto courses = dao.findCourseIdsByStudentId("sv_no_enroll");
    ASSERT_TRUE(courses.has_value());
    EXPECT_TRUE(courses->empty());
}
// --- END OF MODIFIED FILE tests/core/data_access/mock/MockEnrollmentDao_test.cpp ---