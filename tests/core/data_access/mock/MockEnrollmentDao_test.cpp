#include <gtest/gtest.h>
#include "../src/core/data_access/mock/MockEnrollmentDao.h"
#include <algorithm>

EnrollmentRecord makeRecord(const std::string& sid, const std::string& cid) {
    return EnrollmentRecord{sid, cid};
}

TEST(MockEnrollmentDaoTest, EnrollAndFind) {
    MockEnrollmentDao dao;
    auto result = dao.addEnrollment("sv01", "cs101");
    EXPECT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto isEnrolledResult = dao.isEnrolled("sv01", "cs101");
    EXPECT_TRUE(isEnrolledResult.has_value());
    EXPECT_TRUE(isEnrolledResult.value());  // Đúng là đã đăng ký

    auto courses = dao.findCourseIdsByStudentId("sv01");
    EXPECT_TRUE(courses.has_value());
    EXPECT_NE(std::find(courses->begin(), courses->end(), "cs101"), courses->end());
}

TEST(MockEnrollmentDaoTest, Enroll_Duplicate) {
    MockEnrollmentDao dao;
    auto first = dao.addEnrollment("sv01", "cs101");
    EXPECT_TRUE(first.has_value());
    EXPECT_TRUE(first.value());

    auto second = dao.addEnrollment("sv01", "cs101");
    EXPECT_TRUE(second.has_value());
    EXPECT_FALSE(second.value()); // Không được phép trùng
}

TEST(MockEnrollmentDaoTest, FindEnrollment_NotFound) {
    MockEnrollmentDao dao;

    auto isEnrolledResult = dao.isEnrolled("non_exist", "no_course");
    EXPECT_TRUE(isEnrolledResult.has_value());
    EXPECT_FALSE(isEnrolledResult.value());  // Không có enrollment này
}


TEST(MockEnrollmentDaoTest, RemoveEnrollment) {
    MockEnrollmentDao dao;
    auto enroll = dao.addEnrollment("sv01", "cs101");
    EXPECT_TRUE(enroll.has_value());
    EXPECT_TRUE(enroll.value());

    auto removeResult = dao.removeEnrollment("sv01", "cs101");
    EXPECT_TRUE(removeResult.has_value());
    EXPECT_TRUE(removeResult.value());

    auto isEnrolledAfterRemove = dao.isEnrolled("sv01", "cs101");
    EXPECT_TRUE(isEnrolledAfterRemove.has_value());
    EXPECT_FALSE(isEnrolledAfterRemove.value());
}

TEST(MockEnrollmentDaoTest, RemoveEnrollment_NotExist) {
    MockEnrollmentDao dao;
    auto removeResult = dao.removeEnrollment("noone", "nothing");
    EXPECT_TRUE(removeResult.has_value());
    EXPECT_FALSE(removeResult.value());
}

TEST(MockEnrollmentDaoTest, GetAllEnrollments) {
    MockEnrollmentDao dao;
    dao.addEnrollment("sv01", "cs101");
    dao.addEnrollment("sv01", "cs102");
    dao.addEnrollment("sv02", "cs101");

    auto all = dao.getAllEnrollments();
    EXPECT_TRUE(all.has_value());
    EXPECT_EQ(all->size(), 3);
}

TEST(MockEnrollmentDaoTest, GetEnrollmentsByStudentId) {
    MockEnrollmentDao dao;
    dao.addEnrollment("sv01", "cs101");
    dao.addEnrollment("sv01", "cs102");
    dao.addEnrollment("sv02", "cs101");

    auto courses = dao.findCourseIdsByStudentId("sv01");
    EXPECT_TRUE(courses.has_value());
    EXPECT_EQ(courses->size(), 2);
    EXPECT_NE(std::find(courses->begin(), courses->end(), "cs101"), courses->end());
    EXPECT_NE(std::find(courses->begin(), courses->end(), "cs102"), courses->end());
}
