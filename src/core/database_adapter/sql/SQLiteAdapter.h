/**
 * @file SQLiteAdapter.h
 * @brief Định nghĩa lớp điều hợp cơ sở dữ liệu SQLite
 * 
 * File này định nghĩa lớp SQLiteAdapter, triển khai giao diện IDatabaseAdapter
 * để tương tác với cơ sở dữ liệu SQLite. Lớp này cung cấp các phương thức để kết nối,
 * ngắt kết nối, thực hiện truy vấn và cập nhật dữ liệu trong cơ sở dữ liệu SQLite.
 */
#ifndef SQLITEADAPTER_H
#define SQLITEADAPTER_H

#include "../interface/IDatabaseAdapter.h"
#include "SQLiteConnector.h"
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <any>
#include <expected>
#include "../../../common/ErrorType.h"

/**
 * @class SQLiteAdapter
 * @brief Lớp điều hợp cho cơ sở dữ liệu SQLite
 * 
 * Lớp này triển khai giao diện IDatabaseAdapter để tương tác với cơ sở dữ liệu SQLite.
 * Nó quản lý kết nối, thực hiện các truy vấn SQL, và xử lý các giao dịch với cơ sở dữ liệu SQLite.
 */
class SQLiteAdapter : public IDatabaseAdapter {
private:
    std::unique_ptr<SQLiteConnector> _connector; ///< Đối tượng kết nối SQLite
    std::string _dbPath; ///< Đường dẫn đến file cơ sở dữ liệu
    bool _isConnected; ///< Trạng thái kết nối hiện tại
    int _transactionDepth; ///< Để theo dõi transaction lồng nhau (SQLite hỗ trợ qua SAVEPOINT)

    /**
     * @brief Gắn các tham số vào câu lệnh SQL đã chuẩn bị
     * 
     * @param stmt Con trỏ đến câu lệnh SQLite đã chuẩn bị
     * @param params Danh sách tham số cần gắn
     * @return Lỗi nếu có, hoặc không có lỗi
     */
    Error bindParameters(sqlite3_stmt* stmt, const std::vector<DbQueryParam>& params);

public:
    /**
     * @brief Constructor mặc định
     */
    SQLiteAdapter();
    
    /**
     * @brief Hàm hủy, đảm bảo đóng kết nối
     */
    ~SQLiteAdapter() override;

    /**
     * @brief Kết nối đến cơ sở dữ liệu SQLite
     * 
     * @param dbPath Đường dẫn đến file cơ sở dữ liệu SQLite
     * @return Kết quả thành công hoặc lỗi
     */
    std::expected<bool, Error> connect(const std::string& dbPath) override;
    
    /**
     * @brief Ngắt kết nối với cơ sở dữ liệu SQLite
     * @return Kết quả thành công hoặc lỗi
     */
    std::expected<bool, Error> disconnect() override;
    
    /**
     * @brief Kiểm tra trạng thái kết nối
     * @return true nếu đã kết nối, false nếu chưa
     */
    bool isConnected() const override;

    std::expected<DbQueryResultTable, Error> executeQuery(const std::string& sqlQuery, const std::vector<DbQueryParam>& params = {}) override;
    std::expected<long, Error> executeUpdate(const std::string& sqlQuery, const std::vector<DbQueryParam>& params = {}) override;

    // (➕) Transaction Management
    std::expected<bool, Error> beginTransaction() override;
    std::expected<bool, Error> commitTransaction() override;
    std::expected<bool, Error> rollbackTransaction() override;
    bool isInTransaction() const override;

    std::expected<bool, Error> ensureTablesExist();
};

#endif 