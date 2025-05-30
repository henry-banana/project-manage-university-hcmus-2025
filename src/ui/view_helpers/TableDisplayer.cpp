#include "TableDisplayer.h"

/**
 * @brief Hiển thị dữ liệu dạng bảng với tiêu đề và các hàng dữ liệu
 * 
 * Phương thức này hiển thị dữ liệu trong định dạng bảng với các đường viền,
 * bao gồm tiêu đề cột và các hàng dữ liệu. Độ rộng cột được tự động tính toán
 * hoặc có thể được chỉ định trước.
 * 
 * @param headers Mảng chứa tiêu đề các cột
 * @param rows Mảng hai chiều chứa dữ liệu của các hàng và cột
 * @param initialColumnWidths Mảng chứa độ rộng ban đầu cho mỗi cột (tùy chọn)
 */
void TableDisplayer::displayTable(const std::vector<std::string>& headers,
                                  const std::vector<std::vector<std::string>>& rows,
                                  const std::vector<int>& initialColumnWidths) const {
    if (headers.empty()) {
        std::cout << "No headers to display.\n";
        return;
    }

    size_t numColumns = headers.size();
    std::vector<int> columnWidths = initialColumnWidths;

    // Nếu không cung cấp độ rộng cột, hoặc số lượng không khớp, tự tính toán
    if (columnWidths.size() != numColumns) {
        columnWidths.assign(numColumns, 0);
        for (size_t i = 0; i < numColumns; ++i) {
            columnWidths[i] = static_cast<int>(headers[i].length());
        }
        for (const auto& row : rows) {
            if (row.size() != numColumns) {
                std::cerr << "Warning: Row size mismatch in TableDisplayer. Skipping row.\n";
                continue;
            }
            for (size_t i = 0; i < numColumns; ++i) {
                if (static_cast<int>(row[i].length()) > columnWidths[i]) {
                    columnWidths[i] = static_cast<int>(row[i].length());
                }
            }
        }
    }
    
    // Thêm padding
    for (int& width : columnWidths) {
        width += 2; // 1 padding mỗi bên
    }

    /**
     * @brief Hàm lambda in đường kẻ ngang cho bảng
     * 
     * Hàm này in đường kẻ ngang với độ dài phù hợp với độ rộng của mỗi cột
     */
    auto printHorizontalLine = [&]() {
        for (int width : columnWidths) {
            std::cout << "+";
            for (int i = 0; i < width; ++i) std::cout << "-";
        }
        std::cout << "+\n";
    };

    // In header
    printHorizontalLine();
    std::cout << "|";
    for (size_t i = 0; i < numColumns; ++i) {
        std::cout << " " << std::left << std::setw(columnWidths[i] -1) << headers[i] << "|";
    }
    std::cout << "\n";
    printHorizontalLine();

    // In các dòng dữ liệu
    if (rows.empty()) {
        std::cout << "|";
        // Tính tổng độ rộng cho thông báo "No data"
        int totalWidth = 0;
        for(size_t i = 0; i < numColumns; ++i) totalWidth += columnWidths[i] + (i > 0 ? 1 : 0);
        if (numColumns > 0) totalWidth += (numColumns -1); // for the inner "|"

        std::string noDataMsg = " No data available ";
        std::cout << std::left << std::setw(totalWidth > static_cast<int>(noDataMsg.length()) ? totalWidth : static_cast<int>(noDataMsg.length()) ) << noDataMsg;
        std::cout << (numColumns > 0 ? "|" : "") << "\n"; // Chỉ thêm | cuối nếu có cột
    } else {
        for (const auto& row : rows) {
            if (row.size() != numColumns) continue; // Bỏ qua dòng lỗi (đã cảnh báo)
            std::cout << "|";
            for (size_t i = 0; i < numColumns; ++i) {
                std::cout << " " << std::left << std::setw(columnWidths[i] - 1) << row[i] << "|";
            }
            std::cout << "\n";
        }
    }
    printHorizontalLine();
}