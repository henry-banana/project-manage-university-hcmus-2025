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
    std::string dbPath;

    void setupDatabase() {
        dbPath = ":memory:";

        dbAdapter = std::make_shared<SQLiteAdapter>();
        auto connectResult = dbAdapter->connect(dbPath);
        ASSERT_TRUE(connectResult.has_value()) << connectResult.error().message;
        ASSERT_TRUE(connectResult.value());

        auto tableResult = std::static_pointer_cast<SQLiteAdapter>(dbAdapter)->ensureTablesExist();
        ASSERT_TRUE(tableResult.has_value()) << tableResult.error().message;
        ASSERT_TRUE(tableResult.value());

        salaryParser = std::make_shared<SalaryRecordSqlParser>();
        salaryDao = std::make_unique<SqlSalaryRecordDao>(dbAdapter, salaryParser);
    }

    void SetUp() override {
        setupDatabase();
        clearSalaryTable();
    }

    void TearDown() override {
        if (dbAdapter && dbAdapter->isConnected()) {
            dbAdapter->disconnect();
        }
    }

    void clearSalaryTable() {
        if (dbAdapter->isConnected()) {
            auto result = dbAdapter->executeUpdate("DELETE FROM SalaryRecords;");
            ASSERT_TRUE(result.has_value()) << result.error().message;
        }
    }

    SalaryRecord addSalaryDirectly(const std::string& id, double pay) {
        SalaryRecord r(id, pay);
        auto paramsExp = salaryParser->toQueryInsertParams(r);
        EXPECT_TRUE(paramsExp.has_value());
        auto execResult = dbAdapter->executeUpdate(
            "INSERT INTO SalaryRecords (teacherId, basicMonthlyPay) VALUES (?, ?);",
            paramsExp.value()
        );
        EXPECT_TRUE(execResult.has_value());
        return r;
    }
};

// === TEST CASES ===

TEST_F(SqlSalaryRecordDaoTest, AddSalary_Success) {
    SalaryRecord r("GV01", 1234.5);
    auto result = salaryDao->add(r);
    ASSERT_TRUE(result.has_value()) << result.error().message;
    EXPECT_EQ(result.value().getTeacherId(), "GV01");
    EXPECT_DOUBLE_EQ(result.value().getBasicMonthlyPay(), 1234.5);
}

TEST_F(SqlSalaryRecordDaoTest, AddSalary_Duplicate_ReturnsError) {
    addSalaryDirectly("GV02", 1000.0);
    SalaryRecord r("GV02", 2000.0); // same ID
    auto result = salaryDao->add(r);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::DB_FOREIGN_KEY_ERROR);
}

TEST_F(SqlSalaryRecordDaoTest, GetById_Exists) {
    addSalaryDirectly("GV03", 3000.0);
    auto result = salaryDao->getById("GV03");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().getTeacherId(), "GV03");
    EXPECT_DOUBLE_EQ(result.value().getBasicMonthlyPay(), 3000.0);
}

TEST_F(SqlSalaryRecordDaoTest, GetById_NotExists) {
    auto result = salaryDao->getById("NOPE");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlSalaryRecordDaoTest, GetAll_Empty) {
    auto result = salaryDao->getAll();
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value().empty());
}

TEST_F(SqlSalaryRecordDaoTest, GetAll_ReturnsAll) {
    addSalaryDirectly("GV01", 1000);
    addSalaryDirectly("GV02", 2000);
    auto result = salaryDao->getAll();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().size(), 2);
}

TEST_F(SqlSalaryRecordDaoTest, UpdateSalary_Success) {
    addSalaryDirectly("GV04", 1500.0);
    SalaryRecord updated("GV04", 1800.0);
    auto result = salaryDao->update(updated);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto check = salaryDao->getById("GV04");
    ASSERT_TRUE(check.has_value());
    EXPECT_DOUBLE_EQ(check.value().getBasicMonthlyPay(), 1800.0);
}

TEST_F(SqlSalaryRecordDaoTest, Update_NotExists_ReturnsError) {
    SalaryRecord r("GV_UNKNOWN", 123.0);
    auto result = salaryDao->update(r);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlSalaryRecordDaoTest, RemoveSalary_Success) {
    addSalaryDirectly("GV05", 3000.0);
    auto result = salaryDao->remove("GV05");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto check = salaryDao->getById("GV05");
    ASSERT_FALSE(check.has_value());
    EXPECT_EQ(check.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlSalaryRecordDaoTest, RemoveSalary_NotExists_ReturnsError) {
    auto result = salaryDao->remove("NOT_EXIST");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlSalaryRecordDaoTest, Exists_ReturnsTrue) {
    addSalaryDirectly("GV06", 3333.0);
    auto result = salaryDao->exists("GV06");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(SqlSalaryRecordDaoTest, Exists_ReturnsFalse) {
    auto result = salaryDao->exists("NO_GV");
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value());
}
