#include "StringUtils.h"
#include <sstream>  // for std::istringstream

/**
 * @namespace StringUtils
 * @brief Không gian tên chứa các hàm tiện ích xử lý chuỗi
 * 
 * Namespace này cung cấp các hàm tiện ích để xử lý và thao tác với chuỗi trong ứng dụng,
 * bao gồm các thao tác phổ biến như cắt khoảng trắng, chuyển đổi chữ hoa/thường,
 * tách và nối chuỗi, kiểm tra tiền tố/hậu tố và tìm kiếm chuỗi con.
 */
namespace StringUtils {

    /**
     * @brief Loại bỏ khoảng trắng ở đầu và cuối chuỗi
     * 
     * @param str Chuỗi đầu vào cần xử lý
     * @return std::string Chuỗi đã được loại bỏ khoảng trắng ở hai đầu
     */
    std::string trim(const std::string& str) {
        // Find the first non-whitespace character
        size_t start = str.find_first_not_of(" \t\n\r\f\v");
        
        // If the string is all whitespace
        if (start == std::string::npos) {
            return "";
        }
        
        // Find the last non-whitespace character
        size_t end = str.find_last_not_of(" \t\n\r\f\v");
        
        // Return the trimmed substring
        return str.substr(start, end - start + 1);
    }

    /**
     * @brief Chuyển đổi chuỗi thành chữ thường
     * 
     * @param str Chuỗi đầu vào cần chuyển đổi
     * @return std::string Chuỗi đã được chuyển thành chữ thường
     */
    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    /**
     * @brief Chuyển đổi chuỗi thành chữ hoa
     * 
     * @param str Chuỗi đầu vào cần chuyển đổi
     * @return std::string Chuỗi đã được chuyển thành chữ hoa
     */
    std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::toupper(c); });
        return result;
    }

    /**
     * @brief Tách chuỗi thành mảng các chuỗi con dựa vào ký tự phân cách
     * 
     * @param str Chuỗi đầu vào cần tách
     * @param delimiter Ký tự phân cách dùng để tách chuỗi
     * @return std::vector<std::string> Mảng các chuỗi con sau khi tách
     */
    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        
        return tokens;
    }

    /**
     * @brief Nối các phần tử trong mảng chuỗi thành một chuỗi duy nhất
     * 
     * @param elements Mảng các chuỗi cần nối
     * @param delimiter Ký tự phân cách giữa các phần tử
     * @return std::string Chuỗi kết quả sau khi nối
     */
    std::string join(const std::vector<std::string>& elements, char delimiter) {
        std::string result;
        
        for (size_t i = 0; i < elements.size(); ++i) {
            result += elements[i];
            if (i < elements.size() - 1) {
                result += delimiter;
            }
        }
        
        return result;
    }

    /**
     * @brief Kiểm tra xem chuỗi có bắt đầu bằng tiền tố nhất định không
     * 
     * @param str Chuỗi cần kiểm tra
     * @param prefix Tiền tố cần kiểm tra
     * @return true Nếu chuỗi bắt đầu bằng tiền tố
     * @return false Nếu chuỗi không bắt đầu bằng tiền tố
     */
    bool startsWith(const std::string& str, const std::string& prefix) {
        if (str.length() < prefix.length()) {
            return false;
        }
        
        return str.compare(0, prefix.length(), prefix) == 0;
    }

    /**
     * @brief Kiểm tra xem chuỗi có kết thúc bằng hậu tố nhất định không
     * 
     * @param str Chuỗi cần kiểm tra
     * @param suffix Hậu tố cần kiểm tra
     * @return true Nếu chuỗi kết thúc bằng hậu tố
     * @return false Nếu chuỗi không kết thúc bằng hậu tố
     */
    bool endsWith(const std::string& str, const std::string& suffix) {
        if (str.length() < suffix.length()) {
            return false;
        }
        
        return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
    }

    /**
     * @brief Kiểm tra xem chuỗi có chứa chuỗi con nhất định không
     * 
     * @param str Chuỗi cần kiểm tra
     * @param substring Chuỗi con cần tìm
     * @return true Nếu chuỗi chứa chuỗi con
     * @return false Nếu chuỗi không chứa chuỗi con
     */
    bool contains(const std::string& str, const std::string& substring) {
        return str.find(substring) != std::string::npos;
    }

}
