#include "gtest/gtest.h"
#include "../../../../src/core/data_access/sql/SqlSalaryRecordDao.h"
#include "../../../../src/core/entities/SalaryRecord.h"
#include "../../../../src/core/database_adapter/sql/SQLiteAdapter.h"
#include "../../../../src/core/parsing/impl_sql_parser/SalaryRecordSqlParser.h"
#include "../../../../src/common/ErrorType.h"

class SqlSalaryRecordDaoTest : public ::testing::Test {
protected:
    std::shared_ptr<IDatabaseAdapter> dbAdapter;
    std::shared_ptr<IEntityParser<SalaryRecord, DbQueryResultRow>> salaryParser;
    std::unique_ptr<SqlSalaryRecordDao> salaryDao;

    void SetUp() override {
        dbAdapter = std::make_shared<SQLiteAdapter>();
        auto connectRes = dbAdapter->connect(":memory:");
        ASSERT_TRUE(connectRes.has_value() && connectRes.value()) << connectRes.error().message;

        auto ensureRes = std::static_pointer_cast<SQLiteAdapter>(dbAdapter)->ensureTablesExist();
        ASSERT_TRUE(ensureRes.has_value() && ensureRes.value()) << ensureRes.error().message;

        salaryParser = std::make_shared<SalaryRecordSqlParser>();
        salaryDao = std::make_unique<SqlSalaryRecordDao>(dbAdapter, salaryParser);

        clearTable();
    }

    void TearDown() override {
        if (dbAdapter && dbAdapter->isConnected()) {
            dbAdapter->disconnect();
        }
    }

    void clearTable() {
        auto res = dbAdapter->executeUpdate("DELETE FROM SalaryRecords;");
        ASSERT_TRUE(res.has_value()) << res.error().message;
    }

    SalaryRecord insertDirectly(const std::string& teacherId, long pay) {
        SalaryRecord record(teacherId, pay);
        auto paramsOpt = salaryParser->toQueryInsertParams(record);
        EXPECT_TRUE(paramsOpt.has_value());
        auto res = dbAdapter->executeUpdate(
            "INSERT INTO SalaryRecords (teacherId, basicMonthlyPay) VALUES (?, ?);",
            paramsOpt.value()
        );
        EXPECT_TRUE(res.has_value());
        return record;
    }
};

// Tests

TEST_F(SqlSalaryRecordDaoTest, Add_Success) {
    SalaryRecord r("GV01", 1500);
    auto res = salaryDao->add(r);
    ASSERT_TRUE(res.has_value()) << res.error().message;
    EXPECT_EQ(res.value().getTeacherId(), "GV01");
    EXPECT_EQ(res.value().getBasicMonthlyPay(), 1500);
}

TEST_F(SqlSalaryRecordDaoTest, Add_Duplicate_Fails) {
    insertDirectly("GV02", 2000);
    SalaryRecord r("GV02", 2500);
    auto res = salaryDao->add(r);
    ASSERT_FALSE(res.has_value());
    EXPECT_EQ(res.error().code, ErrorCode::DB_FOREIGN_KEY_ERROR);
}

TEST_F(SqlSalaryRecordDaoTest, GetById_Existing_ReturnsRecord) {
    insertDirectly("GV03", 3000);
    auto res = salaryDao->getById("GV03");
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(res.value().getTeacherId(), "GV03");
    EXPECT_EQ(res.value().getBasicMonthlyPay(), 3000);
}

TEST_F(SqlSalaryRecordDaoTest, GetById_NonExisting_ReturnsError) {
    auto res = salaryDao->getById("UNKNOWN");
    ASSERT_FALSE(res.has_value());
    EXPECT_EQ(res.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlSalaryRecordDaoTest, GetAll_Empty_ReturnsEmptyVector) {
    auto res = salaryDao->getAll();
    ASSERT_TRUE(res.has_value());
    EXPECT_TRUE(res.value().empty());
}

TEST_F(SqlSalaryRecordDaoTest, GetAll_WithData_ReturnsAllRecords) {
    insertDirectly("GV01", 1000);
    insertDirectly("GV02", 2000);
    auto res = salaryDao->getAll();
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(res.value().size(), 2);
}

TEST_F(SqlSalaryRecordDaoTest, Update_Success) {
    insertDirectly("GV04", 1200);
    SalaryRecord updated("GV04", 1800);
    auto res = salaryDao->update(updated);
    ASSERT_TRUE(res.has_value());
    EXPECT_TRUE(res.value());

    auto check = salaryDao->getById("GV04");
    ASSERT_TRUE(check.has_value());
    EXPECT_EQ(check.value().getBasicMonthlyPay(), 1800);
}

TEST_F(SqlSalaryRecordDaoTest, Update_NonExisting_ReturnsError) {
    SalaryRecord r("GV_UNKNOWN", 123);
    auto res = salaryDao->update(r);
    ASSERT_FALSE(res.has_value());
    EXPECT_EQ(res.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlSalaryRecordDaoTest, Remove_Success) {
    insertDirectly("GV05", 3000);
    auto res = salaryDao->remove("GV05");
    ASSERT_TRUE(res.has_value());
    EXPECT_TRUE(res.value());

    auto check = salaryDao->getById("GV05");
    ASSERT_FALSE(check.has_value());
    EXPECT_EQ(check.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlSalaryRecordDaoTest, Remove_NonExisting_ReturnsError) {
    auto res = salaryDao->remove("NOT_EXISTS");
    ASSERT_FALSE(res.has_value());
    EXPECT_EQ(res.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlSalaryRecordDaoTest, Exists_ReturnsTrueIfExists) {
    insertDirectly("GV06", 3333);
    auto res = salaryDao->exists("GV06");
    ASSERT_TRUE(res.has_value());
    EXPECT_TRUE(res.value());
}

TEST_F(SqlSalaryRecordDaoTest, Exists_ReturnsFalseIfNotExists) {
    auto res = salaryDao->exists("NO_GV");
    ASSERT_TRUE(res.has_value());
    EXPECT_FALSE(res.value());
}
