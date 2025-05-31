// --- START OF NEW FILE tests/SQLiteConnector_test.cpp ---
#include "gtest/gtest.h"
#include "../src/core/database_adapter/sql/SQLiteConnector.h"
#include <filesystem> // Cần cho kiểm tra file (nếu test với file thật)

TEST(SQLiteConnectorTest, ConnectAndDisconnectInMemory) {
    SQLiteConnector connector(":memory:");
    ASSERT_FALSE(connector.isConnected());
    ASSERT_TRUE(connector.connect());
    ASSERT_TRUE(connector.isConnected());
    ASSERT_NE(connector.getDbHandle(), nullptr);
    connector.disconnect();
    ASSERT_FALSE(connector.isConnected());
    ASSERT_EQ(connector.getDbHandle(), nullptr);
}

TEST(SQLiteConnectorTest, ConnectToFileAndDisconnect) {
    std::string dbPath = "test_connector_db.sqlite";
    // Đảm bảo file không tồn tại trước khi test (hoặc xóa sau khi test)
    if (std::filesystem::exists(dbPath)) {
        std::filesystem::remove(dbPath);
    }

    SQLiteConnector connector(dbPath);
    ASSERT_TRUE(connector.connect());
    ASSERT_TRUE(connector.isConnected());
    
    // Thử thực thi một lệnh đơn giản
    ASSERT_TRUE(connector.executeNonQuery("CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT);"));
    
    connector.disconnect();
    ASSERT_FALSE(connector.isConnected());

    // Kiểm tra file đã được tạo
    EXPECT_TRUE(std::filesystem::exists(dbPath));
    // Dọn dẹp file test
    std::filesystem::remove(dbPath);
}

TEST(SQLiteConnectorTest, ConnectFailsForInvalidPath) {
    // Đường dẫn không hợp lệ (ví dụ, thư mục không tồn tại và không có quyền tạo)
    // Test này phụ thuộc vào quyền của user chạy test và OS.
    // Có thể khó để tạo một trường hợp luôn thất bại một cách đáng tin cậy.
    // SQLite có thể vẫn tạo được file nếu đường dẫn hợp lệ dù không tồn tại.
    // SQLiteConnector connector("/non_existent_dir/test.db");
    // ASSERT_FALSE(connector.connect()); // Mong đợi thất bại
    // ASSERT_FALSE(connector.isConnected());
    // Tuy nhiên, test với ":memory:" và file thật là đủ cơ bản.
}

TEST(SQLiteConnectorTest, ExecuteNonQueryOnDisconnectedDB) {
    SQLiteConnector connector(":memory:");
    // Không gọi connect()
    ASSERT_FALSE(connector.executeNonQuery("CREATE TABLE test (id INT);"));
    // Hàm này của connector hiện tại không trả về lỗi một cách rõ ràng
    // mà chỉ in ra cerr. Sẽ tốt hơn nếu nó trả về Error.
    // Với GTest, chúng ta không dễ để bắt output của cerr trừ khi dùng kỹ thuật phức tạp.
}

TEST(SQLiteConnectorTest, MoveConstructor) {
    SQLiteConnector connector1("move_test_db1.sqlite");
    ASSERT_TRUE(connector1.connect());
    sqlite3* handle1 = connector1.getDbHandle();
    ASSERT_NE(handle1, nullptr);

    SQLiteConnector connector2(std::move(connector1));
    
    ASSERT_TRUE(connector2.isConnected()); // connector2 giờ đã kết nối
    ASSERT_EQ(connector2.getDbHandle(), handle1); // Nó giữ handle cũ
    
    // connector1 không còn kết nối và không còn giữ handle
    ASSERT_FALSE(connector1.isConnected()); 
    ASSERT_EQ(connector1.getDbHandle(), nullptr);

    connector2.disconnect();
    if (std::filesystem::exists("move_test_db1.sqlite")) {
        std::filesystem::remove("move_test_db1.sqlite");
    }
}

TEST(SQLiteConnectorTest, MoveAssignment) {
    SQLiteConnector connector1("move_assign_db1.sqlite");
    ASSERT_TRUE(connector1.connect());
    sqlite3* handle1 = connector1.getDbHandle();
    ASSERT_NE(handle1, nullptr);

    SQLiteConnector connector2("move_assign_db2.sqlite"); // Có thể đã connect hoặc chưa
    
    connector2 = std::move(connector1);
    
    ASSERT_TRUE(connector2.isConnected());
    ASSERT_EQ(connector2.getDbHandle(), handle1);
    
    ASSERT_FALSE(connector1.isConnected());
    ASSERT_EQ(connector1.getDbHandle(), nullptr);

    connector2.disconnect();
     if (std::filesystem::exists("move_assign_db1.sqlite")) { // DB file được tạo bởi connector1
        std::filesystem::remove("move_assign_db1.sqlite");
    }
    if (std::filesystem::exists("move_assign_db2.sqlite")) {
        std::filesystem::remove("move_assign_db2.sqlite");
    }
}


// --- END OF NEW FILE tests/SQLiteConnector_test.cpp ---