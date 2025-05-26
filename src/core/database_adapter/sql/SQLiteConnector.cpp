#include "SQLiteConnector.h"
#include <iostream> // Tạm thời cho std::cerr
#include <utility>  // Cho std::move

SQLiteConnector::SQLiteConnector(const std::string& dbPath)
    : _db(nullptr), _dbPath(dbPath) {
    // std::cout << "SQLiteConnector created for path: " << _dbPath << "\n"; // Debug
}

SQLiteConnector::~SQLiteConnector() {
    // std::cout << "SQLiteConnector for path " << _dbPath << " being destroyed.\n"; // Debug
    disconnect();
}

// Move constructor
SQLiteConnector::SQLiteConnector(SQLiteConnector&& other) noexcept
    : _db(other._db), _dbPath(std::move(other._dbPath)) {
    other._db = nullptr; // Quan trọng: Chuyển quyền sở hữu con trỏ _db
    // std::cout << "SQLiteConnector move constructed for path: " << _dbPath << "\n"; // Debug
}

// Move assignment operator
SQLiteConnector& SQLiteConnector::operator=(SQLiteConnector&& other) noexcept {
    if (this != &other) {
        disconnect(); // Đảm bảo ngắt kết nối hiện tại (nếu có)

        _db = other._db;
        _dbPath = std::move(other._dbPath);

        other._db = nullptr; // Chuyển quyền sở hữu
        // std::cout << "SQLiteConnector move assigned for path: " << _dbPath << "\n"; // Debug
    }
    return *this;
}


bool SQLiteConnector::connect() {
    if (_db) { // Đã kết nối rồi
        // std::cout << "SQLiteConnector::connect - Already connected to " << _dbPath << "\n"; // Debug
        return true;
    }
    // SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE: Mở để đọc/ghi, tạo nếu chưa có
    // SQLITE_OPEN_FULLMUTEX: Đảm bảo thread-safe ở mức độ kết nối (SQLite sẽ tự quản lý mutex)
    int rc = sqlite3_open_v2(_dbPath.c_str(), &_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQLiteConnector Error: Cannot open database '" << _dbPath << "': " << sqlite3_errmsg(_db) << "\n";
        sqlite3_close(_db); // Quan trọng: close db handle ngay cả khi open thất bại để giải phóng tài nguyên
        _db = nullptr;
        return false;
    }
    // std::cout << "SQLiteConnector::connect - Successfully connected to " << _dbPath << "\n"; // Debug
    return true;
}

void SQLiteConnector::disconnect() {
    if (_db) {
        // std::cout << "SQLiteConnector::disconnect - Disconnecting from " << _dbPath << "\n"; // Debug
        sqlite3_close_v2(_db); // sqlite3_close_v2 an toàn hơn nếu có prepared statements chưa finalize
        _db = nullptr;
    }
}

bool SQLiteConnector::isConnected() const {
    return _db != nullptr;
}

bool SQLiteConnector::executeNonQuery(const std::string& sqlQuery) {
    if (!_db) {
        std::cerr << "SQLiteConnector Error: Database not connected. Cannot execute query.\n";
        return false;
    }
    char* errMsg = nullptr;
    int rc = sqlite3_exec(_db, sqlQuery.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQLiteConnector SQL error in executeNonQuery: " << errMsg << " (Query: " << sqlQuery << ")\n";
        sqlite3_free(errMsg); // Phải giải phóng errMsg
        return false;
    }
    return true;
}

std::string SQLiteConnector::getLastError() const {
    if (_db) {
        return sqlite3_errmsg(_db);
    }
    return "Database not connected.";
}

sqlite3* SQLiteConnector::getDbHandle() const {
    return _db;
}