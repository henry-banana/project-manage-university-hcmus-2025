#include "SQLiteAdapter.h"
#include "sqlite3.h"
#include <iostream>
#include <stdexcept>
#include "../../../utils/Logger.h"
#include "../../../common/ErrorType.h"

// Constructor và Destructor giữ nguyên như trước
SQLiteAdapter::SQLiteAdapter() : _connector(nullptr), _isConnected(false), _transactionDepth(0) { // (➕) Khởi tạo _transactionDepth
    LOG_DEBUG("SQLiteAdapter: Instance created.");
}

SQLiteAdapter::~SQLiteAdapter() {
    LOG_DEBUG("SQLiteAdapter: Instance being destroyed.");
    // unique_ptr _connector sẽ tự dọn dẹp SQLiteConnector
}

// connect, disconnect, isConnected, bindParameters, executeQuery, executeUpdate giữ nguyên như triển khai trước đó.
// (Nhớ copy phần triển khai đầy đủ của chúng vào đây)

// (Triển khai các hàm đã có từ phản hồi trước...)
// connect(dbPath) ...
// disconnect() ...
// isConnected() ...
// bindParameters(stmt, params) ...
// executeQuery(sqlQuery, params) ...
// executeUpdate(sqlQuery, params) ...
// ensureTablesExist() ...
// (Copy lại các hàm này từ phản hồi trước vào đây)
std::expected<bool, Error> SQLiteAdapter::connect(const std::string& dbPath) {
    if (_isConnected && _connector) {
        LOG_INFO("SQLiteAdapter::connect - Already connected to " + _dbPath + ". Disconnecting first.");
    }
    _dbPath = dbPath;
    _connector = std::make_unique<SQLiteConnector>(_dbPath);
    if (_connector->connect()) {
        _isConnected = true;
        _transactionDepth = 0; // Reset transaction depth khi kết nối mới
        LOG_INFO("SQLiteAdapter: Successfully connected to database: " + _dbPath);
        std::string fk_pragma = "PRAGMA foreign_keys = ON;";
        char* errMsg = nullptr;
        int rc = sqlite3_exec(_connector->getDbHandle(), fk_pragma.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::string fkErrMsg = "SQLiteAdapter: Failed to enable foreign keys: " + std::string(errMsg ? errMsg : "Unknown SQLite error");
            sqlite3_free(errMsg);
            LOG_ERROR(fkErrMsg);
            if (_connector) _connector->disconnect();
            _isConnected = false;
            _connector.reset();
            return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, fkErrMsg});
        }
        sqlite3_free(errMsg);
        LOG_INFO("SQLiteAdapter: Foreign key enforcement enabled.");
        return true;
    } else {
        std::string connErrMsg = _connector->getLastError();
        _connector.reset();
        _isConnected = false;
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, "Failed to connect via SQLiteConnector: " + connErrMsg});
    }
}

std::expected<bool, Error> SQLiteAdapter::disconnect() {
    if (!_isConnected) {
        LOG_INFO("SQLiteAdapter: Already disconnected.");
        return true;
    }
    if (_transactionDepth > 0) {
        LOG_WARN("SQLiteAdapter::disconnect - Disconnecting while a transaction is active (depth: " + std::to_string(_transactionDepth) + "). Rolling back automatically.");
        // SQLite tự động rollback khi close connection, nhưng log cảnh báo là tốt
        // Hoặc có thể chủ động gọi rollbackTransaction() nhiều lần
        while (_transactionDepth > 0) {
            auto rbResult = rollbackTransaction(); // Sẽ giảm _transactionDepth
            if (!rbResult.has_value()) {
                LOG_ERROR("SQLiteAdapter::disconnect - Error during automatic rollback: " + rbResult.error().message);
                // Tiếp tục cố gắng ngắt kết nối
                break;
            }
        }
    }
    if (_connector) {
        _connector->disconnect();
        _connector.reset();
    }
    _isConnected = false;
    _transactionDepth = 0;
    LOG_INFO("SQLiteAdapter: Disconnected from database: " + _dbPath);
    return true;
}

bool SQLiteAdapter::isConnected() const {
    return _isConnected && _connector && _connector->isConnected();
}

// bindParameters giữ nguyên


// (➕) Triển khai các phương thức Transaction
std::expected<bool, Error> SQLiteAdapter::beginTransaction() {
    if (!isConnected()) {
        LOG_ERROR("SQLiteAdapter::beginTransaction - Not connected to database.");
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, "Not connected to database."});
    }
    std::string sql;
    if (_transactionDepth == 0) {
        sql = "BEGIN TRANSACTION;"; // Hoặc "BEGIN IMMEDIATE TRANSACTION;" để lock sớm hơn
    } else {
        // SQLite dùng SAVEPOINT cho transaction lồng nhau
        sql = "SAVEPOINT LEVEL_" + std::to_string(_transactionDepth) + ";";
    }

    LOG_DEBUG("SQLiteAdapter::beginTransaction - Executing: " + sql);
    if (_connector->executeNonQuery(sql)) {
        _transactionDepth++;
        LOG_INFO("SQLiteAdapter::beginTransaction - Transaction started/savepoint created. Depth: " + std::to_string(_transactionDepth));
        return true;
    } else {
        std::string errMsg = "SQLiteAdapter::beginTransaction - Failed to begin transaction/savepoint: " + _connector->getLastError();
        LOG_ERROR(errMsg);
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
    }
}

std::expected<bool, Error> SQLiteAdapter::commitTransaction() {
    if (!isConnected()) {
        LOG_ERROR("SQLiteAdapter::commitTransaction - Not connected to database.");
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, "Not connected to database."});
    }
    if (_transactionDepth <= 0) {
        LOG_WARN("SQLiteAdapter::commitTransaction - No active transaction to commit.");
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "No active transaction to commit."});
    }

    std::string sql;
    if (_transactionDepth == 1) {
        sql = "COMMIT TRANSACTION;";
    } else {
        // Release SAVEPOINT cho transaction lồng nhau
        sql = "RELEASE SAVEPOINT LEVEL_" + std::to_string(_transactionDepth - 1) + ";";
    }

    LOG_DEBUG("SQLiteAdapter::commitTransaction - Executing: " + sql);
    if (_connector->executeNonQuery(sql)) {
        _transactionDepth--;
        LOG_INFO("SQLiteAdapter::commitTransaction - Transaction committed/savepoint released. New depth: " + std::to_string(_transactionDepth));
        return true;
    } else {
        // Nếu commit/release thất bại, CẦN rollback TOÀN BỘ transaction ngoài cùng
        // hoặc ít nhất là rollback savepoint hiện tại.
        // Điều này quan trọng để tránh trạng thái không nhất quán.
        std::string errMsg = "SQLiteAdapter::commitTransaction - Failed to commit/release: " + _connector->getLastError();
        LOG_ERROR(errMsg + ". Attempting full rollback.");

        // Cố gắng rollback toàn bộ
        std::string fullRollbackSql = "ROLLBACK TRANSACTION;";
        if (_transactionDepth > 1) { // Nếu đang ở savepoint, rollback về savepoint trước đó hoặc rollback toàn bộ
             fullRollbackSql = "ROLLBACK TO SAVEPOINT LEVEL_" + std::to_string(_transactionDepth - 1) + ";";
             // Hoặc luôn rollback toàn bộ:
             // fullRollbackSql = "ROLLBACK TRANSACTION;";
        }


        if (!_connector->executeNonQuery(fullRollbackSql)) {
            LOG_CRITICAL("SQLiteAdapter::commitTransaction - CRITICAL: Failed to rollback after commit failure: " + _connector->getLastError());
            // Trạng thái DB có thể không nhất quán!
        }
        _transactionDepth = 0; // Sau full rollback, depth là 0
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg + ". Transaction was rolled back."});
    }
}

std::expected<bool, Error> SQLiteAdapter::rollbackTransaction() {
    if (!isConnected()) {
        LOG_ERROR("SQLiteAdapter::rollbackTransaction - Not connected to database.");
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, "Not connected to database."});
    }
    if (_transactionDepth <= 0) {
        LOG_WARN("SQLiteAdapter::rollbackTransaction - No active transaction to rollback.");
        // Coi như thành công nếu không có gì để rollback
        return true;
    }

    std::string sql;
    if (_transactionDepth == 1) {
        sql = "ROLLBACK TRANSACTION;";
    } else {
        // Rollback SAVEPOINT
        sql = "ROLLBACK TRANSACTION TO SAVEPOINT LEVEL_" + std::to_string(_transactionDepth - 1) + ";";
        // Và sau đó cũng cần RELEASE savepoint đó để nó không còn tồn tại
        // Tuy nhiên, "ROLLBACK TO" đã loại bỏ savepoint đó và các savepoint sau nó.
    }

    LOG_DEBUG("SQLiteAdapter::rollbackTransaction - Executing: " + sql);
    if (_connector->executeNonQuery(sql)) {
        _transactionDepth--;
        LOG_INFO("SQLiteAdapter::rollbackTransaction - Transaction rolled back/savepoint rolled back. New depth: " + std::to_string(_transactionDepth));
        // Nếu là rollback to savepoint, chúng ta cũng nên "release" nó để SQLite dọn dẹp,
        // nhưng thực tế "ROLLBACK TO" đã làm điều đó.
        // Nếu _transactionDepth vẫn > 0 sau khi rollback savepoint, thì savepoint đó không còn nữa.
        return true;
    } else {
        std::string errMsg = "SQLiteAdapter::rollbackTransaction - Failed to rollback/rollback to savepoint: " + _connector->getLastError();
        LOG_CRITICAL("SQLiteAdapter::rollbackTransaction - CRITICAL: " + errMsg + ". Database might be in an inconsistent state.");
        // Không rõ trạng thái của _transactionDepth ở đây, có thể reset về 0 để an toàn
        // _transactionDepth = 0;
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
    }
}

bool SQLiteAdapter::isInTransaction() const {
    return _transactionDepth > 0;
}

// Phần còn lại của SQLiteAdapter.cpp (executeQuery, executeUpdate, ensureTablesExist, bindParameters) giữ nguyên như lần triển khai trước đó.
// Bạn cần copy các phần đó vào đây.
Error SQLiteAdapter::bindParameters(sqlite3_stmt* stmt, const std::vector<DbQueryParam>& params) {
    for (size_t i = 0; i < params.size(); ++i) {
        const auto& param_any = params[i];
        int bind_idx = static_cast<int>(i + 1);
        int rc = SQLITE_OK;
        if (!param_any.has_value()) {
            rc = sqlite3_bind_null(stmt, bind_idx);
        } else if (param_any.type() == typeid(int)) {
            rc = sqlite3_bind_int(stmt, bind_idx, std::any_cast<int>(param_any));
        } else if (param_any.type() == typeid(long)) {
            rc = sqlite3_bind_int64(stmt, bind_idx, std::any_cast<long>(param_any));
        } else if (param_any.type() == typeid(sqlite3_int64)) {
            rc = sqlite3_bind_int64(stmt, bind_idx, std::any_cast<sqlite3_int64>(param_any));
        } else if (param_any.type() == typeid(double)) {
            rc = sqlite3_bind_double(stmt, bind_idx, std::any_cast<double>(param_any));
        } else if (param_any.type() == typeid(const char*)) {
            const char* text_val = std::any_cast<const char*>(param_any);
            rc = sqlite3_bind_text(stmt, bind_idx, text_val, -1, SQLITE_STATIC);
        } else if (param_any.type() == typeid(std::string)) {
            const std::string& text_val = std::any_cast<const std::string&>(param_any);
            rc = sqlite3_bind_text(stmt, bind_idx, text_val.c_str(), -1, SQLITE_TRANSIENT);
        }
        else {
            std::string errMsg = "SQLiteAdapter::bindParameters - Unsupported parameter type at index " + std::to_string(i);
            LOG_ERROR(errMsg);
            return Error{ErrorCode::OPERATION_FAILED, errMsg};
        }
        if (rc != SQLITE_OK) {
            std::string errMsg = "SQLiteAdapter::bindParameters - Failed to bind parameter at index " + std::to_string(i) +
                                 " (SQLite error code: " + std::to_string(rc) + "): " +
                                 (_connector ? sqlite3_errmsg(_connector->getDbHandle()) : "Connector not available");
            LOG_ERROR(errMsg);
            return Error{ErrorCode::DATA_ACCESS_ERROR, errMsg};
        }
    }
    return Error{SQLITE_OK, ""};
}

std::expected<DbQueryResultTable, Error> SQLiteAdapter::executeQuery(const std::string& sqlQuery, const std::vector<DbQueryParam>& params) {
    if (!isConnected()) {
        LOG_ERROR("SQLiteAdapter::executeQuery - Not connected to database.");
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, "Not connected to database."});
    }
    sqlite3_stmt* stmt = nullptr;
    LOG_DEBUG("SQLiteAdapter::executeQuery - Preparing query: " + sqlQuery);
    int rc_prepare = sqlite3_prepare_v2(_connector->getDbHandle(), sqlQuery.c_str(), -1, &stmt, nullptr);
    if (rc_prepare != SQLITE_OK) {
        std::string errMsg = "SQLiteAdapter::executeQuery - Failed to prepare statement: " +
                             std::string(sqlite3_errmsg(_connector->getDbHandle())) + " | Query: " + sqlQuery;
        LOG_ERROR(errMsg);
        if (stmt) sqlite3_finalize(stmt);
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
    }
    Error bindErr = bindParameters(stmt, params);
    if (bindErr.code != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return std::unexpected(bindErr);
    }
    DbQueryResultTable results;
    int rc_step;
    LOG_DEBUG("SQLiteAdapter::executeQuery - Stepping through results for query: " + sqlQuery);
    while ((rc_step = sqlite3_step(stmt)) == SQLITE_ROW) {
        DbQueryResultRow currentRow;
        int columnCount = sqlite3_column_count(stmt);
        for (int i = 0; i < columnCount; ++i) {
            const char* columnName_cstr = sqlite3_column_name(stmt, i);
            if (!columnName_cstr) {
                 LOG_WARN("SQLiteAdapter::executeQuery - NULL column name at index " + std::to_string(i) + " for query: " + sqlQuery);
                 continue;
            }
            std::string columnName(columnName_cstr);
            int columnType = sqlite3_column_type(stmt, i);
            std::any columnValue;
            switch (columnType) {
                case SQLITE_INTEGER:
                    columnValue = sqlite3_column_int64(stmt, i);
                    break;
                case SQLITE_FLOAT:
                    columnValue = sqlite3_column_double(stmt, i);
                    break;
                case SQLITE_TEXT:
                    {
                        const unsigned char* text = sqlite3_column_text(stmt, i);
                        columnValue = text ? std::string(reinterpret_cast<const char*>(text)) : std::string();
                    }
                    break;
                case SQLITE_BLOB:
                    LOG_WARN("SQLiteAdapter::executeQuery - BLOB data type encountered at column '" + columnName + "' but not fully handled.");
                    columnValue = std::string("[BLOB_DATA_PLACEHOLDER]");
                    break;
                case SQLITE_NULL:
                    columnValue = std::any{};
                    break;
                default:
                    LOG_WARN("SQLiteAdapter::executeQuery - Unknown SQLite column type: " + std::to_string(columnType) + " for column: " + columnName);
                    columnValue = std::any{};
            }
            currentRow[columnName] = columnValue;
        }
        results.push_back(currentRow);
    }
    if (rc_step != SQLITE_DONE) {
        std::string errMsg = "SQLiteAdapter::executeQuery - Failed to step through results: " +
                             std::string(sqlite3_errmsg(_connector->getDbHandle())) + " | Query: " + sqlQuery;
        LOG_ERROR(errMsg);
        sqlite3_finalize(stmt);
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
    }
    sqlite3_finalize(stmt);
    LOG_INFO("SQLiteAdapter::executeQuery - Query executed successfully. Rows: " + std::to_string(results.size()) + " | Query: " + sqlQuery);
    return results;
}

std::expected<long, Error> SQLiteAdapter::executeUpdate(const std::string& sqlQuery, const std::vector<DbQueryParam>& params) {
    if (!isConnected()) {
        LOG_ERROR("SQLiteAdapter::executeUpdate - Not connected to database.");
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, "Not connected to database."});
    }
    sqlite3_stmt* stmt = nullptr;
    LOG_DEBUG("SQLiteAdapter::executeUpdate - Preparing query: " + sqlQuery);
    int rc_prepare = sqlite3_prepare_v2(_connector->getDbHandle(), sqlQuery.c_str(), -1, &stmt, nullptr);
    if (rc_prepare != SQLITE_OK) {
        std::string errMsg = "SQLiteAdapter::executeUpdate - Failed to prepare statement: " +
                             std::string(sqlite3_errmsg(_connector->getDbHandle())) + " | Query: " + sqlQuery;
        LOG_ERROR(errMsg);
        if (stmt) sqlite3_finalize(stmt);
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
    }
    Error bindErr = bindParameters(stmt, params);
    if (bindErr.code != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return std::unexpected(bindErr);
    }
    int rc_step = sqlite3_step(stmt);
    if (rc_step != SQLITE_DONE) {
        std::string errMsg = "SQLiteAdapter::executeUpdate - Failed to execute statement (SQLite code: " + std::to_string(rc_step) + "): " +
                             std::string(sqlite3_errmsg(_connector->getDbHandle()));
        LOG_ERROR(errMsg + " | Query: " + sqlQuery);
        sqlite3_finalize(stmt);
        return std::unexpected(Error{rc_step, errMsg});
    }
    long affectedRows = sqlite3_changes(_connector->getDbHandle());
    sqlite3_finalize(stmt);
    LOG_INFO("SQLiteAdapter::executeUpdate - Update executed successfully. Rows affected: " + std::to_string(affectedRows) + " | Query: " + sqlQuery);
    return affectedRows;
}

std::expected<bool, Error> SQLiteAdapter::ensureTablesExist() {
    if (!isConnected()) {
        LOG_ERROR("SQLiteAdapter::ensureTablesExist - Database not connected.");
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, "Database not connected. Cannot ensure tables exist."});
    }
    LOG_INFO("SQLiteAdapter::ensureTablesExist - Ensuring database schema exists in " + _dbPath);
    std::vector<std::pair<std::string, std::string>> tablesToCreate = {
        {"Faculties", R"SQL(
            CREATE TABLE IF NOT EXISTS Faculties (
                id TEXT PRIMARY KEY,
                name TEXT NOT NULL UNIQUE
            ) WITHOUT ROWID;
        )SQL"},
        {"Users", R"SQL(
            CREATE TABLE IF NOT EXISTS Users (
                id TEXT PRIMARY KEY,
                firstName TEXT NOT NULL,
                lastName TEXT NOT NULL,
                birthDay INTEGER,
                birthMonth INTEGER,
                birthYear INTEGER,
                address TEXT,
                citizenId TEXT UNIQUE,
                email TEXT UNIQUE,
                phoneNumber TEXT,
                role INTEGER NOT NULL,
                status INTEGER NOT NULL
            ) WITHOUT ROWID;
        )SQL"},
        {"Students", R"SQL(
            CREATE TABLE IF NOT EXISTS Students (
                userId TEXT PRIMARY KEY,
                facultyId TEXT,
                FOREIGN KEY (userId) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE,
                FOREIGN KEY (facultyId) REFERENCES Faculties(id) ON DELETE SET NULL ON UPDATE CASCADE
            ) WITHOUT ROWID;
        )SQL"},
        {"Teachers", R"SQL(
            CREATE TABLE IF NOT EXISTS Teachers (
                userId TEXT PRIMARY KEY,
                facultyId TEXT,
                qualification TEXT,
                specializationSubjects TEXT,
                designation TEXT,
                experienceYears INTEGER DEFAULT 0,
                FOREIGN KEY (userId) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE,
                FOREIGN KEY (facultyId) REFERENCES Faculties(id) ON DELETE SET NULL ON UPDATE CASCADE
            ) WITHOUT ROWID;
        )SQL"},
        {"Courses", R"SQL(
            CREATE TABLE IF NOT EXISTS Courses (
                id TEXT PRIMARY KEY,
                name TEXT NOT NULL,
                credits INTEGER NOT NULL CHECK(credits > 0 AND credits <= 10),
                facultyId TEXT,
                FOREIGN KEY (facultyId) REFERENCES Faculties(id) ON DELETE SET NULL ON UPDATE CASCADE
            ) WITHOUT ROWID;
        )SQL"},
        {"Logins", R"SQL(
            CREATE TABLE IF NOT EXISTS Logins (
                userId TEXT PRIMARY KEY,
                passwordHash TEXT NOT NULL,
                salt TEXT NOT NULL,
                FOREIGN KEY (userId) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE
            ) WITHOUT ROWID;
        )SQL"},
        {"Enrollments", R"SQL(
            CREATE TABLE IF NOT EXISTS Enrollments (
                studentId TEXT NOT NULL,
                courseId TEXT NOT NULL,
                enrollmentDate TEXT,
                PRIMARY KEY (studentId, courseId),
                FOREIGN KEY (studentId) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE,
                FOREIGN KEY (courseId) REFERENCES Courses(id) ON DELETE CASCADE ON UPDATE CASCADE
            );
        )SQL"},
        {"CourseResults", R"SQL(
            CREATE TABLE IF NOT EXISTS CourseResults (
                studentId TEXT NOT NULL,
                courseId TEXT NOT NULL,
                marks INTEGER,
                grade TEXT,
                PRIMARY KEY (studentId, courseId),
                FOREIGN KEY (studentId) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE,
                FOREIGN KEY (courseId) REFERENCES Courses(id) ON DELETE CASCADE ON UPDATE CASCADE,
                CHECK (marks IS NULL OR (marks >= -1 AND marks <= 100))
            );
        )SQL"},
        {"FeeRecords", R"SQL(
            CREATE TABLE IF NOT EXISTS FeeRecords (
                studentId TEXT PRIMARY KEY,
                totalFee INTEGER NOT NULL CHECK(totalFee >= 0),
                paidFee INTEGER NOT NULL DEFAULT 0 CHECK(paidFee >= 0),
                FOREIGN KEY (studentId) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE,
                CHECK(paidFee <= totalFee)
            ) WITHOUT ROWID;
        )SQL"},
        {"SalaryRecords", R"SQL(
            CREATE TABLE IF NOT EXISTS SalaryRecords (
                teacherId TEXT PRIMARY KEY,
                basicMonthlyPay INTEGER NOT NULL CHECK(basicMonthlyPay >= 0),
                FOREIGN KEY (teacherId) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE
            ) WITHOUT ROWID;
        )SQL"}
    };
    auto beginTransResult = beginTransaction();
    if (!beginTransResult.has_value()) {
        return std::unexpected(beginTransResult.error());
    }
    for (const auto& tableDef : tablesToCreate) {
        LOG_DEBUG("SQLiteAdapter::ensureTablesExist - Creating table if not exists: " + tableDef.first);
        // Dùng executeUpdate của chính adapter này để nó được bọc trong transaction (nếu logic transaction nằm trong executeUpdate)
        // Tuy nhiên, CREATE TABLE IF NOT EXISTS là idempotent, không thực sự cần transaction cho mỗi lệnh.
        // Nhưng để đảm bảo tất cả bảng được tạo hoặc không gì cả, transaction bao ngoài là tốt.
        if (!_connector->executeNonQuery(tableDef.second)) { // Gọi trực tiếp connector để tránh logic transaction lồng nhau phức tạp
            std::string errMsg = "SQLiteAdapter::ensureTablesExist - Failed to create table '" + tableDef.first + "': " + _connector->getLastError();
            LOG_ERROR(errMsg);
            auto rollbackResult = rollbackTransaction(); // Cố gắng rollback
            if(!rollbackResult.has_value()) LOG_ERROR("SQLiteAdapter::ensureTablesExist - Rollback failed: " + rollbackResult.error().message);
            return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
        }
    }
    auto commitResult = commitTransaction();
    if (!commitResult.has_value()) {
        return std::unexpected(commitResult.error());
    }
    LOG_INFO("SQLiteAdapter::ensureTablesExist - Database schema ensured successfully.");
    return true;
}