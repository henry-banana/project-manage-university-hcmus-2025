// --- START OF MODIFIED FILE tests/core/data_access/mock/MockFeeRecordDao_test.cpp ---
#include <gtest/gtest.h>
#include "../../../../src/core/data_access/mock/MockFeeRecordDao.h" // (➕)
#include "../../../../src/core/entities/FeeRecord.h"     // (➕)
#include "../../../../src/common/ErrorType.h"         // (➕)

class MockFeeRecordDaoTest : public ::testing::Test {
protected:
    MockFeeRecordDao dao;
    FeeRecord sampleToAdd{"SV_Test_Add", 5000000, 1000000};

    void SetUp() override {
        MockFeeRecordDao::clearMockData();
        MockFeeRecordDao::initializeDefaultMockData(); // Gọi data mặc định (S001, S002)
    }

    void TearDown() override {
        MockFeeRecordDao::clearMockData();
    }
};

TEST_F(MockFeeRecordDaoTest, AddFeeRecord_ShouldSucceed) {
    FeeRecord newRecord("SV002_Test", 7000000, 2000000); // ID khác với default
    auto result = dao.add(newRecord);
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_EQ(result->getStudentId(), "SV002_Test");
}

TEST_F(MockFeeRecordDaoTest, AddFeeRecord_DuplicateId_ShouldFail) {
    FeeRecord duplicateRecord("S001", 6000000, 0); // S001 đã có trong default data
    auto result = dao.add(duplicateRecord);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS);
}

TEST_F(MockFeeRecordDaoTest, GetById_ShouldReturnCorrectRecord) {
    auto result = dao.getById("S001"); // S001 có trong default data
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getStudentId(), "S001");
    EXPECT_EQ(result->getTotalFee(), 5000000);
}

TEST_F(MockFeeRecordDaoTest, GetById_NotFound_ShouldReturnError) {
    auto result = dao.getById("SV_NON_EXIST");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockFeeRecordDaoTest, Exists_ShouldReturnTrueIfExists) {
    auto result = dao.exists("S002"); // S002 có trong default data
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(MockFeeRecordDaoTest, Exists_ShouldReturnFalseIfNotExists) {
    auto result = dao.exists("SV_NON_EXIST_EXISTS");
    ASSERT_TRUE(result.has_value()); // exists không trả lỗi nếu không tìm thấy
    EXPECT_FALSE(result.value());
}

TEST_F(MockFeeRecordDaoTest, UpdateFeeRecord_ShouldSucceed) {
    FeeRecord updated("S001", 6000000, 3000000); // S001 có trong default data
    auto result = dao.update(updated);
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_TRUE(result.value());

    auto check = dao.getById("S001");
    ASSERT_TRUE(check.has_value());
    EXPECT_EQ(check->getTotalFee(), 6000000);
    EXPECT_EQ(check->getPaidFee(), 3000000);
}

TEST_F(MockFeeRecordDaoTest, UpdateNonExistentFeeRecord_ShouldFail) {
    FeeRecord updated("SV_NON_EXIST_UPD", 6000000, 1000000);
    auto result = dao.update(updated);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockFeeRecordDaoTest, Remove_ShouldSucceed) {
    auto result = dao.remove("S001"); // S001 có trong default data
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto check = dao.getById("S001");
    ASSERT_FALSE(check.has_value());
}

TEST_F(MockFeeRecordDaoTest, RemoveNonExistent_ShouldFail) {
    auto result = dao.remove("SV_NON_EXIST_REM");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockFeeRecordDaoTest, GetAll_ShouldReturnAllRecords) {
    // initializeDefaultMockData() đã thêm 2 records (S001, S002)
    FeeRecord extra("SV_ALL_EXTRA", 7000000, 2000000);
    dao.add(extra);

    auto result = dao.getAll();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 2 + 1); // 2 từ default + 1 mới thêm
}
// --- END OF MODIFIED FILE tests/core/data_access/mock/MockFeeRecordDao_test.cpp ---