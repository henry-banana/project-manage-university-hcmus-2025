#include <gtest/gtest.h>
#include "../src/core/data_access/mock/MockFacultyDao.h"
#include "../src/core/entities/Faculty.h"

Faculty makeSampleFaculty(const std::string& id = "IT", const std::string& name = "Information Technology") {
    return Faculty(id, name);
}

class FacultyDaoTest : public ::testing::Test {
protected:
    void SetUp() override {
        MockFacultyDao::resetMockData();
    }
};

TEST_F(FacultyDaoTest, AddAndGetById) {
    MockFacultyDao dao;
    Faculty f = makeSampleFaculty();
    auto addResult = dao.add(f);
    ASSERT_TRUE(addResult.has_value());

    auto getResult = dao.getById("IT");
    ASSERT_TRUE(getResult.has_value());
    EXPECT_EQ(getResult->getId(), "IT");
    EXPECT_EQ(getResult->getName(), "Information Technology");
}

TEST_F(FacultyDaoTest, GetById_NotFound) {
    MockFacultyDao dao;
    auto result = dao.getById("NON_EXIST");
    EXPECT_FALSE(result.has_value());
}

TEST_F(FacultyDaoTest, Exists) {
    MockFacultyDao dao;
    dao.add(makeSampleFaculty());

    auto exist = dao.exists("IT");
    ASSERT_TRUE(exist.has_value());
    EXPECT_TRUE(exist.value());

    auto notExist = dao.exists("XYZ");
    ASSERT_TRUE(notExist.has_value());
    EXPECT_FALSE(notExist.value());
}

TEST_F(FacultyDaoTest, UpdateFaculty) {
    MockFacultyDao dao;
    dao.add(makeSampleFaculty());

    Faculty updated("IT", "New Name");
    auto updateResult = dao.update(updated);
    ASSERT_TRUE(updateResult.has_value());
    EXPECT_TRUE(updateResult.value());

    auto getResult = dao.getById("IT");
    ASSERT_TRUE(getResult.has_value());
    EXPECT_EQ(getResult->getName(), "New Name");
}

TEST_F(FacultyDaoTest, RemoveFaculty) {
    MockFacultyDao dao;
    dao.add(makeSampleFaculty());

    auto removeResult = dao.remove("IT");
    ASSERT_TRUE(removeResult.has_value());
    EXPECT_TRUE(removeResult.value());

    auto getResult = dao.getById("IT");
    EXPECT_FALSE(getResult.has_value());
}

TEST_F(FacultyDaoTest, FindByName) {
    MockFacultyDao dao;
    dao.add(makeSampleFaculty("IT", "InfoTech"));
    dao.add(makeSampleFaculty("EE", "Electrical"));

    auto found = dao.findByName("Electrical");
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->getId(), "EE");
}

TEST_F(FacultyDaoTest, GetAll) {
    MockFacultyDao dao;
    dao.add(makeSampleFaculty("IT", "InfoTech"));
    dao.add(makeSampleFaculty("EE", "Electrical"));

    auto all = dao.getAll();
    ASSERT_TRUE(all.has_value());
    EXPECT_EQ(all->size(), 2);
}
