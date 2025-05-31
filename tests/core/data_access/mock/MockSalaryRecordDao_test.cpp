// --- START OF MODIFIED FILE tests/core/data_access/mock/MockSalaryRecordDao_test.cpp ---
#include <gtest/gtest.h>
#include "../../../../src/core/data_access/mock/MockSalaryRecordDao.h" // (➕)
#include "../../../../src/core/entities/SalaryRecord.h"     // (➕)
#include "../../../../src/common/ErrorType.h"         // (➕)

class MockSalaryRecordDaoTest : public ::testing::Test {
protected:
    MockSalaryRecordDao dao;
    // Sample không cần khởi tạo ở đây nữa nếu SetUp sẽ thêm
    // SalaryRecord sample{"GV001_Setup", 10000000};

    void SetUp() override {
        MockSalaryRecordDao::clearMockData();
        MockSalaryRecordDao::initializeDefaultMockData(); // Load T001, T002
        // Nếu muốn thêm sample cụ thể cho test suite này:
        // dao.add(SalaryRecord{"GV_SETUP_001", 10000000});
    }
    void TearDown() override {
        MockSalaryRecordDao::clearMockData();
    }
};

TEST_F(MockSalaryRecordDaoTest, AddSalaryRecord_ShouldSucceed) {
    SalaryRecord newRecord("GV002_Test", 12000000); // ID khác với default
    auto result = dao.add(newRecord);
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_EQ(result->getTeacherId(), "GV002_Test");
}

TEST_F(MockSalaryRecordDaoTest, AddDuplicate_ShouldReturnError) {
    SalaryRecord dup("T001", 9999999); // T001 đã có trong default
    auto result = dao.add(dup);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS);
}

TEST_F(MockSalaryRecordDaoTest, GetById_Exists_ShouldReturnCorrectRecord) {
    auto result = dao.getById("T001"); // T001 từ default
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTeacherId(), "T001");
    EXPECT_EQ(result->getBasicMonthlyPay(), 15000000);
}

TEST_F(MockSalaryRecordDaoTest, GetById_NotExists_ShouldReturnError) {
    auto result = dao.getById("GV_NON_EXIST");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockSalaryRecordDaoTest, Exists_ShouldReturnTrue) {
    auto result = dao.exists("T002"); // T002 từ default
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(MockSalaryRecordDaoTest, Exists_ShouldReturnFalse) {
    auto result = dao.exists("GV_NON_EXIST_EXISTS");
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}

TEST_F(MockSalaryRecordDaoTest, Update_ShouldSucceed) {
    SalaryRecord updated("T001", 11000000); // T001 từ default
    auto result = dao.update(updated);
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_TRUE(result.value());

    auto check = dao.getById("T001");
    ASSERT_TRUE(check.has_value());
    EXPECT_EQ(check->getBasicMonthlyPay(), 11000000);
}

TEST_F(MockSalaryRecordDaoTest, UpdateNonexistent_ShouldFail) {
    SalaryRecord updated("GV_NON_EXIST_UPD", 9000000);
    auto result = dao.update(updated);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockSalaryRecordDaoTest, Remove_ShouldSucceed) {
    auto result = dao.remove("T001"); // T001 từ default
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto check = dao.getById("T001");
    ASSERT_FALSE(check.has_value());
}

TEST_F(MockSalaryRecordDaoTest, RemoveNonexistent_ShouldFail) {
    auto result = dao.remove("GV_NON_EXIST_REM");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockSalaryRecordDaoTest, GetAll_ShouldReturnAllRecords) {
    // initializeDefaultMockData() đã thêm 2 records (T001, T002)
    dao.add(SalaryRecord("GV003_TestGetAll", 9500000));
    dao.add(SalaryRecord("GV004_TestGetAll", 13000000));

    auto result = dao.getAll();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->size(), 2 + 2); 
}
// --- END OF MODIFIED FILE tests/core/data_access/mock/MockSalaryRecordDao_test.cpp ---