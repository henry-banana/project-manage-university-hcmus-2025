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

/**
 * @struct AppConfig
 * @brief Cấu trúc lưu trữ cấu hình ứng dụng
 * 
 * Cấu trúc này chứa các thông số cấu hình cho ứng dụng, bao gồm loại nguồn dữ liệu,
 * đường dẫn đến các file CSV, chuỗi kết nối SQL, cấp độ ghi log, và đường dẫn file log.
 */
struct AppConfig {
    DataSourceType dataSourceType = DataSourceType::MOCK; ///< Loại nguồn dữ liệu (MOCK, CSV, SQL)
    std::map<EntityType, std::filesystem::path> csvFilePaths; ///< Ánh xạ từ loại thực thể đến đường dẫn file CSV
    std::string sqlConnectionString; ///< Chuỗi kết nối SQL

    Logger::Level logLevel = Logger::Level::INFO; ///< Cấp độ ghi log (mặc định là INFO)
    std::filesystem::path logFilePath = "logs/app.log"; ///< Đường dẫn đến file log

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