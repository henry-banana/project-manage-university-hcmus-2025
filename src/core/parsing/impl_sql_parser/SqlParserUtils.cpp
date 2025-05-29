#include "SqlParserUtils.h" 
#include "../../../utils/Logger.h" 

template <>
std::string SqlParserUtils::getOptional<std::string>(const DbQueryResultRow& row, const std::string& columnName, std::string defaultValue) {
    auto it = row.find(columnName);
    if (it != row.end() && it->second.has_value()) {
        try {
            // Thử cast trực tiếp sang std::string trước
            return std::any_cast<std::string>(it->second);
        } catch (const std::bad_any_cast&) {
                // Nếu thất bại, thử cast sang const char* (trường hợp giá trị là string literal từ DB)
                try { 
                const char* cstr_val = std::any_cast<const char*>(it->second);
                if (cstr_val) { // Kiểm tra con trỏ không null
                    return std::string(cstr_val);
                }
                // Nếu const char* là null, trả về defaultValue
                return defaultValue;
            } catch (const std::bad_any_cast& e) {
                LOG_WARN("SqlParserUtils: Bad any_cast for string column '" + columnName + "'. Neither std::string nor const char* worked. Error: " + e.what());
                return defaultValue;
            }
        }
    }
    return defaultValue;
}