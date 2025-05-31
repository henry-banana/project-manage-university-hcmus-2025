// --- START OF NEW FILE tests/SQLiteAdapter_test.cpp ---
#include "gtest/gtest.h"
#include "../src/core/database_adapter/sql/SQLiteAdapter.h"
#include <filesystem>

TEST(SQLiteAdapterTest, ConnectAndDisconnect) {
    SQLiteAdapter adapter;
    std::string dbPath = ":memory:";

    auto connectResult = adapter.connect(dbPath);
    ASSERT_TRUE(connectResult.has_value());
    ASSERT_TRUE(adapter.isConnected());

    auto disconnectResult = adapter.disconnect();
    ASSERT_TRUE(disconnectResult.has_value());
    ASSERT_FALSE(adapter.isConnected());
}

TEST(SQLiteAdapterTest, ExecuteQuery_CreateAndSelect) {
    SQLiteAdapter adapter;
    ASSERT_TRUE(adapter.connect(":memory:").has_value());

    std::string createTable = "CREATE TABLE test_table (id TEXT PRIMARY KEY, name TEXT);";
    auto result = adapter.executeUpdate(createTable);
    ASSERT_TRUE(result.has_value());

    std::string insertSql = "INSERT INTO test_table (id, name) VALUES (?, ?);";
    std::vector<DbQueryParam> insertParams = {
        std::string("1"),
        std::string("Alice")
    };
    ASSERT_TRUE(adapter.executeUpdate(insertSql, insertParams).has_value());

    std::string selectSql = "SELECT * FROM test_table WHERE id = ?;";
    std::vector<DbQueryParam> selectParams = {
        std::string("1")
    };
    auto queryResult = adapter.executeQuery(selectSql, selectParams);
    ASSERT_TRUE(queryResult.has_value());
    ASSERT_EQ(queryResult->size(), 1);
    EXPECT_EQ(std::any_cast<std::string>((*queryResult)[0].at("id")), "1");
    EXPECT_EQ(std::any_cast<std::string>((*queryResult)[0].at("name")), "Alice");

    adapter.disconnect();
}

TEST(SQLiteAdapterTest, TransactionCommit) {
    SQLiteAdapter adapter;
    ASSERT_TRUE(adapter.connect(":memory:").has_value());

    ASSERT_TRUE(adapter.executeUpdate("CREATE TABLE t (id TEXT);").has_value());
    ASSERT_TRUE(adapter.beginTransaction().has_value());
    ASSERT_TRUE(adapter.executeUpdate("INSERT INTO t VALUES ('1');").has_value());
    ASSERT_TRUE(adapter.commitTransaction().has_value());

    auto result = adapter.executeQuery("SELECT COUNT(*) AS count FROM t;");
    ASSERT_TRUE(result.has_value());

    // Lấy giá trị count kiểu int hoặc long long
    // Thường SQLite trả về int64_t (long long)
    auto& row0 = (*result)[0];
    EXPECT_TRUE(row0.find("count") != row0.end());

    // Ép kiểu long long, có thể dùng std::any_cast<long long>
    long long count = std::any_cast<long long>(row0.at("count"));
    EXPECT_EQ(count, 1);
}


TEST(SQLiteAdapterTest, TransactionRollback) {
    SQLiteAdapter adapter;
    ASSERT_TRUE(adapter.connect(":memory:").has_value());

    ASSERT_TRUE(adapter.executeUpdate("CREATE TABLE t (id TEXT);").has_value());
    ASSERT_TRUE(adapter.beginTransaction().has_value());
    ASSERT_TRUE(adapter.executeUpdate("INSERT INTO t VALUES ('999');").has_value());
    ASSERT_TRUE(adapter.rollbackTransaction().has_value());

    auto result = adapter.executeQuery("SELECT COUNT(*) AS count FROM t;");
    ASSERT_TRUE(result.has_value());

    // Lấy kiểu trả về phù hợp (thường SQLite trả về int64_t cho count)
    long long count = std::any_cast<long long>((*result)[0].at("count"));
    EXPECT_EQ(count, 0);
}

TEST(SQLiteAdapterTest, TransactionNested) {
    SQLiteAdapter adapter;
    ASSERT_TRUE(adapter.connect(":memory:").has_value());

    ASSERT_TRUE(adapter.executeUpdate("CREATE TABLE t (id INT);").has_value());

    ASSERT_TRUE(adapter.beginTransaction().has_value());
    ASSERT_TRUE(adapter.executeUpdate("INSERT INTO t VALUES (1);").has_value());

    ASSERT_TRUE(adapter.beginTransaction().has_value()); // CREATE SAVEPOINT
    ASSERT_TRUE(adapter.executeUpdate("INSERT INTO t VALUES (2);").has_value());
    ASSERT_TRUE(adapter.rollbackTransaction().has_value()); // ROLLBACK TO SAVEPOINT

    ASSERT_TRUE(adapter.commitTransaction().has_value()); // COMMIT outer transaction

    auto result = adapter.executeQuery("SELECT COUNT(*) AS count FROM t;");
    ASSERT_TRUE(result.has_value());

    long long count = std::any_cast<long long>((*result)[0].at("count"));
    EXPECT_EQ(count, 1);
}


TEST(SQLiteAdapterTest, ExecuteUpdateInvalidSQL) {
    SQLiteAdapter adapter;
    ASSERT_TRUE(adapter.connect(":memory:").has_value());

    auto result = adapter.executeUpdate("INVALID SQL");
    ASSERT_FALSE(result.has_value());

    // Kiểm tra mã lỗi trả về có phải là lỗi câu truy vấn CSDL không
    EXPECT_EQ(result.error().code, ErrorCode::DATA_ACCESS_ERROR);

    // Có thể kiểm tra thêm thông điệp lỗi không rỗng
    EXPECT_FALSE(result.error().message.empty());

    adapter.disconnect();
}

// --- END OF NEW FILE tests/SQLiteAdapter_test.cpp ---
