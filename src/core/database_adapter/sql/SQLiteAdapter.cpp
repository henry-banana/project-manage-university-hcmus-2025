#include "SQLiteAdapter.h"
#include "sqlite3.h" // Header của thư viện SQLite
#include <iostream>  // Tạm thời cho logging/debug
#include <stdexcept> // Cho các exception (nếu cần)
#include <vector>
#include <map>
#include <any>
#include "../../../utils/Logger.h" // (➕) Sử dụng Logger chính thức

SQLiteAdapter::SQLiteAdapter() : _connector(nullptr), _isConnected(false) {
    LOG_DEBUG("SQLiteAdapter: Instance created.");
}

SQLiteAdapter::~SQLiteAdapter() {
    LOG_DEBUG("SQLiteAdapter: Instance being destroyed.");
    if (_isConnected) {
        // Không cần gọi disconnect() ở đây nữa vì _connector là unique_ptr
        // unique_ptr sẽ tự gọi destructor của SQLiteConnector khi SQLiteAdapter bị hủy.
        // SQLiteConnector destructor sẽ gọi disconnect().
    }
}

std::expected<bool, Error> SQLiteAdapter::connect(const std::string& dbPath) {
    if (_isConnected && _connector) {
        LOG_INFO("SQLiteAdapter::connect - Already connected to " + _dbPath + ". Disconnecting first.");
        // Không cần gọi disconnect trực tiếp ở đây nữa, unique_ptr sẽ xử lý khi reset
    }

    _dbPath = dbPath;
    // Tạo instance mới của SQLiteConnector, unique_ptr cũ (nếu có) sẽ tự động bị hủy
    _connector = std::make_unique<SQLiteConnector>(_dbPath);

    if (_connector->connect()) {
        _isConnected = true;
        LOG_INFO("SQLiteAdapter: Successfully connected to database: " + _dbPath);

        // Kích hoạt Foreign Keys (quan trọng!)
        // Sử dụng executeUpdate của chính Adapter này để đảm bảo nhất quán
        std::string fk_pragma = "PRAGMA foreign_keys = ON;";
        char* errMsg = nullptr;
        int rc = sqlite3_exec(_connector->getDbHandle(), fk_pragma.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::string fkErrMsg = "SQLiteAdapter: Failed to enable foreign keys: " + std::string(errMsg ? errMsg : "Unknown SQLite error");
            sqlite3_free(errMsg); // Luôn giải phóng errMsg
            LOG_ERROR(fkErrMsg);
            // Ngắt kết nối nếu không bật được FK
            if (_connector) _connector->disconnect(); // Gọi trực tiếp connector
            _isConnected = false;
            _connector.reset();
            return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, fkErrMsg});
        }
        sqlite3_free(errMsg); // Giải phóng nếu không có lỗi (errMsg sẽ là NULL)
        LOG_INFO("SQLiteAdapter: Foreign key enforcement enabled.");
        return true;
    } else {
        std::string connErrMsg = _connector->getLastError(); // Connector đã log lỗi chi tiết
        _connector.reset(); // Giải phóng connector nếu kết nối thất bại
        _isConnected = false;
        // Không cần LOG_ERROR ở đây nữa vì Connector đã log
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, "Failed to connect via SQLiteConnector: " + connErrMsg});
    }
}

std::expected<bool, Error> SQLiteAdapter::disconnect() {
    if (!_isConnected) {
        LOG_INFO("SQLiteAdapter: Already disconnected.");
        return true;
    }
    if (_connector) {
        _connector->disconnect(); // SQLiteConnector sẽ xử lý việc đóng _db
        _connector.reset();     // Giải phóng unique_ptr
    }
    _isConnected = false;
    LOG_INFO("SQLiteAdapter: Disconnected from database: " + _dbPath);
    return true;
}

bool SQLiteAdapter::isConnected() const {
    return _isConnected && _connector && _connector->isConnected();
}

// Helper function để bind tham số vào sqlite3_stmt
Error SQLiteAdapter::bindParameters(sqlite3_stmt* stmt, const std::vector<DbQueryParam>& params) {
    // Tham số trong SQLite được đánh số từ 1
    for (size_t i = 0; i < params.size(); ++i) {
        const auto& param_any = params[i];
        int bind_idx = static_cast<int>(i + 1);
        int rc = SQLITE_OK;

        if (!param_any.has_value()) { // Xử lý std::any rỗng (tương đương SQL NULL)
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
        // Thêm các kiểu BLOB, ... nếu cần
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
    // Trả về một Error "rỗng" nếu thành công, hoặc một Error có code=0, message=""
    // Để nhất quán, nên trả về Error với code SQLITE_OK
    return Error{SQLITE_OK, ""}; // SQLITE_OK (0) biểu thị thành công
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
        if (stmt) sqlite3_finalize(stmt); // Dọn dẹp stmt nếu prepare lỗi nhưng stmt khác NULL
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
    }

    Error bindErr = bindParameters(stmt, params);
    if (bindErr.code != SQLITE_OK) { // SQLITE_OK là 0
        sqlite3_finalize(stmt);
        // bindErr đã được log bên trong hàm bindParameters
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
                    columnValue = sqlite3_column_int64(stmt, i); // Luôn đọc là int64
                    break;
                case SQLITE_FLOAT:
                    columnValue = sqlite3_column_double(stmt, i);
                    break;
                case SQLITE_TEXT:
                    {
                        const unsigned char* text = sqlite3_column_text(stmt, i);
                        // Kiểm tra NULL pointer trước khi tạo string
                        columnValue = text ? std::string(reinterpret_cast<const char*>(text)) : std::string();
                    }
                    break;
                case SQLITE_BLOB:
                    // Hiện tại chỉ log, chưa xử lý đầy đủ BLOB
                    LOG_WARN("SQLiteAdapter::executeQuery - BLOB data type encountered at column '" + columnName + "' but not fully handled.");
                    columnValue = std::string("[BLOB_DATA_PLACEHOLDER]"); // Hoặc std::vector<unsigned char>()
                    break;
                case SQLITE_NULL:
                    columnValue = std::any{}; // std::any rỗng biểu thị NULL
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
        // Lỗi xảy ra trong quá trình step
        std::string errMsg = "SQLiteAdapter::executeQuery - Failed to step through results: " +
                             std::string(sqlite3_errmsg(_connector->getDbHandle())) + " | Query: " + sqlQuery;
        LOG_ERROR(errMsg);
        sqlite3_finalize(stmt);
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
    }

    sqlite3_finalize(stmt); // Luôn phải gọi
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
        // Đối với các lệnh không phải SELECT, SQLITE_DONE là dấu hiệu thành công.
        // Các lỗi như SQLITE_CONSTRAINT sẽ có rc_step khác SQLITE_DONE.
        std::string errMsg = "SQLiteAdapter::executeUpdate - Failed to execute statement (SQLite code: " + std::to_string(rc_step) + "): " +
                             std::string(sqlite3_errmsg(_connector->getDbHandle()));
        LOG_ERROR(errMsg + " | Query: " + sqlQuery);
        sqlite3_finalize(stmt);
        // Trả về lỗi cụ thể từ SQLite nếu có
        return std::unexpected(Error{rc_step, errMsg}); // Dùng mã lỗi của SQLite
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

    // Câu lệnh PRAGMA để bật foreign keys nên được chạy mỗi khi kết nối,
    // nhưng việc kiểm tra và tạo bảng chỉ cần thiết nếu chúng chưa tồn tại.
    // Hàm connect đã bật foreign keys.

    // Thứ tự tạo bảng quan trọng do có Foreign Keys
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
                citizenId TEXT UNIQUE, -- Nên NOT NULL cho Student/Teacher
                email TEXT UNIQUE,     -- Nên NOT NULL cho Student/Teacher
                phoneNumber TEXT,
                role INTEGER NOT NULL, -- Sẽ map với enum UserRole
                status INTEGER NOT NULL  -- Sẽ map với enum LoginStatus
            ) WITHOUT ROWID;
        )SQL"},
        {"Students", R"SQL(
            CREATE TABLE IF NOT EXISTS Students (
                userId TEXT PRIMARY KEY,    -- FK to Users.id
                facultyId TEXT,
                FOREIGN KEY (userId) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE,
                FOREIGN KEY (facultyId) REFERENCES Faculties(id) ON DELETE SET NULL ON UPDATE CASCADE
            ) WITHOUT ROWID;
        )SQL"},
        {"Teachers", R"SQL(
            CREATE TABLE IF NOT EXISTS Teachers (
                userId TEXT PRIMARY KEY,    -- FK to Users.id
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
                userId TEXT PRIMARY KEY,    -- FK to Users.id
                passwordHash TEXT NOT NULL,
                salt TEXT NOT NULL,
                -- role và status có thể lấy từ bảng Users, hoặc lưu riêng ở đây để truy vấn login nhanh hơn
                -- Nếu lưu riêng, cần đảm bảo đồng bộ. Tạm thời không lưu ở đây.
                FOREIGN KEY (userId) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE
            ) WITHOUT ROWID;
        )SQL"},
        {"Enrollments", R"SQL(
            CREATE TABLE IF NOT EXISTS Enrollments (
                studentId TEXT NOT NULL, -- FK to Users.id (cụ thể là Student)
                courseId TEXT NOT NULL,  -- FK to Courses.id
                enrollmentDate TEXT,     -- (Optional) ISO8601 string (YYYY-MM-DD HH:MM:SS)
                PRIMARY KEY (studentId, courseId),
                FOREIGN KEY (studentId) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE,
                FOREIGN KEY (courseId) REFERENCES Courses(id) ON DELETE CASCADE ON UPDATE CASCADE
            ); -- Bảng này có thể có rowid tự động của SQLite hoặc khai báo WITHOUT ROWID nếu muốn
        )SQL"},
        {"CourseResults", R"SQL(
            CREATE TABLE IF NOT EXISTS CourseResults (
                studentId TEXT NOT NULL, -- FK to Users.id (Student)
                courseId TEXT NOT NULL,  -- FK to Courses.id
                marks INTEGER,           -- NULLable nếu chưa có điểm (-1 sẽ map sang NULL)
                grade TEXT,              -- (A, B, C, D, F, -, ?)
                PRIMARY KEY (studentId, courseId),
                FOREIGN KEY (studentId) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE,
                FOREIGN KEY (courseId) REFERENCES Courses(id) ON DELETE CASCADE ON UPDATE CASCADE,
                CHECK (marks IS NULL OR (marks >= -1 AND marks <= 100))
            );
        )SQL"},
        {"FeeRecords", R"SQL(
            CREATE TABLE IF NOT EXISTS FeeRecords (
                studentId TEXT PRIMARY KEY, -- FK to Users.id (Student)
                totalFee INTEGER NOT NULL CHECK(totalFee >= 0),
                paidFee INTEGER NOT NULL DEFAULT 0 CHECK(paidFee >= 0),
                FOREIGN KEY (studentId) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE,
                CHECK(paidFee <= totalFee)
            ) WITHOUT ROWID;
        )SQL"},
        {"SalaryRecords", R"SQL(
            CREATE TABLE IF NOT EXISTS SalaryRecords (
                teacherId TEXT PRIMARY KEY, -- FK to Users.id (Teacher)
                basicMonthlyPay INTEGER NOT NULL CHECK(basicMonthlyPay >= 0),
                FOREIGN KEY (teacherId) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE
            ) WITHOUT ROWID;
        )SQL"}
    };

    // Bắt đầu một transaction để đảm bảo tất cả các bảng được tạo hoặc không gì cả
    if (!_connector->executeNonQuery("BEGIN TRANSACTION;")) {
        std::string errMsg = "SQLiteAdapter::ensureTablesExist - Failed to begin transaction: " + _connector->getLastError();
        LOG_ERROR(errMsg);
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
    }

    for (const auto& tableDef : tablesToCreate) {
        LOG_DEBUG("SQLiteAdapter::ensureTablesExist - Creating table if not exists: " + tableDef.first);
        if (!_connector->executeNonQuery(tableDef.second)) {
            std::string errMsg = "SQLiteAdapter::ensureTablesExist - Failed to create table '" + tableDef.first + "': " + _connector->getLastError();
            LOG_ERROR(errMsg);
            _connector->executeNonQuery("ROLLBACK TRANSACTION;"); // Cố gắng rollback
            return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
        }
    }

    if (!_connector->executeNonQuery("COMMIT TRANSACTION;")) {
        std::string errMsg = "SQLiteAdapter::ensureTablesExist - Failed to commit transaction: " + _connector->getLastError();
        LOG_ERROR(errMsg);
        // Ở đây, các bảng có thể đã được tạo, nhưng transaction commit thất bại (hiếm)
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, errMsg});
    }

    LOG_INFO("SQLiteAdapter::ensureTablesExist - Database schema ensured successfully.");
    return true;
}