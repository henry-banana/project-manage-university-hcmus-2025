#ifndef SQLPARSERUTILS_H
#define SQLPARSERUTILS_H

#include <string>
#include <vector>
#include <map>
#include <any>
#include <iostream> // Cho ví dụ debug, có thể xóa nếu không dùng trực tiếp

// Alias này cũng có trong IEntityParser.h, nhưng để ở đây giúp SqlParserUtils.h
// có thể được dùng độc lập hơn nếu sau này bạn muốn tách nó hoàn toàn.
// Nếu IEntityParser.h luôn được include trước khi include file này ở những nơi cần thiết,
// bạn có thể bỏ dòng using này để tránh định nghĩa lại.
// Tuy nhiên, using alias thì không gây lỗi multiple definition như hàm.
using DbQueryResultRow = std::map<std::string, std::any>; 

namespace SqlParserUtils {

    /**
     * @brief Lấy giá trị từ một cột trong DbQueryResultRow một cách an toàn.
     *        Hàm template này được định nghĩa inline trong header.
     * @tparam T Kiểu dữ liệu mong muốn.
     * @param row Dòng kết quả từ database.
     * @param columnName Tên cột cần lấy.
     * @param defaultValue Giá trị trả về nếu không tìm thấy cột hoặc cast thất bại.
     * @return Giá trị của cột hoặc defaultValue.
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