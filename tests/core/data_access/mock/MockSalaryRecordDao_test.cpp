#include <gtest/gtest.h>
#include "../../src/core/data_access/mock/MockSalaryRecordDao.h"
#include "../../src/core/entities/SalaryRecord.h"
#include "../../src/common/ErrorType.h"

class MockSalaryRecordDaoTest : public ::testing::Test {
protected:
    MockSalaryRecordDao dao;
    SalaryRecord sample{"GV001", 10000000};

    void SetUp() override {
        dao.add(sample);
    }
};

TEST_F(MockSalaryRecordDaoTest, AddSalaryRecord_ShouldSucceed) {
    SalaryRecord newRecord("GV002", 12000000);
    auto result = dao.add(newRecord);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTeacherId(), "GV002");
    EXPECT_EQ(result->getBasicMonthlyPay(), 12000000);
}

TEST_F(MockSalaryRecordDaoTest, AddDuplicate_ShouldReturnError) {
    SalaryRecord dup("GV001", 9999999);
    auto result = dao.add(dup);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS);
}

TEST_F(MockSalaryRecordDaoTest, GetById_Exists_ShouldReturnCorrectRecord) {
    auto result = dao.getById("GV001");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTeacherId(), "GV001");
    EXPECT_EQ(result->getBasicMonthlyPay(), 10000000);
}

TEST_F(MockSalaryRecordDaoTest, GetById_NotExists_ShouldReturnError) {
    auto result = dao.getById("GV999");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockSalaryRecordDaoTest, Exists_ShouldReturnTrue) {
    auto result = dao.exists("GV001");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(MockSalaryRecordDaoTest, Exists_ShouldReturnFalse) {
    auto result = dao.exists("GV888");
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}

TEST_F(MockSalaryRecordDaoTest, Update_ShouldSucceed) {
    SalaryRecord updated("GV001", 11000000);
    auto result = dao.update(updated);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto check = dao.getById("GV001");
    ASSERT_TRUE(check.has_value());
    EXPECT_EQ(check->getBasicMonthlyPay(), 11000000);
}

TEST_F(MockSalaryRecordDaoTest, UpdateNonexistent_ShouldFail) {
    SalaryRecord updated("GV777", 9000000);
    auto result = dao.update(updated);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockSalaryRecordDaoTest, Remove_ShouldSucceed) {
    auto result = dao.remove("GV001");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto check = dao.getById("GV001");
    ASSERT_FALSE(check.has_value());
}

TEST_F(MockSalaryRecordDaoTest, RemoveNonexistent_ShouldFail) {
    auto result = dao.remove("GV000");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockSalaryRecordDaoTest, GetAll_ShouldReturnAllRecords) {
    dao.add(SalaryRecord("GV003", 9500000));
    dao.add(SalaryRecord("GV004", 13000000));

    auto result = dao.getAll();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 3);  // GV001, GV003, GV004
}
