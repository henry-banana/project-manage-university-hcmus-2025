#include <gtest/gtest.h>
#include "../src/core/data_access/mock/MockTeacherDao.h"
#include "../src/core/entities/Teacher.h"

// Khởi tạo dữ liệu mẫu
Teacher makeSampleTeacher() {
    Teacher teacher("GV001", "An", "Nguyen", "KHTN");
    teacher.setQualification("PhD");
    teacher.setDesignation("Professor");
    teacher.setSpecializationSubjects("Computer Science");
    teacher.setExperienceYears(10);
    return teacher;
}

// Test thêm và lấy giảng viên
TEST(MockTeacherDaoTest, AddAndGetById) {
    MockTeacherDao dao;
    Teacher t = makeSampleTeacher();

    auto addResult = dao.add(t);
    EXPECT_TRUE(addResult.has_value());

    auto getResult = dao.getById("GV001");
    EXPECT_TRUE(getResult.has_value());
    EXPECT_EQ(getResult->getId(), "GV001");
    EXPECT_EQ(getResult->getFirstName(), "An");
    EXPECT_EQ(getResult->getQualification(), "PhD");
    EXPECT_EQ(getResult->getDesignation(), "Professor");
}

// Test lấy giảng viên không tồn tại
TEST(MockTeacherDaoTest, GetById_NotFound) {
    MockTeacherDao dao;
    auto result = dao.getById("NOT_EXIST");
    EXPECT_FALSE(result.has_value());
}

// Test kiểm tra tồn tại
TEST(MockTeacherDaoTest, Exists) {
    MockTeacherDao dao;
    Teacher t = makeSampleTeacher();
    dao.add(t);

    auto existsResult = dao.exists("GV001");
    EXPECT_TRUE(existsResult.has_value());
    EXPECT_TRUE(existsResult.value());

    auto notExistsResult = dao.exists("NO_ID");
    EXPECT_TRUE(notExistsResult.has_value());
    EXPECT_FALSE(notExistsResult.value());
}

// Test update
TEST(MockTeacherDaoTest, UpdateTeacher) {
    MockTeacherDao dao;
    Teacher t = makeSampleTeacher();
    dao.add(t);

    t.setQualification("MSc");
    auto updateResult = dao.update(t);
    EXPECT_TRUE(updateResult.has_value());
    EXPECT_TRUE(updateResult.value());

    auto updated = dao.getById("GV001");
    EXPECT_TRUE(updated.has_value());
    EXPECT_EQ(updated->getQualification(), "MSc");
}

// Test remove
TEST(MockTeacherDaoTest, RemoveTeacher) {
    MockTeacherDao dao;
    Teacher t = makeSampleTeacher();
    dao.add(t);

    auto removeResult = dao.remove("GV001");
    EXPECT_TRUE(removeResult.has_value());
    EXPECT_TRUE(removeResult.value());

    auto getResult = dao.getById("GV001");
    EXPECT_FALSE(getResult.has_value());
}
