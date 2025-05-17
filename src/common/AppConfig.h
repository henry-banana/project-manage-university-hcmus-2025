#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <string>
#include <map>
#include <filesystem> // For paths
#include "DataSourceType.h"
#include "EntityType.h"    // Giả sử bạn định nghĩa EntityType
#include "UserRole.h"      // Để biết log level
#include "../utils/Logger.h" // Logger


// // Forward declaration (nếu Logger::Level phức tạp, nếu đơn giản thì include Logger.h)
// namespace LoggerNamespace { // Đặt Logger::Level vào namespace để tránh trùng
//     enum class Level;
// }


struct AppConfig {
    DataSourceType dataSourceType = DataSourceType::MOCK;
    std::map<EntityType, std::filesystem::path> csvFilePaths;
    std::string sqlConnectionString;

    Logger::Level logLevel = Logger::Level::INFO; // Mặc định là INFO
    std::filesystem::path logFilePath = "logs/app.log";

    // // Dùng cho SQL
    // std::string dbHost;
    // int dbPort = 0;
    // std::string dbUser;
    // std::string dbPassword;
    // std::string dbName;

    // // Getter tiện lợi cho đường dẫn CSV
    // std::string getCsvPath(const std::string& entityName) const {
    //     auto it = csvPaths.find(entityName);
    //     if (it != csvPaths.end()) {
    //         return it->second;
    //     }
    //     // Hoặc throw exception, hoặc trả về chuỗi rỗng và để DAO xử lý
    //     return "";
    // }
};



#endif // APPCONFIG_H