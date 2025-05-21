#ifndef IDATABASEADAPTER_H
#define IDATABASEADAPTER_H


#include <string>
#include <vector>
#include <map>
#include <any>
#include <expected> // Đã thống nhất dùng std::expected
#include "../../../common/ErrorType.h"
// AppConfig không cần ở đây nữa nếu connectionString truyền qua connect()

using DbQueryParam = std::any;
using DbQueryResultRow = std::map<std::string, std::any>;
using DbQueryResultTable = std::vector<DbQueryResultRow>;

class IDatabaseAdapter {
public:
    virtual ~IDatabaseAdapter() = default;

    virtual std::expected<bool, Error> connect(const std::string& connectionString) = 0;
    virtual std::expected<bool, Error> disconnect() = 0;
    virtual bool isConnected() const = 0;

    virtual std::expected<DbQueryResultTable, Error> executeQuery(const std::string& sqlQuery, const std::vector<DbQueryParam>& params = {}) = 0;
    virtual std::expected<long, Error> executeUpdate(const std::string& sqlQuery, const std::vector<DbQueryParam>& params = {}) = 0;

    // (➕) Transaction Management Methods
    virtual std::expected<bool, Error> beginTransaction() = 0;
    virtual std::expected<bool, Error> commitTransaction() = 0;
    virtual std::expected<bool, Error> rollbackTransaction() = 0;
    virtual bool isInTransaction() const = 0; // (➕) Kiểm tra xem có đang trong transaction không
};

#endif