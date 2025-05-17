// src/common/AppConfig.h
#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <string>
#include <map>

enum class DataSourceType {
    MOCK,
    CSV,
    SQL_MYSQL // Cụ thể hơn nếu có nhiều loại SQL
    // SQLITE,
    // POSTGRESQL
};

// Helper function (nếu cần thiết cho logging hoặc debug)
inline std::string dataSourceTypeToString(DataSourceType type) {
    switch (type) {
        case DataSourceType::MOCK: return "MOCK";
        case DataSourceType::CSV: return "CSV";
        case DataSourceType::SQL_MYSQL: return "SQL_MYSQL";
        default: return "UNKNOWN_DATA_SOURCE";
    }
}

struct AppConfig {
    DataSourceType dataSourceType = DataSourceType::MOCK; // Mặc định

    // Dùng cho CSV
    std::map<std::string, std::string> csvPaths; // Key: "students", "teachers", etc. Value: "data/students.csv"

    // // Dùng cho SQL
    // std::string dbHost;
    // int dbPort = 0;
    // std::string dbUser;
    // std::string dbPassword;
    // std::string dbName;

    // Dùng cho Logger (ví dụ)
    std::string logFilePath = "logs/app.log";
    std::string logLevelStr = "INFO"; // Sẽ được chuyển thành enum Level trong Logger

    // Getter tiện lợi cho đường dẫn CSV
    std::string getCsvPath(const std::string& entityName) const {
        auto it = csvPaths.find(entityName);
        if (it != csvPaths.end()) {
            return it->second;
        }
        // Hoặc throw exception, hoặc trả về chuỗi rỗng và để DAO xử lý
        return "";
    }
};

#endif // APP_CONFIG_H