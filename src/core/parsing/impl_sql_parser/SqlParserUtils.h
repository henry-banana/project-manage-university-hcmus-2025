/**
 * @file SqlParserUtils.h
 * @brief Định nghĩa các hàm tiện ích cho việc phân tích dữ liệu SQL
 * 
 * Namespace SqlParserUtils cung cấp các hàm tiện ích để chuyển đổi và trích xuất
 * dữ liệu từ các kết quả truy vấn SQL một cách an toàn và thuận tiện.
 */
#ifndef SQLPARSERUTILS_H
#define SQLPARSERUTILS_H

#include <string>
#include <vector>
#include <map>
#include <any>
#include <iostream> // Cho ví dụ debug, có thể xóa nếu không dùng trực tiếp

/**
 * @typedef DbQueryResultRow
 * @brief Kiểu dữ liệu đại diện cho một hàng kết quả từ truy vấn cơ sở dữ liệu
 * 
 * Alias này cũng có trong IEntityParser.h, nhưng để ở đây giúp SqlParserUtils.h
 * có thể được dùng độc lập hơn nếu sau này bạn muốn tách nó hoàn toàn.
 */
using DbQueryResultRow = std::map<std::string, std::any>; 

/**
 * @namespace SqlParserUtils
 * @brief Namespace chứa các hàm tiện ích xử lý dữ liệu SQL
 */
namespace SqlParserUtils {

    /**
     * @brief Lấy giá trị từ một cột trong DbQueryResultRow một cách an toàn
     * 
     * @tparam T Kiểu dữ liệu mong muốn
     * @param row Dòng kết quả từ database
     * @param columnName Tên cột cần lấy
     * @param defaultValue Giá trị trả về nếu không tìm thấy cột hoặc cast thất bại
     * @return Giá trị của cột hoặc defaultValue
     */
    template <typename T>
    inline T getOptional(const DbQueryResultRow& row, const std::string& columnName, T defaultValue = T{}) {
        auto it = row.find(columnName);
        if (it != row.end() && it->second.has_value()) {
            try {
                return std::any_cast<T>(it->second);
            } catch (const std::bad_any_cast& e) {
                // Trong thực tế, nên dùng Logger ở đây thay vì std::cerr
                // LOG_WARN("SqlParserUtils: Bad any_cast for column '" + columnName + "' to expected type. Error: " + e.what());
                // std::cerr << "SqlParserUtils: Bad any_cast for column '" << columnName 
                //           << "' to type (size " << sizeof(T) << "). Error: " << e.what() << std::endl;
                return defaultValue;
            }
        }
        return defaultValue;
    }

    /**
     * @brief Khai báo (declaration) cho specialization của getOptional cho kiểu std::string.
     *        Định nghĩa (definition) sẽ nằm trong file SqlParserUtils.cpp.
     */
    template <>
    std::string getOptional<std::string>(const DbQueryResultRow& row, const std::string& columnName, std::string defaultValue);

} // namespace SqlParserUtils

#endif // SQLPARSERUTILS_H