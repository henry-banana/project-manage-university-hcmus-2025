#ifndef IENTITYPARSER_H
#define IENTITYPARSER_H

/**
 * @file IEntityParser.h
 * @brief Định nghĩa giao diện cho bộ phân tích và chuyển đổi đối tượng Entity
 */

#include <vector>
#include <string>
#include <any> // For DbQueryResultRow in SQL context
#include <map> // For DbQueryResultRow
#include <expected>
#include "../../../common/ErrorType.h" // For Error

/**
 * @class IEntityParser
 * @brief Giao diện cho bộ phân tích và chuyển đổi đối tượng Entity
 * 
 * @tparam TEntity Kiểu dữ liệu của đối tượng Entity
 * @tparam TInputRow Kiểu dữ liệu của một dòng đầu vào (ví dụ: std::vector<std::string> cho CSV, 
 *                   std::map<std::string, std::any> cho SQL row)
 */
template <typename TEntity, typename TInputRow>
class IEntityParser {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IEntityParser() = default;

    /**
     * @brief Phân tích một dòng đầu vào thành một đối tượng TEntity.
     * @param row Dòng dữ liệu đầu vào.
     * @return std::expected chứa đối tượng TEntity nếu thành công, hoặc Error nếu thất bại.
     */
    virtual std::expected<TEntity, Error> parse(const TInputRow& row) const = 0;

    /**
     * @brief Chuyển đổi một đối tượng TEntity thành một dòng đầu ra (TInputRow).
     *        Dùng cho việc chuẩn bị dữ liệu để ghi vào CSDL hoặc file.
     *        Lưu ý: Đối với SQL, TInputRow có thể không trực tiếp là cái được ghi,
     *        mà là một cấu trúc trung gian như std::vector<DbQueryParam>
     *        hoặc map tên cột -> giá trị. Chúng ta sẽ điều chỉnh nếu cần.
     *        Hiện tại, giả định TInputRow có thể đại diện cho dữ liệu entity.
     * @param entity Đối tượng TEntity cần chuyển đổi.
     * @return std::expected chứa TInputRow nếu thành công, hoặc Error nếu thất bại.
     */
    virtual std::expected<TInputRow, Error> serialize(const TEntity& entity) const = 0;    // (Optional) If preparing parameters for SQL (INSERT/UPDATE) is complex enough,

    /**
     * @brief Chuyển đổi một đối tượng TEntity thành một vector các tham số truy vấn INSERT
     * @param entity Đối tượng TEntity cần chuyển đổi
     * @return Vector các tham số nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<std::any>, Error> toQueryInsertParams(const TEntity& entity) const = 0;
    
    /**
     * @brief Chuyển đổi một đối tượng TEntity thành một vector các tham số truy vấn UPDATE
     * @param entity Đối tượng TEntity cần chuyển đổi
     * @return Vector các tham số nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<std::any>, Error> toQueryUpdateParams(const TEntity& entity) const = 0;

};

/**
 * @typedef DbQueryResultRow
 * @brief Kiểu dữ liệu đại diện cho một hàng kết quả từ truy vấn cơ sở dữ liệu
 */
using DbQueryResultRow = std::map<std::string, std::any>;
#endif // IENTITYPARSER_H