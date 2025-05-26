#include "SqlParserUtils.h" 
// #include "../../../utils/Logger.h" // Bỏ comment nếu bạn muốn dùng Logger ở đây

template <>
std::string SqlParserUtils::getOptional<std::string>(const DbQueryResultRow& row, const std::string& columnName, std::string defaultValue) {
    auto it = row.find(columnName);
    if (it != row.end() && it->second.has_value()) {
        try {
            // Thử cast trực tiếp sang std::string trước
            return std::any_cast<std::string>(it->second);
        } catch (const std::bad_any_cast&) {
                // Nếu thất bại, thử cast sang const char* (trường hợp giá trị là string literal)
                try { 
                return std::string(std::any_cast<const char*>(it->second));
            } catch (const std::bad_any_cast& e) {
                // Ghi log lỗi nếu cả hai lần cast đều thất bại
                // LOG_WARN("SqlParserUtils: Bad any_cast for string column '" + columnName + "'. Neither std::string nor const char* worked. Error: " + e.what());
                // std::cerr << "SqlParserUtils: Bad any_cast for string column '" << columnName 
                //           << "'. Error: " << e.what() << "\n";
                return defaultValue;
            }
        }
    }
    return defaultValue;
}