#include <gtest/gtest.h>
#include "../src/core/data_access/mock/MockStudentDao.h"

// Hàm tạo sinh viên mẫu để test
Student makeSampleStudent() {
    return Student("SV001", "Van", "Nguyen", "CNTT", LoginStatus::ACTIVE);
}

TEST(MockStudentDaoTest, AddStudent_ShouldReturnSameStudent) {
    MockStudentDao dao;
    Student s = makeSampleStudent();
    
    auto result = dao.add(s);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getId(), "SV001");
    EXPECT_EQ(result->getFirstName(), "Van");
    EXPECT_EQ(result->getLastName(), "Nguyen");
    EXPECT_EQ(result->getFacultyId(), "CNTT");
    EXPECT_EQ(result->getStatus(), LoginStatus::ACTIVE);
}

TEST(MockStudentDaoTest, GetById_ShouldReturnStudentIfExists) {
    MockStudentDao dao;
    Student s = makeSampleStudent();
    dao.add(s);

    auto result = dao.getById("SV001");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getLastName(), "Nguyen");
}

TEST(MockStudentDaoTest, Exists_ShouldReturnTrueIfStudentExists) {
    MockStudentDao dao;
    dao.add(makeSampleStudent());

    auto result = dao.exists("SV001");

    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST(MockStudentDaoTest, RemoveStudent_ShouldReturnTrue) {
    MockStudentDao dao;
    dao.add(makeSampleStudent());

    auto result = dao.remove("SV001");

    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto check = dao.exists("SV001");
    ASSERT_TRUE(check.has_value());
    EXPECT_FALSE(check.value());
}
