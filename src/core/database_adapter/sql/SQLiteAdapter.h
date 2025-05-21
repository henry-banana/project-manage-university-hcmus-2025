#ifndef SQLITEADAPTER_H
#define SQLITEADAPTER_H

#include "../interface/IDatabaseAdapter.h" // Interface mà lớp này sẽ implement
#include "SQLiteConnector.h"               // Lớp kết nối SQLite cấp thấp
#include <memory>                          // Cho std::unique_ptr
#include <string>
#include <vector>
#include <map>
#include <any>
#include <expected>
#include "../../../common/ErrorType.h"

// (DbQueryParam, DbQueryResultRow, DbQueryResultTable đã được định nghĩa trong IDatabaseAdapter.h)

class SQLiteAdapter : public IDatabaseAdapter {
private:
    std::unique_ptr<SQLiteConnector> _connector;
    std::string _dbPath; // Lưu lại đường dẫn file DB
    bool _isConnected;

    // Helper để bind tham số vào sqlite3_stmt (cho prepared statements)
    Error bindParameters(sqlite3_stmt* stmt, const std::vector<DbQueryParam>& params);

public:
    SQLiteAdapter(); // Constructor không nhận dbPath, path sẽ được truyền qua connect()
    ~SQLiteAdapter() override;

    // IDatabaseAdapter interface implementation
    std::expected<bool, Error> connect(const std::string& dbPath) override;
    std::expected<bool, Error> disconnect() override;
    bool isConnected() const override;

    std::expected<DbQueryResultTable, Error> executeQuery(const std::string& sqlQuery, const std::vector<DbQueryParam>& params = {}) override;
    std::expected<long, Error> executeUpdate(const std::string& sqlQuery, const std::vector<DbQueryParam>& params = {}) override; // Trả về số dòng bị ảnh hưởng

    // (➕) Hàm đặc thù của SQLiteAdapter để tạo schema nếu cần
    std::expected<bool, Error> ensureTablesExist();
};

#endif 