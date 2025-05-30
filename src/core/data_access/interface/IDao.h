/**
 * @file IDao.h
 * @brief Định nghĩa giao diện DAO cơ bản cho truy cập dữ liệu
 * 
 * Giao diện IDao định nghĩa các phương thức cơ bản để truy cập và
 * thao tác với dữ liệu thông qua mẫu thiết kế DAO (Data Access Object).
 */
#ifndef IDAO_H
#define IDAO_H

#include <vector>
#include <string>
#include <memory>
#include <expected> // (➕) Thêm include này
#include "../../../common/ErrorType.h" // Để dùng Error struct
#include "../../entities/IEntity.h"

/**
 * @class IDao
 * @brief Giao diện DAO cơ bản cho truy cập dữ liệu
 * 
 * @tparam TEntity Kiểu dữ liệu của thực thể
 * @tparam TId Kiểu dữ liệu của định danh (mặc định là std::string)
 * 
 * Giao diện này định nghĩa các phương thức cơ bản để truy cập và
 * thao tác với dữ liệu cho một loại thực thể cụ thể.
 */
template <typename TEntity, typename TId = std::string>
class IDao {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IDao() = default;

    /**
     * @brief Lấy thực thể theo ID
     * @param id ID của thực thể cần lấy
     * @return Đối tượng thực thể nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<TEntity, Error> getById(const TId& id) const = 0;
    
    /**
     * @brief Lấy danh sách tất cả thực thể
     * @return Danh sách các đối tượng thực thể nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<TEntity>, Error> getAll() const = 0;
    // virtual std::expected<std::vector<TEntity>, Error> getAll(PagingParams paging, SortParams sorting) const = 0;

    /**
     * @brief Thêm thực thể mới
     * @param entity Đối tượng thực thể cần thêm
     * @return Đối tượng thực thể đã thêm nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<TEntity, Error> add(const TEntity& entity) = 0;
    
    /**
     * @brief Cập nhật thông tin thực thể
     * @param entity Đối tượng thực thể cần cập nhật
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> update(const TEntity& entity) = 0;
    
    /**
     * @brief Xóa thực thể theo ID
     * @param id ID của thực thể cần xóa
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> remove(const TId& id) = 0;

    /**
     * @brief Kiểm tra sự tồn tại của thực thể theo ID
     * @param id ID của thực thể cần kiểm tra
     * @return true nếu tồn tại, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> exists(const TId& id) const = 0;
};

#endif // IDAO_H