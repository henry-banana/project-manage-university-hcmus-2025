/**
 * @file ConfigLoader.h
 * @brief Định nghĩa lớp tải cấu hình ứng dụng từ file
 * 
 * Lớp ConfigLoader cung cấp phương thức để đọc và phân tích cú pháp file cấu hình
 * của ứng dụng (INI, JSON, v.v.) và chuyển đổi thành đối tượng AppConfig.
 */
#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <string>
#include <filesystem>
#include "../common/AppConfig.h" // Để trả về AppConfig object
#include "../common/ErrorType.h"
#include <expected>

/**
 * @class ConfigLoader
 * @brief Lớp tải cấu hình ứng dụng từ file
 * 
 * Lớp này cung cấp các phương thức để đọc và phân tích cú pháp file cấu hình
 * của ứng dụng, hỗ trợ nhiều định dạng file khác nhau.
 */
class ConfigLoader {
private:
    std::filesystem::path _configFilePath; ///< Đường dẫn đến file cấu hình
    
    /**
     * @brief Phân tích cú pháp file INI
     * @return Đối tượng AppConfig nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<AppConfig, Error> parseIniFile() const; // Ví dụ
public:
    /**
     * @brief Hàm khởi tạo với đường dẫn file cấu hình
     * @param configFilePath Đường dẫn đến file cấu hình
     */
    ConfigLoader(const std::filesystem::path& configFilePath);
    
    /**
     * @brief Tải cấu hình từ file
     * @return Đối tượng AppConfig nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<AppConfig, Error> loadConfig() const;
};

/**
 * @brief Phân tích chuỗi thành DataSourceType
 * @param str Chuỗi cần phân tích
 * @return Enum DataSourceType tương ứng
 */
DataSourceType parseDataSourceType(const std::string& str);

/**
 * @brief Phân tích chuỗi thành Logger::Level
 * @param str Chuỗi cần phân tích
 * @return Enum Logger::Level tương ứng
 */
Logger::Level parseLogLevel(const std::string& str);

#endif