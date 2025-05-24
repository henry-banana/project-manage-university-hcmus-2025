#include "TableDisplayer.h"

void TableDisplayer::displayTable(const std::vector<std::string>& headers,
                                  const std::vector<std::vector<std::string>>& rows,
                                  const std::vector<int>& initialColumnWidths) const {
    if (headers.empty()) {
        std::cout << "No headers to display." << std::endl;
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
                std::cerr << "Warning: Row size mismatch in TableDisplayer. Skipping row." << std::endl;
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

    // In dòng kẻ ngang
    auto printHorizontalLine = [&]() {
        for (int width : columnWidths) {
            std::cout << "+";
            for (int i = 0; i < width; ++i) std::cout << "-";
        }
        std::cout << "+" << std::endl;
    };

    // In header
    printHorizontalLine();
    std::cout << "|";
    for (size_t i = 0; i < numColumns; ++i) {
        std::cout << " " << std::left << std::setw(columnWidths[i] -1) << headers[i] << "|";
    }
    std::cout << std::endl;
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
        std::cout << (numColumns > 0 ? "|" : "") << std::endl; // Chỉ thêm | cuối nếu có cột
    } else {
        for (const auto& row : rows) {
            if (row.size() != numColumns) continue; // Bỏ qua dòng lỗi (đã cảnh báo)
            std::cout << "|";
            for (size_t i = 0; i < numColumns; ++i) {
                std::cout << " " << std::left << std::setw(columnWidths[i] - 1) << row[i] << "|";
            }
            std::cout << std::endl;
        }
    }
    printHorizontalLine();
}