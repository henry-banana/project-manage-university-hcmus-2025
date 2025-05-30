/**
 * @file StringUtils.h
 * @brief Định nghĩa các hàm tiện ích xử lý chuỗi
 * 
 * Namespace StringUtils cung cấp các hàm tiện ích để xử lý chuỗi như cắt, chuyển đổi,
 * tìm kiếm và thao tác với chuỗi, giúp đơn giản hóa và chuẩn hóa việc xử lý chuỗi.
 */
#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <vector>
#include <algorithm> // for std::transform
#include <cctype>    // for ::tolower, ::isspace

/**
 * @namespace StringUtils
 * @brief Namespace chứa các hàm tiện ích xử lý chuỗi
 */
namespace StringUtils {
    /**
     * @brief Loại bỏ khoảng trắng ở đầu và cuối chuỗi
     * @param str Chuỗi cần xử lý
     * @return Chuỗi đã được loại bỏ khoảng trắng
     */
    std::string trim(const std::string& str);
    
    /**
     * @brief Chuyển đổi chuỗi thành chữ thường
     * @param str Chuỗi cần chuyển đổi
     * @return Chuỗi đã được chuyển thành chữ thường
     */
    std::string toLower(const std::string& str);
    
    /**
     * @brief Chuyển đổi chuỗi thành chữ hoa
     * @param str Chuỗi cần chuyển đổi
     * @return Chuỗi đã được chuyển thành chữ hoa
     */
    std::string toUpper(const std::string& str);
    
    /**
     * @brief Tách chuỗi thành mảng các chuỗi con theo ký tự phân cách
     * @param str Chuỗi cần tách
     * @param delimiter Ký tự phân cách
     * @return Mảng các chuỗi con
     */
    std::vector<std::string> split(const std::string& str, char delimiter);
    
    /**
     * @brief Kết hợp mảng các chuỗi thành một chuỗi với ký tự phân cách
     * @param elements Mảng các chuỗi cần kết hợp
     * @param delimiter Ký tự phân cách
     * @return Chuỗi đã kết hợp
     */
    std::string join(const std::vector<std::string>& elements, char delimiter);
    
    /**
     * @brief Kiểm tra chuỗi có bắt đầu bằng tiền tố không
     * @param str Chuỗi cần kiểm tra
     * @param prefix Tiền tố
     * @return true nếu chuỗi bắt đầu bằng tiền tố, false nếu ngược lại
     */
    bool startsWith(const std::string& str, const std::string& prefix);
    
    /**
     * @brief Kiểm tra chuỗi có kết thúc bằng hậu tố không
     * @param str Chuỗi cần kiểm tra
     * @param suffix Hậu tố
     * @return true nếu chuỗi kết thúc bằng hậu tố, false nếu ngược lại
     */
    bool endsWith(const std::string& str, const std::string& suffix);
    
    /**
     * @brief Kiểm tra chuỗi có chứa chuỗi con không
     * @param str Chuỗi cần kiểm tra
     * @param substring Chuỗi con
     * @return true nếu chuỗi chứa chuỗi con, false nếu ngược lại
     */
    bool contains(const std::string& str, const std::string& substring);
}

#endif