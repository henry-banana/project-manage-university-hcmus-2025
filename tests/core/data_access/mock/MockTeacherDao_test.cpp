// --- START OF MODIFIED FILE tests/core/data_access/mock/MockTeacherDao_test.cpp ---
#include <gtest/gtest.h>
#include "../../../../src/core/data_access/mock/MockTeacherDao.h" // (➕)
#include "../../../../src/core/entities/Teacher.h"      // (➕)
#include "../../../../src/common/ErrorType.h"          // (➕)

class MockTeacherDaoTest : public ::testing::Test { // (➕)
protected:
    MockTeacherDao dao;

    Teacher makeSampleTeacher(const std::string& id = "GV_TEST_001", 
                              const std::string& email = "test.teacher@example.com") {
        Teacher t(id, "TestTeacherFirst", "TestTeacherLast", "IT_Test_Faculty");
        t.setEmail(email);
        t.setQualification("PhD");
        t.setDesignation("Professor");
        return t;
    }

    void SetUp() override {
        MockTeacherDao::clearMockData();
        // MockTeacherDao::initializeDefaultMockData(); // Gọi nếu muốn data mặc định
    }

    void TearDown() override {
        MockTeacherDao::clearMockData();
    }
};

TEST_F(MockTeacherDaoTest, AddAndGetById) {
    Teacher t = makeSampleTeacher("GV001_ADD");
    auto addResult = dao.add(t);
    ASSERT_TRUE(addResult.has_value()) << addResult.error().message;

    auto getResult = dao.getById("GV001_ADD");
    ASSERT_TRUE(getResult.has_value());
    EXPECT_EQ(getResult->getId(), "GV001_ADD");
    EXPECT_EQ(getResult->getQualification(), "PhD");
}

TEST_F(MockTeacherDaoTest, GetById_NotFound) {
    auto result = dao.getById("NOT_EXIST_TEACHER");
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockTeacherDaoTest, Exists_WhenPresent) {
    dao.add(makeSampleTeacher("GV_EXISTS"));
    auto existsResult = dao.exists("GV_EXISTS");
    ASSERT_TRUE(existsResult.has_value());
    EXPECT_TRUE(existsResult.value());
}

TEST_F(MockTeacherDaoTest, Exists_WhenNotPresent) {
    auto notExistsResult = dao.exists("GV_NO_ID_EXISTS");
    ASSERT_TRUE(notExistsResult.has_value());
    EXPECT_FALSE(notExistsResult.value());
}

TEST_F(MockTeacherDaoTest, UpdateTeacher_Success) {
    Teacher t_orig = makeSampleTeacher("GV_UPDATE");
    dao.add(t_orig);

    Teacher t_updated = makeSampleTeacher("GV_UPDATE");
    t_updated.setQualification("Doctorate");
    t_updated.setDesignation("Associate Professor");
    auto updateResult = dao.update(t_updated);
    ASSERT_TRUE(updateResult.has_value()) << updateResult.error().message;
    EXPECT_TRUE(updateResult.value());

    auto updated = dao.getById("GV_UPDATE");
    ASSERT_TRUE(updated.has_value());
    EXPECT_EQ(updated->getQualification(), "Doctorate");
    EXPECT_EQ(updated->getDesignation(), "Associate Professor");
}

TEST_F(MockTeacherDaoTest, RemoveTeacher_Success) {
    dao.add(makeSampleTeacher("GV_REMOVE"));
    auto removeResult = dao.remove("GV_REMOVE");
    ASSERT_TRUE(removeResult.has_value());
    EXPECT_TRUE(removeResult.value());

    auto getResult = dao.getById("GV_REMOVE");
    EXPECT_FALSE(getResult.has_value());
}

TEST_F(MockTeacherDaoTest, FindByEmail_Success) {
    dao.add(makeSampleTeacher("GV_EMAIL_T1", "teacher.find@example.com"));
    auto result = dao.findByEmail("teacher.find@example.com");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getId(), "GV_EMAIL_T1");
}

TEST_F(MockTeacherDaoTest, FindByFacultyId_Success) {
    dao.add(Teacher("T_IT_1", "A", "A", "IT", LoginStatus::ACTIVE));
    dao.add(Teacher("T_CS_1", "B", "B", "CS", LoginStatus::ACTIVE));
    dao.add(Teacher("T_IT_2", "C", "C", "IT", LoginStatus::ACTIVE));

    auto result = dao.findByFacultyId("IT");
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 2);
}

TEST_F(MockTeacherDaoTest, GetAll_WhenPopulated) {
    // MockTeacherDao::initializeDefaultMockData(); // Gọi nếu hàm này thêm 3 teacher
    // size_t defaultSize = 3;
    size_t defaultSize = 0; // Nếu SetUp chỉ clear
    dao.add(makeSampleTeacher("GV_ALL_1", "all1@example.com"));
    dao.add(makeSampleTeacher("GV_ALL_2", "all2@example.com"));

    auto result = dao.getAll();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), defaultSize + 2);
}
// --- END OF MODIFIED FILE tests/core/data_access/mock/MockTeacherDao_test.cpp ---