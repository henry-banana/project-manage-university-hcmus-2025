#include "ConfigLoader.h"
#include <fstream>
#include <sstream>
#include <algorithm> // for std::transform
#include "../../utils/StringUtils.h" // Cho trim, toUpper
#include "../../utils/Logger.h" // Để log

/**
 * @brief Khởi tạo đối tượng ConfigLoader với đường dẫn file cấu hình
 * 
 * @param configFilePath Đường dẫn đến file cấu hình
 */
ConfigLoader::ConfigLoader(const std::filesystem::path& configFilePath)
    : _configFilePath(configFilePath) {}

/**
 * @brief Phân tích loại nguồn dữ liệu từ chuỗi
 * 
 * Hàm này chuyển đổi chuỗi đại diện cho loại nguồn dữ liệu thành 
 * giá trị enum DataSourceType tương ứng.
 * 
 * @param str Chuỗi cần phân tích
 * @return DataSourceType Loại nguồn dữ liệu tương ứng
 */
DataSourceType parseDataSourceType(const std::string& str) {
    std::string upperStr = StringUtils::toUpper(StringUtils::trim(str));
    if (upperStr == "SQL") return DataSourceType::SQL;
    if (upperStr == "CSV") return DataSourceType::CSV;
    if (upperStr == "MOCK") return DataSourceType::MOCK;
    LOG_WARN("ConfigLoader: Unknown DataSourceType '" + str + "', defaulting to MOCK.");
    return DataSourceType::MOCK; // Default
}

/**
 * @brief Phân tích cấp độ log từ chuỗi
 * 
 * Hàm này chuyển đổi chuỗi đại diện cho cấp độ log thành 
 * giá trị enum Logger::Level tương ứng.
 * 
 * @param str Chuỗi cần phân tích
 * @return Logger::Level Cấp độ log tương ứng
 */
Logger::Level parseLogLevel(const std::string& str) {
    std::string upperStr = StringUtils::toUpper(StringUtils::trim(str));
    if (upperStr == "DEBUG") return Logger::Level::DEBUG;
    if (upperStr == "INFO") return Logger::Level::INFO;
    if (upperStr == "WARN") return Logger::Level::WARN;
    if (upperStr == "ERROR") return Logger::Level::ERROR;
    if (upperStr == "CRITICAL") return Logger::Level::CRITICAL;
    LOG_WARN("ConfigLoader: Unknown LogLevel '" + str + "', defaulting to INFO.");
    return Logger::Level::INFO; // Default
}

/**
 * @brief Đọc và phân tích file cấu hình
 * 
 * Phương thức này đọc file cấu hình, phân tích cú pháp INI và
 * trả về một đối tượng AppConfig chứa các thông số cấu hình.
 * 
 * @return std::expected<AppConfig, Error> Đối tượng cấu hình nếu thành công, 
 *         hoặc đối tượng Error nếu có lỗi
 */
std::expected<AppConfig, Error> ConfigLoader::loadConfig() const { // (➕) Thay đổi kiểu trả về
    AppConfig config; 
    std::ifstream configFile(_configFilePath);

    if (!configFile.is_open()) {
        return std::unexpected(Error{ErrorCode::DATA_ACCESS_ERROR, "Cannot open config file: " + _configFilePath.string()});
    }

    std::string line;
    std::string currentSection;

    while (std::getline(configFile, line)) {
        line = StringUtils::trim(line);
        if (line.empty() || line[0] == ';' || line[0] == '#') {
            continue;
        }

        if (line[0] == '[' && line.back() == ']') {
            currentSection = StringUtils::trim(line.substr(1, line.length() - 2));
            continue;
        }

        size_t delimPos = line.find('=');
        if (delimPos != std::string::npos) {
            std::string key = StringUtils::trim(line.substr(0, delimPos));
            std::string value = StringUtils::trim(line.substr(delimPos + 1));

            if (currentSection == "Database") {
                if (key == "DataSourceType") {
                    config.dataSourceType = parseDataSourceType(value);
                } else if (key == "SqlConnectionString") {
                    config.sqlConnectionString = value;
                }
            } else if (currentSection == "Logging") {
                if (key == "LogLevel") {
                    config.logLevel = parseLogLevel(value);
                } else if (key == "LogFilePath") {
                    config.logFilePath = value;
                }
            }
        }
    }
    configFile.close();
    LOG_INFO("Configuration loaded successfully from: " + _configFilePath.string());
    return config; // (➕) Trả về config trực tiếp
}