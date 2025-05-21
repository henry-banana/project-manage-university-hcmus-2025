#ifndef SQLITE_CONNECTOR_H
#define SQLITE_CONNECTOR_H

#include <string>
#include "sqlite3.h" // Đây là file header bạn cung cấp

/**
 * @class SQLiteConnector
 * @brief Lớp cấp thấp chịu trách nhiệm mở, đóng kết nối vật lý
 *        tới file database SQLite và thực thi các lệnh SQL đơn giản.
 *        Nó không xử lý logic phức tạp của việc đọc kết quả truy vấn.
 */
class SQLiteConnector {
private:
    sqlite3* _db;         // Con trỏ tới đối tượng database của SQLite
    std::string _dbPath;  // Đường dẫn tới file database

public:
    /**
     * @brief Constructor.
     * @param dbPath Đường dẫn tới file database SQLite.
     */
    explicit SQLiteConnector(const std::string& dbPath);

    /**
     * @brief Destructor, đảm bảo ngắt kết nối.
     */
    ~SQLiteConnector();

    // Xóa copy constructor và copy assignment operator để tránh quản lý sai _db
    SQLiteConnector(const SQLiteConnector&) = delete;
    SQLiteConnector& operator=(const SQLiteConnector&) = delete;

    // Cho phép move constructor và move assignment (nếu cần, thường thì không)
    SQLiteConnector(SQLiteConnector&&) noexcept;
    SQLiteConnector& operator=(SQLiteConnector&&) noexcept;

    /**
     * @brief Mở kết nối tới file database.
     *        Sẽ tạo file nếu chưa tồn tại.
     * @return true nếu kết nối thành công, false nếu thất bại.
     */
    bool connect();

    /**
     * @brief Đóng kết nối tới database.
     */
    void disconnect();

    /**
     * @brief Kiểm tra xem kết nối có đang mở không.
     * @return true nếu đang kết nối, false nếu không.
     */
    bool isConnected() const;

    /**
     * @brief Thực thi một câu lệnh SQL không trả về tập kết quả (INSERT, UPDATE, DELETE, CREATE).
     *        Sử dụng sqlite3_exec. Cẩn thận với SQL injection nếu query không được parameterized.
     *        Adapter nên dùng prepared statements cho các thao tác có tham số.
     * @param sqlQuery Câu lệnh SQL để thực thi.
     * @return true nếu thực thi thành công, false nếu có lỗi.
     */
    bool executeNonQuery(const std::string& sqlQuery);

    /**
     * @brief Lấy thông báo lỗi cuối cùng từ SQLite.
     * @return Chuỗi mô tả lỗi, hoặc "Database not connected" nếu chưa kết nối.
     */
    std::string getLastError() const;

    /**
     * @brief Lấy con trỏ sqlite3* tới đối tượng database.
     *        Cần thiết cho các thao tác nâng cao hơn như prepared statements.
     * @return Con trỏ sqlite3*, hoặc nullptr nếu chưa kết nối.
     */
    sqlite3* getDbHandle() const;
};

#endif // SQLITE_CONNECTOR_H