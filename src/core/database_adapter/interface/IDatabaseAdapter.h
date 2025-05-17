#ifndef IDATABASEADAPTER_H
#define IDATABASEADAPTER_H

#include <string>
#include <vector>
#include <map>
#include <any> // For parameter values and result values
#include "../../../common/OperationResult.h"
#include "../../../common/AppConfig.h" // Để lấy connection string

// Định nghĩa kiểu cho một dòng kết quả từ CSDL
using DbQueryParam = std::any; // Hoặc dùng std::variant nếu biết trước các kiểu có thể
using DbQueryResultRow = std::map<std::string, std::any>;
using DbQueryResultTable = std::vector<DbQueryResultRow>;

class IDatabaseAdapter {
public:
    virtual ~IDatabaseAdapter() = default;

    virtual OperationResult<bool> connect(const std::string& connectionString) = 0;
    virtual OperationResult<bool> disconnect() = 0;
    virtual bool isConnected() const = 0;

    // Dùng cho SELECT
    virtual OperationResult<DbQueryResultTable> executeQuery(const std::string& sqlQuery, const std::vector<DbQueryParam>& params = {}) = 0;
    // Dùng cho INSERT, UPDATE, DELETE. Trả về số dòng bị ảnh hưởng (nếu CSDL hỗ trợ và cần thiết)
    virtual OperationResult<long> executeUpdate(const std::string& sqlQuery, const std::vector<DbQueryParam>& params = {}) = 0;
    // Hoặc đơn giản là OperationResult<bool>
    // virtual OperationResult<bool> executeCommand(const std::string& sqlQuery, const std::vector<DbQueryParam>& params = {}) = 0;
};

#endif