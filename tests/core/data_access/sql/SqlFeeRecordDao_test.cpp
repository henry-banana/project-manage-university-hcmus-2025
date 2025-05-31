#include <gtest/gtest.h>
#include <memory>
#include "core/data_access/sql/SqlFeeRecordDao.h"
#include "core/entities/FeeRecord.h"
#include "core/database_adapter/sql/SQLiteAdapter.h"
#include "core/parsing/impl_sql_parser/FeeRecordSqlParser.h" 

class SqlFeeRecordDaoTest : public ::testing::Test {
protected:
    std::shared_ptr<SQLiteAdapter> dbAdapter;
    std::shared_ptr<FeeRecordSqlParser> parser;
    std::unique_ptr<SqlFeeRecordDao> dao;

    void SetUp() override {
        dbAdapter = std::make_shared<SQLiteAdapter>();
        dbAdapter->connect(":memory:");
        parser = std::make_shared<FeeRecordSqlParser>();
        dao = std::make_unique<SqlFeeRecordDao>(dbAdapter, parser);

        auto result = dbAdapter->executeUpdate(R"(
            CREATE TABLE FeeRecords (
                studentId TEXT PRIMARY KEY,
                totalFee INTEGER NOT NULL,
                paidFee INTEGER NOT NULL
            );
        )");
        ASSERT_TRUE(result.has_value());
    }
};

TEST_F(SqlFeeRecordDaoTest, AddAndGetFeeRecord_Success) {
    FeeRecord record("SV001", 5000000, 2000000);
    auto added = dao->add(record);
    ASSERT_TRUE(added.has_value());
    EXPECT_EQ(added->getStudentId(), "SV001");
    EXPECT_EQ(added->getTotalFee(), 5000000);
    EXPECT_EQ(added->getPaidFee(), 2000000);

    auto fetched = dao->getById("SV001");
    ASSERT_TRUE(fetched.has_value());
    EXPECT_EQ(fetched->getStudentId(), "SV001");
    EXPECT_EQ(fetched->getDueFee(), 3000000);
}

TEST_F(SqlFeeRecordDaoTest, UpdateFeeRecord_Success) {
    dao->add(FeeRecord("SV002", 6000000, 1000000));
    FeeRecord updated("SV002", 6000000, 3000000);
    auto result = dao->update(updated);
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result.value());

    auto fetched = dao->getById("SV002");
    ASSERT_TRUE(fetched.has_value());
    EXPECT_EQ(fetched->getPaidFee(), 3000000);
}

TEST_F(SqlFeeRecordDaoTest, RemoveFeeRecord_Success) {
    dao->add(FeeRecord("SV003", 4000000, 1500000));
    auto removed = dao->remove("SV003");
    ASSERT_TRUE(removed.has_value());
    ASSERT_TRUE(removed.value());

    auto fetched = dao->getById("SV003");
    ASSERT_FALSE(fetched.has_value());
}

TEST_F(SqlFeeRecordDaoTest, ExistsCheck) {
    dao->add(FeeRecord("SV004", 7000000, 7000000));
    auto exists = dao->exists("SV004");
    ASSERT_TRUE(exists.has_value());
    EXPECT_TRUE(exists.value());

    auto notExists = dao->exists("SV999");
    ASSERT_TRUE(notExists.has_value());
    EXPECT_FALSE(notExists.value());
}

TEST_F(SqlFeeRecordDaoTest, GetAllFeeRecords) {
    dao->add(FeeRecord("SV005", 3000000, 1000000));
    dao->add(FeeRecord("SV006", 5000000, 5000000));

    auto records = dao->getAll();
    ASSERT_TRUE(records.has_value());
    EXPECT_EQ(records->size(), 2);
}
