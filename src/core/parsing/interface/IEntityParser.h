#ifndef IENTITYPARSER_H
#define IENTITYPARSER_H

#include <vector>
#include <string>
#include <any> // For DbQueryResultRow in SQL context
#include <map> // For DbQueryResultRow
#include <expected>
#include "../../../common/ErrorType.h" // For Error

// Forward declaration for specific entity types if needed,
// though typically they will be included by concrete parser implementations.
// class Student; 
// class Course;
// ...

// TInputRow là kiểu dữ liệu của một dòng đầu vào (ví dụ: std::vector<std::string> cho CSV, 
//                                                std::map<std::string, std::any> cho SQL row)
template <typename TEntity, typename TInputRow>
class IEntityParser {
public:
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
    virtual std::expected<TInputRow, Error> serialize(const TEntity& entity) const = 0;

    // (Optional) If preparing parameters for SQL (INSERT/UPDATE) is complex enough,
    // it might warrant its own method distinct from serialize().
    // For now, let's assume serialize() can handle both creating a representation for
    // storage (like a CSV row) and a representation for query parameters (like a map for SQL).

    /**
     * @brief Chuyển đổi một đối tượng TEntity thành một vector các tham số truy vấn cơ sở dữ liệu.
     *        Hàm này đặc biệt hữu ích cho việc chuẩn bị tham số cho các câu lệnh SQL INSERT và UPDATE.
     *        Thứ tự của các tham số trong vector phải khớp với thứ tự placeholder trong câu lệnh SQL.
     * @param entity Đối tượng TEntity cần chuyển đổi.
     * @param includeId true nếu ID của entity cũng nên được bao gồm trong danh sách tham số (ví dụ, cho UPDATE),
     *                  false nếu ID không nên được bao gồm (ví dụ, cho INSERT với ID tự tăng hoặc đã được bind riêng).
     * @return std::expected chứa std::vector<std::any> nếu thành công, hoặc Error nếu thất bại.
     */
    virtual std::expected<std::vector<std::any>, Error> toQueryInsertParams(const TEntity& entity) const = 0;
    
    virtual std::expected<std::vector<std::any>, Error> toQueryUpdateParams(const TEntity& entity) const = 0;

};

// Alias cho kiểu dữ liệu một dòng kết quả từ CSDL SQL
using DbQueryResultRow = std::map<std::string, std::any>;
#endif // IENTITYPARSER_H