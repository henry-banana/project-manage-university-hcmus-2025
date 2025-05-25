#ifndef TABLE_DISPLAYER_H
#define TABLE_DISPLAYER_H

#include <vector>
#include <string>
#include <map> 
#include <iomanip> 
#include <iostream>
#include <numeric> // Cho std::accumulate (nếu cần tính tổng độ rộng)
#include <algorithm> // Cho std::max_element (nếu cần tự tính độ rộng)


class TableDisplayer {
public:
    TableDisplayer() = default;

    void displayTable(const std::vector<std::string>& headers,
                      const std::vector<std::vector<std::string>>& rows,
                      const std::vector<int>& columnWidths = {}) const;
};

#endif