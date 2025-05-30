#ifndef DATASOURCETYPE_H
#define DATASOURCETYPE_H

/**
 * @enum DataSourceType
 * @brief Liệt kê các loại nguồn dữ liệu trong hệ thống
 * 
 * Enum này định nghĩa các loại nguồn dữ liệu có thể được sử dụng 
 * trong hệ thống để lưu trữ và truy xuất dữ liệu.
 */
enum class DataSourceType {
    MOCK, ///< Dữ liệu giả lập trong bộ nhớ (cho mục đích kiểm thử)
    CSV,  ///< Dữ liệu được lưu trữ trong các file CSV
    SQL   ///< Dữ liệu được lưu trữ trong cơ sở dữ liệu SQL
};

#endif // DATASOURCETYPE_H