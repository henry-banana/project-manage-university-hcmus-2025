#include "SQLiteAdapter.h"
#include "sqlite3.h"
#include <iostream>
#include <stdexcept>
#include "../../../utils/Logger.h"
#include "../../../common/ErrorType.h"
#include "../../../utils/PasswordInput.h" // (QUAN TRỌNG) Cần cho việc hash password admin
#include "../../../common/UserRole.h"     // (QUAN TRỌNG)
#include "../../../common/LoginStatus.h"  // (QUAN TRỌNG)


// Constructor và Destructor (giữ nguyên như trước)
SQLiteAdapter::SQLiteAdapter() : _connector(nullptr), _isConnected(false), _transactionDepth(0) {
    LOG_DEBUG("SQLiteAdapter: Instance created.");
}

SQLiteAdapter::~SQLiteAdapter() {
    LOG_DEBUG("SQLiteAdapter: Instance being destroyed.");
    // Không cần gọi disconnect() tường minh ở đây vì unique_ptr _connector 
    // sẽ tự gọi destructor của SQLiteConnector, mà SQLiteConnector::~SQLiteConnector() đã gọi disconnect().
    // Tuy nhiên, nếu muốn chắc chắn hơn về việc giải phóng transaction trước khi connector bị hủy,
    // bạn có thể gọi disconnect() ở đây.
    if (_isConnected) {
        disconnect(); 
    }
}

// connect, disconnect, isConnected, bindParameters, executeQuery, executeUpdate, beginTransaction, commitTransaction, rollbackTransaction, isInTransaction
// (Giữ nguyên các hàm này như đã triển khai ở các phản hồi trước)

// ... (Copy các hàm connect, disconnect, isConnected, bindParameters, executeQuery, executeUpdate, ...
//      beginTransaction, commitTransaction, rollbackTransaction, isInTransaction từ các phản hồi trước vào đây) ...
// Ví dụ, tôi sẽ copy lại một phần để đảm bảo ngữ cảnh:

std::expected<bool, Error> SQLiteAdapter::connect(const std::string& dbPath) {
    if (_isConnected && _connector) {
        LOG_INFO("SQLiteAdapter::connect - Already connected to " + _dbPath + ". Disconnecting first.");
        // Cân nhắc gọi disconnect() ở đây nếu muốn đảm bảo trạng thái sạch sẽ
        // auto disconnectRes = disconnect();
        // if (!disconnectRes.has_value()) {
        //     LOG_ERROR("SQLiteAdapter::connect - Failed to disconnect previous connection: " + disconnectRes.error().message);
        //     // Có thể quyết định không cho kết nối mới nếu ngắt kết nối cũ lỗi
        // }
    }
    _dbPath = dbPath; // Lưu đường dẫn mới
    _connector = std::make_unique<SQLiteConnector>(_dbPath);
    if (_connector->connect()) {
        _isConnected = true;
        _transactionDepth = 0; 
        LOG_INFO("SQLiteAdapter: Successfully connected to database: " + _dbPath);
        
        // Bật foreign keys
        char* errMsgFK = nullptr;
        int rcFK = sqlite3_exec(_connector->getDbHandle(), "PRAGMA foreign_keys = ON;", nullptr, nullptr, &errMsgFK);
        if (rcFK != SQLITE_OK) {
            std::string fkErrMsg = "SQLiteAdapter: Failed to enable foreign keys: " + std::string(errMsgFK ? errMsgFK : "Unknown SQLite error");
            sqlite3_free(errMsgFK);
            LOG_ERROR(fkErrMsg);
            // Nếu bật FK lỗi, coi như kết nối thất bại hoàn toàn
            if (_connector) _connector->disconnect();
            _isConnected = false;
            _connector.reset();
            return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, fkErrMsg});
        }
        sqlite3_free(errMsgFK);
        LOG_INFO("SQLiteAdapter: Foreign key enforcement enabled.");
        return true;
    } else {
        std::string connErrMsg = _connector->getLastError();
        _connector.reset(); // Quan trọng: giải phóng connector nếu connect thất bại
        _isConnected = false;
        LOG_ERROR("SQLiteAdapter: Failed to connect to database: " + connErrMsg);
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, "Failed to connect via SQLiteConnector: " + connErrMsg});
    }
}

std::expected<bool, Error> SQLiteAdapter::disconnect() {
    if (!_isConnected) {
        LOG_INFO("SQLiteAdapter: Already disconnected.");
        return true;
    }
    if (isInTransaction()) { // Dùng hàm đã có
        LOG_WARN("SQLiteAdapter::disconnect - Disconnecting while a transaction is active (depth: " + std::to_string(_transactionDepth) + "). Rolling back automatically.");
        auto rbResult = rollbackTransaction(); // Cố gắng rollback transaction ngoài cùng
        if (!rbResult.has_value()) {
            LOG_ERROR("SQLiteAdapter::disconnect - Error during automatic rollback: " + rbResult.error().message);
        }
        // Dù rollback thành công hay không, vẫn tiếp tục ngắt kết nối
    }
    if (_connector) {
        _connector->disconnect();
        _connector.reset(); // Giải phóng unique_ptr
    }
    _isConnected = false;
    _transactionDepth = 0; // Reset transaction depth
    LOG_INFO("SQLiteAdapter: Disconnected from database: " + _dbPath);
    _dbPath.clear();
    return true;
}

bool SQLiteAdapter::isConnected() const {
    return _isConnected && _connector && _connector->isConnected();
}

Error SQLiteAdapter::bindParameters(sqlite3_stmt* stmt, const std::vector<DbQueryParam>& params) {
    for (size_t i = 0; i < params.size(); ++i) {
        const auto& param_any = params[i];
        int bind_idx = static_cast<int>(i + 1);
        int rc = SQLITE_OK;
        if (!param_any.has_value()) {
            rc = sqlite3_bind_null(stmt, bind_idx);
        } else if (param_any.type() == typeid(int)) {
            rc = sqlite3_bind_int(stmt, bind_idx, std::any_cast<int>(param_any));
        } else if (param_any.type() == typeid(long)) { // Thêm kiểu long
            rc = sqlite3_bind_int64(stmt, bind_idx, std::any_cast<long>(param_any));
        } else if (param_any.type() == typeid(long long)) { // Thêm kiểu long long (thường là sqlite3_int64)
             rc = sqlite3_bind_int64(stmt, bind_idx, std::any_cast<long long>(param_any));
        } else if (param_any.type() == typeid(sqlite3_int64)) {
            rc = sqlite3_bind_int64(stmt, bind_idx, std::any_cast<sqlite3_int64>(param_any));
        } else if (param_any.type() == typeid(double)) {
            rc = sqlite3_bind_double(stmt, bind_idx, std::any_cast<double>(param_any));
        } else if (param_any.type() == typeid(const char*)) {
            const char* text_val = std::any_cast<const char*>(param_any);
            rc = sqlite3_bind_text(stmt, bind_idx, text_val, -1, SQLITE_STATIC);
        } else if (param_any.type() == typeid(std::string)) {
            const std::string& text_val = std::any_cast<const std::string&>(param_any);
            // Dùng SQLITE_TRANSIENT vì text_val có thể bị hủy sau khi hàm này kết thúc
            rc = sqlite3_bind_text(stmt, bind_idx, text_val.c_str(), -1, SQLITE_TRANSIENT);
        } else if (param_any.type() == typeid(bool)) { // (➕) Thêm case cho bool
            rc = sqlite3_bind_int(stmt, bind_idx, std::any_cast<bool>(param_any) ? 1 : 0);
        }
        else {
            std::string errMsg = "SQLiteAdapter::bindParameters - Unsupported parameter type at index " + std::to_string(i) + " (type: " + param_any.type().name() + ")";
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
    return Error{SQLITE_OK, ""}; // SQLITE_OK là 0, đại diện cho thành công
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
        if (stmt) sqlite3_finalize(stmt); // Dọn dẹp stmt nếu prepare lỗi nhưng stmt được cấp phát
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
    }
    Error bindErr = bindParameters(stmt, params);
    if (bindErr.code != SQLITE_OK) { // Kiểm tra code lỗi từ bindParameters
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
                 // Có thể gán một tên placeholder hoặc bỏ qua cột này
                 continue; 
            }
            std::string columnName(columnName_cstr);
            int columnType = sqlite3_column_type(stmt, i);
            std::any columnValue;
            switch (columnType) {
                case SQLITE_INTEGER:
                    columnValue = sqlite3_column_int64(stmt, i); // Luôn đọc là int64 cho nhất quán
                    break;
                case SQLITE_FLOAT:
                    columnValue = sqlite3_column_double(stmt, i);
                    break;
                case SQLITE_TEXT:
                    {
                        const unsigned char* text = sqlite3_column_text(stmt, i);
                        // sqlite3_column_text có thể trả về NULL nếu giá trị là SQL NULL dù type là TEXT (hiếm)
                        columnValue = text ? std::string(reinterpret_cast<const char*>(text)) : std::string();
                    }
                    break;
                case SQLITE_BLOB:
                    // Xử lý BLOB phức tạp hơn, tạm thời có thể đọc thành vector<char> hoặc string
                    LOG_WARN("SQLiteAdapter::executeQuery - BLOB data type encountered at column '" + columnName + "' but not fully handled (returning as string placeholder).");
                    // const void* blob_data = sqlite3_column_blob(stmt, i);
                    // int blob_size = sqlite3_column_bytes(stmt, i);
                    // columnValue = std::vector<unsigned char>(static_cast<const unsigned char*>(blob_data), static_cast<const unsigned char*>(blob_data) + blob_size);
                    columnValue = std::string("[BLOB_DATA_PLACEHOLDER]"); // Tạm thời
                    break;
                case SQLITE_NULL:
                    columnValue = std::any{}; // std::any rỗng đại diện cho NULL
                    break;
                default:
                    LOG_WARN("SQLiteAdapter::executeQuery - Unknown SQLite column type: " + std::to_string(columnType) + " for column: " + columnName);
                    columnValue = std::any{}; // Hoặc một giá trị báo lỗi
            }
            currentRow[columnName] = columnValue;
        }
        results.push_back(currentRow);
    }
    if (rc_step != SQLITE_DONE) { // Nếu vòng lặp kết thúc không phải vì DONE (ví dụ lỗi)
        std::string errMsg = "SQLiteAdapter::executeQuery - Failed to step through results (SQLite error code: " + std::to_string(rc_step) + "): " +
                             std::string(sqlite3_errmsg(_connector->getDbHandle())) + " | Query: " + sqlQuery;
        LOG_ERROR(errMsg);
        sqlite3_finalize(stmt);
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
    }
    sqlite3_finalize(stmt); // Luôn finalize stmt
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
        // Kiểm tra lỗi ràng buộc khóa ngoại cụ thể
        int extended_err_code = sqlite3_extended_errcode(_connector->getDbHandle());
        if (extended_err_code == SQLITE_CONSTRAINT_FOREIGNKEY) {
            std::string errMsg = "SQLiteAdapter::executeUpdate - Foreign key constraint failed: " +
                                 std::string(sqlite3_errmsg(_connector->getDbHandle()));
            LOG_ERROR(errMsg + " | Query: " + sqlQuery);
            sqlite3_finalize(stmt);
            return std::unexpected(Error{ErrorCode::DB_FOREIGN_KEY_ERROR, errMsg}); // (➕) Mã lỗi cụ thể hơn
        }
        // Các lỗi ràng buộc khác (bao gồm cả primary key, unique)
        if (rc_step == SQLITE_CONSTRAINT) { // Mã lỗi chung cho constraint
            std::string errMsg = "SQLiteAdapter::executeUpdate - Constraint violation: " +
                                 std::string(sqlite3_errmsg(_connector->getDbHandle()));
            LOG_ERROR(errMsg + " | Query: " + sqlQuery);
            sqlite3_finalize(stmt);
            // Phân loại lỗi constraint dựa trên extended_err_code
            if(extended_err_code == SQLITE_CONSTRAINT_PRIMARYKEY || extended_err_code == SQLITE_CONSTRAINT_UNIQUE){
                 return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, errMsg});
            }
            return std::unexpected(Error{ErrorCode::DB_CONSTRAINT_ERROR, errMsg});
        }

        std::string errMsg = "SQLiteAdapter::executeUpdate - Failed to execute statement (SQLite code: " + std::to_string(rc_step) + "): " +
                             std::string(sqlite3_errmsg(_connector->getDbHandle()));
        LOG_ERROR(errMsg + " | Query: " + sqlQuery);
        sqlite3_finalize(stmt);
        // Trả về mã lỗi SQLite gốc nếu không phải là lỗi ràng buộc đã xử lý ở trên
        return std::unexpected(Error{ErrorCode::DB_QUERY_ERROR, errMsg}); 
    }
    long affectedRows = sqlite3_changes(_connector->getDbHandle());
    sqlite3_finalize(stmt);
    LOG_INFO("SQLiteAdapter::executeUpdate - Update executed successfully. Rows affected: " + std::to_string(affectedRows) + " | Query: " + sqlQuery);
    return affectedRows;
}

std::expected<bool, Error> SQLiteAdapter::beginTransaction() {
    if (!isConnected()) {
        LOG_ERROR("SQLiteAdapter::beginTransaction - Not connected to database.");
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, "Not connected to database."});
    }
    std::string sql;
    if (_transactionDepth == 0) {
        sql = "BEGIN TRANSACTION;"; 
    } else {
        sql = "SAVEPOINT LEVEL_" + std::to_string(_transactionDepth) + ";";
    }

    LOG_DEBUG("SQLiteAdapter::beginTransaction - Executing: " + sql);
    // Dùng executeUpdate để chạy lệnh không trả về kết quả.
    // Lưu ý: executeNonQuery của SQLiteConnector đơn giản hơn, không cần prepare/step/finalize
    // Nhưng để nhất quán và có thể handle lỗi tốt hơn, dùng executeUpdate của chính adapter.
    auto execRes = executeUpdate(sql); // Không có params
    if (!execRes.has_value()) {
        std::string errMsg = "SQLiteAdapter::beginTransaction - Failed to begin transaction/savepoint: " + execRes.error().message;
        LOG_ERROR(errMsg);
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
    }
    _transactionDepth++;
    LOG_INFO("SQLiteAdapter::beginTransaction - Transaction started/savepoint created. Depth: " + std::to_string(_transactionDepth));
    return true;
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
        sql = "RELEASE SAVEPOINT LEVEL_" + std::to_string(_transactionDepth - 1) + ";";
    }

    LOG_DEBUG("SQLiteAdapter::commitTransaction - Executing: " + sql);
    auto execRes = executeUpdate(sql);
    if (!execRes.has_value()) {
        std::string errMsg = "SQLiteAdapter::commitTransaction - Failed to commit/release: " + execRes.error().message;
        LOG_ERROR(errMsg + ". Attempting full rollback of current transaction level.");
        
        // Không cần gọi rollbackTransaction() ở đây nữa vì nếu COMMIT/RELEASE lỗi,
        // transaction/savepoint đó vẫn còn active (hoặc DB tự rollback).
        // Việc gọi rollbackTransaction() có thể làm giảm _transactionDepth sai.
        // SQLite sẽ tự rollback transaction ngoài cùng nếu COMMIT lỗi.
        // Với SAVEPOINT, nếu RELEASE lỗi, savepoint đó vẫn còn.
        // Tốt nhất là để DB tự xử lý hoặc báo lỗi nghiêm trọng.
        // Nếu muốn chủ động rollback, cần cẩn thận.
        // _transactionDepth = 0; // Reset về 0 nếu commit chính lỗi, hoặc giảm 1 nếu release savepoint lỗi? -> Phức tạp
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg + ". Transaction state might be inconsistent."});
    }
    _transactionDepth--;
    LOG_INFO("SQLiteAdapter::commitTransaction - Transaction committed/savepoint released. New depth: " + std::to_string(_transactionDepth));
    return true;
}

std::expected<bool, Error> SQLiteAdapter::rollbackTransaction() {
    if (!isConnected()) {
        LOG_ERROR("SQLiteAdapter::rollbackTransaction - Not connected to database.");
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, "Not connected to database."});
    }
    if (_transactionDepth <= 0) {
        LOG_WARN("SQLiteAdapter::rollbackTransaction - No active transaction to rollback.");
        return true; // Coi như thành công nếu không có gì để rollback
    }

    std::string sql;
    if (_transactionDepth == 1) {
        sql = "ROLLBACK TRANSACTION;";
    } else {
        // Rollback TO savepoint sẽ hủy bỏ savepoint đó và các savepoint sau nó.
        sql = "ROLLBACK TRANSACTION TO SAVEPOINT LEVEL_" + std::to_string(_transactionDepth - 1) + ";";
        // Sau đó, cần RELEASE savepoint đó để nó không còn "treo"
        // Tuy nhiên, "ROLLBACK TO SAVEPOINT name;" cũng tương đương với "RELEASE name;" sau đó.
        // Thực tế, chỉ cần "ROLLBACK TO SAVEPOINT" là đủ.
    }

    LOG_DEBUG("SQLiteAdapter::rollbackTransaction - Executing: " + sql);
    auto execRes = executeUpdate(sql);
    if (!execRes.has_value()) {
        std::string errMsg = "SQLiteAdapter::rollbackTransaction - Failed to rollback/rollback to savepoint: " + execRes.error().message;
        LOG_CRITICAL("SQLiteAdapter::rollbackTransaction - CRITICAL: " + errMsg + ". Database might be in an inconsistent state.");
        // Trong trường hợp lỗi nghiêm trọng khi rollback, trạng thái _transactionDepth có thể không chắc chắn.
        // Có thể reset về 0 để an toàn, nhưng điều này có thể không phản ánh đúng trạng thái DB.
        // _transactionDepth = 0;
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
    }
    _transactionDepth--;
    LOG_INFO("SQLiteAdapter::rollbackTransaction - Transaction rolled back/savepoint rolled back. New depth: " + std::to_string(_transactionDepth));
    return true;
}

bool SQLiteAdapter::isInTransaction() const {
    return _transactionDepth > 0;
}


// ĐÂY LÀ PHIÊN BẢN ĐẦY ĐỦ CỦA ensureTablesExist
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
                citizenId TEXT UNIQUE, -- Nên là NOT NULL cho Student/Teacher
                email TEXT UNIQUE,     -- Nên là NOT NULL cho Student/Teacher
                phoneNumber TEXT,
                role INTEGER NOT NULL,
                status INTEGER NOT NULL 
            ) WITHOUT ROWID; 
        )SQL"},
        {"Students", R"SQL(
            CREATE TABLE IF NOT EXISTS Students (
                userId TEXT PRIMARY KEY,
                facultyId TEXT, -- Có thể NULL ban đầu hoặc nếu khoa bị xóa (SET NULL)
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
                enrollmentDate TEXT DEFAULT CURRENT_TIMESTAMP,
                PRIMARY KEY (studentId, courseId),
                FOREIGN KEY (studentId) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE,
                FOREIGN KEY (courseId) REFERENCES Courses(id) ON DELETE CASCADE ON UPDATE CASCADE
            ) /* Không dùng WITHOUT ROWID cho bảng có PK không phải INTEGER */ ;
        )SQL"},
        {"CourseResults", R"SQL(
            CREATE TABLE IF NOT EXISTS CourseResults (
                studentId TEXT NOT NULL,
                courseId TEXT NOT NULL,
                marks INTEGER, -- -1 nghĩa là chưa có điểm
                grade TEXT,    -- Sẽ được tính bởi ứng dụng khi hiển thị hoặc Service khi cần
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
        LOG_CRITICAL("SQLiteAdapter::ensureTablesExist - Failed to begin transaction for schema creation: " + beginTransResult.error().message);
        return std::unexpected(beginTransResult.error());
    }

    for (const auto& tableDef : tablesToCreate) {
        LOG_DEBUG("SQLiteAdapter::ensureTablesExist - Creating table if not exists: " + tableDef.first);
        auto createTableRes = executeUpdate(tableDef.second); 
        if (!createTableRes.has_value()) {
            std::string errMsg = "SQLiteAdapter::ensureTablesExist - Failed to create table '" + tableDef.first + "': " + createTableRes.error().message;
            LOG_ERROR(errMsg);
            auto rbRes = rollbackTransaction(); 
            if(!rbRes.has_value()) LOG_CRITICAL("SQLiteAdapter::ensureTablesExist - Rollback failed after table creation error: " + rbRes.error().message);
            return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
        }
    }

    // (➕) Tạo Admin User mặc định
    std::string adminId = "admin";
    std::string adminDefaultPassword = "admin123"; 
    std::string adminEmail = "admin@university.local"; 

    std::string checkAdminSql = "SELECT 1 FROM Users WHERE id = ? AND role = ?;";
    std::vector<DbQueryParam> checkAdminParams = {adminId, static_cast<int>(UserRole::ADMIN)};
    auto adminExistsResult = executeQuery(checkAdminSql, checkAdminParams);

    if (!adminExistsResult.has_value()) {
        LOG_ERROR("SQLiteAdapter::ensureTablesExist - Error checking for existing admin: " + adminExistsResult.error().message);
        rollbackTransaction(); // Quan trọng: rollback transaction đang mở
        return std::unexpected(adminExistsResult.error());
    }

    if (adminExistsResult.value().empty()) { 
        LOG_INFO("SQLiteAdapter::ensureTablesExist - Admin user not found. Creating default admin...");
        
        std::string insertUserSql = "INSERT INTO Users (id, firstName, lastName, email, role, status) VALUES (?, ?, ?, ?, ?, ?);";
        std::vector<DbQueryParam> userParams = {
            adminId, "DefaultAdmin", "User", adminEmail,
            static_cast<int>(UserRole::ADMIN), static_cast<int>(LoginStatus::ACTIVE)
        };
        auto insertUserRes = executeUpdate(insertUserSql, userParams);
        if (!insertUserRes.has_value() || (insertUserRes.has_value() && insertUserRes.value() == 0 && insertUserRes.error().code == 0 /*no error code from successful 0 affected rows*/ )) {
            // Kiểm tra cả trường hợp executeUpdate trả về 0 rows affected mà không có lỗi tường minh
            std::string errMsg = "Failed to insert default admin into Users table.";
            if (!insertUserRes.has_value()) errMsg += " Error: " + insertUserRes.error().message;
            else if (insertUserRes.value() == 0) errMsg += " No rows affected.";
            
            LOG_ERROR("SQLiteAdapter::ensureTablesExist - " + errMsg);
            rollbackTransaction();
            return std::unexpected(Error{ErrorCode::OPERATION_FAILED, errMsg});
        }


        std::string salt = PasswordUtils::generateSalt();
        std::string hashedPassword = PasswordUtils::hashPassword(adminDefaultPassword, salt);
        std::string insertLoginSql = "INSERT INTO Logins (userId, passwordHash, salt) VALUES (?, ?, ?);";
        std::vector<DbQueryParam> loginParams = {adminId, hashedPassword, salt};
        auto insertLoginRes = executeUpdate(insertLoginSql, loginParams);
         if (!insertLoginRes.has_value() || (insertLoginRes.has_value() && insertLoginRes.value() == 0 && insertLoginRes.error().code == 0)) {
            std::string errMsg = "Failed to insert default admin credentials into Logins table.";
             if (!insertLoginRes.has_value()) errMsg += " Error: " + insertLoginRes.error().message;
            else if (insertLoginRes.value() == 0) errMsg += " No rows affected.";

            LOG_ERROR("SQLiteAdapter::ensureTablesExist - " + errMsg);
            rollbackTransaction();
            executeUpdate("DELETE FROM Users WHERE id = ?;", {adminId}); // Cố gắng dọn dẹp
            return std::unexpected(Error{ErrorCode::OPERATION_FAILED, errMsg});
        }
        LOG_INFO("SQLiteAdapter::ensureTablesExist - Default admin user '" + adminId + "' created with default password. PLEASE CHANGE IT AFTER FIRST LOGIN.");
    } else {
        LOG_INFO("SQLiteAdapter::ensureTablesExist - Admin user already exists.");
    }
    
    auto commitResult = commitTransaction();
    if (!commitResult.has_value()) {
        LOG_CRITICAL("SQLiteAdapter::ensureTablesExist - Failed to commit schema and admin creation transaction: " + commitResult.error().message);
        // Không cần rollbackTransaction() ở đây nữa vì commit thất bại thì transaction đã tự rollback hoặc vẫn giữ nguyên.
        return std::unexpected(commitResult.error());
    }

    LOG_INFO("SQLiteAdapter::ensureTablesExist - Database schema and default admin (if needed) ensured successfully.");
    return true;
}