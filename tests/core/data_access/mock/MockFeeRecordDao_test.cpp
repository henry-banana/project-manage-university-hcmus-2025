#include <gtest/gtest.h>
#include "../../src/core/data_access/mock/MockFeeRecordDao.h"
#include "../../src/core/entities/FeeRecord.h"
#include "../src/common/ErrorType.h"

class MockFeeRecordDaoTest : public ::testing::Test {
protected:
    MockFeeRecordDao dao;
    FeeRecord sample{"SV001", 5000000, 1000000};

    void SetUp() override {
        dao.add(sample);
    }
};

TEST_F(MockFeeRecordDaoTest, AddFeeRecord_ShouldSucceed) {
    FeeRecord newRecord("SV002", 7000000, 2000000);
    auto result = dao.add(newRecord);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getStudentId(), "SV002");
}

TEST_F(MockFeeRecordDaoTest, GetById_ShouldReturnCorrectRecord) {
    auto result = dao.getById("SV001");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getStudentId(), "SV001");
    EXPECT_EQ(result->getTotalFee(), 5000000);
    EXPECT_EQ(result->getPaidFee(), 1000000);
}

TEST_F(MockFeeRecordDaoTest, GetById_NotFound_ShouldReturnError) {
    auto result = dao.getById("SV999");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockFeeRecordDaoTest, Exists_ShouldReturnTrueIfExists) {
    auto result = dao.exists("SV001");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(MockFeeRecordDaoTest, Exists_ShouldReturnFalseIfNotExists) {
    auto result = dao.exists("SV888");
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}

TEST_F(MockFeeRecordDaoTest, UpdateFeeRecord_ShouldSucceed) {
    FeeRecord updated("SV001", 6000000, 3000000);
    auto result = dao.update(updated);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto check = dao.getById("SV001");
    ASSERT_TRUE(check.has_value());
    EXPECT_EQ(check->getTotalFee(), 6000000);
    EXPECT_EQ(check->getPaidFee(), 3000000);
}

TEST_F(MockFeeRecordDaoTest, UpdateNonExistentFeeRecord_ShouldFail) {
    FeeRecord updated("SV777", 6000000, 1000000);
    auto result = dao.update(updated);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockFeeRecordDaoTest, Remove_ShouldSucceed) {
    auto result = dao.remove("SV001");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto check = dao.getById("SV001");
    ASSERT_FALSE(check.has_value());
}

TEST_F(MockFeeRecordDaoTest, RemoveNonExistent_ShouldFail) {
    auto result = dao.remove("SV000");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockFeeRecordDaoTest, GetAll_ShouldReturnAllRecords) {
    FeeRecord extra("SV002", 7000000, 2000000);
    dao.add(extra);

    auto result = dao.getAll();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 2);
}
