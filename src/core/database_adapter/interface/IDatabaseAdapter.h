/**
 * @file IDatabaseAdapter.h
 * @brief Định nghĩa giao diện cho bộ điều hợp cơ sở dữ liệu
 * 
 * File này định nghĩa giao diện IDatabaseAdapter, cung cấp một lớp trừu tượng
 * cho việc tương tác với các loại cơ sở dữ liệu khác nhau. Giao diện này cung cấp
 * các phương thức để kết nối, ngắt kết nối, và thực hiện các truy vấn và cập nhật
 * cơ sở dữ liệu.
 */
#ifndef IDATABASEADAPTER_H
#define IDATABASEADAPTER_H


#include <string>
#include <vector>
#include <map>
#include <any>
#include <expected> // Đã thống nhất dùng std::expected
#include "../../../common/ErrorType.h"
// AppConfig không cần ở đây nữa nếu connectionString truyền qua connect()

/**
 * @typedef DbQueryParam
 * @brief Kiểu dữ liệu cho tham số truy vấn cơ sở dữ liệu
 */
using DbQueryParam = std::any;

/**
 * @typedef DbQueryResultRow
 * @brief Kiểu dữ liệu đại diện cho một hàng kết quả từ truy vấn cơ sở dữ liệu
 */
using DbQueryResultRow = std::map<std::string, std::any>;

/**
 * @typedef DbQueryResultTable
 * @brief Kiểu dữ liệu đại diện cho một bảng kết quả từ truy vấn cơ sở dữ liệu
 */
using DbQueryResultTable = std::vector<DbQueryResultRow>;

/**
 * @class IDatabaseAdapter
 * @brief Giao diện cơ sở cho bộ điều hợp cơ sở dữ liệu
 * 
 * Giao diện này định nghĩa các phương thức mà tất cả các bộ điều hợp cơ sở dữ liệu
 * phải triển khai, bao gồm các phương thức để kết nối, ngắt kết nối, kiểm tra kết nối,
 * thực hiện truy vấn và cập nhật, và quản lý giao dịch.
 */
class IDatabaseAdapter {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IDatabaseAdapter() = default;

    /**
     * @brief Kết nối đến cơ sở dữ liệu
     * 
     * @param connectionString Chuỗi kết nối đến cơ sở dữ liệu
     * @return Kết quả thành công hoặc lỗi
     */
    virtual std::expected<bool, Error> connect(const std::string& connectionString) = 0;
    
    /**
     * @brief Ngắt kết nối với cơ sở dữ liệu
     * @return Kết quả thành công hoặc lỗi
     */
    virtual std::expected<bool, Error> disconnect() = 0;
    
    /**
     * @brief Kiểm tra trạng thái kết nối
     * @return true nếu đã kết nối, false nếu chưa
     */
    virtual bool isConnected() const = 0;

    /**
     * @brief Thực hiện truy vấn SELECT
     * 
     * @param sqlQuery Câu lệnh SQL truy vấn
     * @param params Danh sách tham số cho truy vấn
     * @return Bảng kết quả hoặc lỗi
     */
    virtual std::expected<DbQueryResultTable, Error> executeQuery(const std::string& sqlQuery, const std::vector<DbQueryParam>& params = {}) = 0;
    
    /**
     * @brief Thực hiện truy vấn cập nhật (INSERT, UPDATE, DELETE)
     * 
     * @param sqlQuery Câu lệnh SQL cập nhật
     * @param params Danh sách tham số cho truy vấn
     * @return Số hàng bị ảnh hưởng hoặc lỗi
     */
    virtual std::expected<long, Error> executeUpdate(const std::string& sqlQuery, const std::vector<DbQueryParam>& params = {}) = 0;

    /**
     * @brief Bắt đầu một giao dịch
     * @return Kết quả thành công hoặc lỗi
     */
    virtual std::expected<bool, Error> beginTransaction() = 0;
    
    /**
     * @brief Xác nhận giao dịch
     * @return Kết quả thành công hoặc lỗi
     */
    virtual std::expected<bool, Error> commitTransaction() = 0;
    virtual std::expected<bool, Error> rollbackTransaction() = 0;
    virtual bool isInTransaction() const = 0; // (➕) Kiểm tra xem có đang trong transaction không
};

#endif