#ifndef TABLE_DISPLAYER_H
#define TABLE_DISPLAYER_H

#include <vector>
#include <string>
#include <map> // Nếu dòng là map
#include <iomanip> // for std::setw
#include <iostream>

class TableDisplayer {
public:
    TableDisplayer() = default;

    // Hiển thị bảng với header và các dòng dữ liệu (mỗi dòng là vector string)
    void displayTable(const std::vector<std::string>& headers,
                      const std::vector<std::vector<std::string>>& rows,
                      const std::vector<int>& columnWidths = {}) const;

    // (Optional) Template method để hiển thị danh sách các object bất kỳ
    // template<typename TEntity>
    // void displayEntities(const std::vector<std::string>& headers,
    //                      const std::vector<TEntity>& entities,
    //                      std::function<std::vector<std::string>(const TEntity&)> rowMapper) const;
};

#endif