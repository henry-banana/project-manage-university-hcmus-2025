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

class SQLiteAdapter : public IDatabaseAdapter {
private:
    std::unique_ptr<SQLiteConnector> _connector;
    std::string _dbPath;
    bool _isConnected;
    int _transactionDepth; // (➕) Để theo dõi transaction lồng nhau (SQLite hỗ trợ qua SAVEPOINT)

    Error bindParameters(sqlite3_stmt* stmt, const std::vector<DbQueryParam>& params);

public:
    SQLiteAdapter();
    ~SQLiteAdapter() override;

    std::expected<bool, Error> connect(const std::string& dbPath) override;
    std::expected<bool, Error> disconnect() override;
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