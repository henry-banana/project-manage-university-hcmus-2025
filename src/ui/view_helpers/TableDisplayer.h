/**
 * @file TableDisplayer.h
 * @brief Định nghĩa lớp hiển thị dữ liệu dạng bảng trong giao diện console
 * 
 * Lớp TableDisplayer cung cấp phương thức để hiển thị dữ liệu dưới dạng bảng
 * có định dạng trong giao diện console, hỗ trợ việc hiển thị thông tin một cách
 * có cấu trúc và dễ đọc.
 */
#ifndef TABLE_DISPLAYER_H
#define TABLE_DISPLAYER_H

#include <vector>
#include <string>
#include <map> 
#include <iomanip> 
#include <iostream>
#include <numeric> // Cho std::accumulate (nếu cần tính tổng độ rộng)
#include <algorithm> // Cho std::max_element (nếu cần tự tính độ rộng)

/**
 * @class TableDisplayer
 * @brief Lớp hiển thị dữ liệu dạng bảng trong giao diện console
 * 
 * Lớp này cung cấp các phương thức để hiển thị dữ liệu dưới dạng bảng có
 * định dạng đẹp, với việc tự động căn chỉnh cột và hàng.
 */
class TableDisplayer {
public:
    /**
     * @brief Hàm khởi tạo mặc định
     */
    TableDisplayer() = default;

    /**
     * @brief Hiển thị dữ liệu dưới dạng bảng
     * @param headers Tiêu đề các cột
     * @param rows Dữ liệu các hàng trong bảng
     * @param columnWidths Độ rộng của mỗi cột (tùy chọn)
     */
    void displayTable(const std::vector<std::string>& headers,
                      const std::vector<std::vector<std::string>>& rows,
                      const std::vector<int>& columnWidths = {}) const;
};

#endif