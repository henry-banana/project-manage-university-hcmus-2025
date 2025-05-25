/**
 * @file SqlTeacherDao.h
 * @brief Định nghĩa lớp DAO truy cập dữ liệu giáo viên từ cơ sở dữ liệu SQL
 * 
 * Lớp SqlTeacherDao cung cấp các phương thức truy cập và thao tác với dữ liệu
 * giáo viên được lưu trữ trong cơ sở dữ liệu SQL.
 */
#ifndef SQLTEACHERDAO_H
#define SQLTEACHERDAO_H

#include "../interface/ITeacherDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h"

/**
 * @class SqlTeacherDao
 * @brief Lớp DAO truy cập dữ liệu giáo viên từ cơ sở dữ liệu SQL
 * 
 * Lớp này triển khai giao diện ITeacherDao và cung cấp các phương thức
 * để thao tác với dữ liệu giáo viên trong cơ sở dữ liệu SQL.
 */
class SqlTeacherDao : public ITeacherDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter; ///< Adapter kết nối cơ sở dữ liệu
    std::shared_ptr<IEntityParser<Teacher, DbQueryResultRow>> _parser; ///< Parser chuyển đổi dữ liệu

public:
    /**
     * @brief Hàm khởi tạo
     * @param dbAdapter Adapter kết nối cơ sở dữ liệu
     * @param parser Parser chuyển đổi dữ liệu
     */
    SqlTeacherDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                  std::shared_ptr<IEntityParser<Teacher, DbQueryResultRow>> parser);
    
    /**
     * @brief Hàm hủy mặc định
     */
    ~SqlTeacherDao() override = default;

    /**
     * @brief Lấy thông tin giáo viên theo ID
     * @param id ID của giáo viên
     * @return Đối tượng Teacher nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Teacher, Error> getById(const std::string& id) const override;
    
    /**
     * @brief Lấy danh sách tất cả giáo viên
     * @return Danh sách các đối tượng Teacher nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Teacher>, Error> getAll() const override;
    
    /**
     * @brief Thêm giáo viên mới
     * @param teacher Đối tượng Teacher cần thêm
     * @return Đối tượng Teacher đã thêm nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Teacher, Error> add(const Teacher& teacher) override;
    
    /**
     * @brief Cập nhật thông tin giáo viên
     * @param teacher Đối tượng Teacher cần cập nhật
     * @return true nếu thành công, hoặc Error nếu thất bại
     */    
    std::expected<bool, Error> update(const Teacher& teacher) override;
    
    /**
     * @brief Xóa giáo viên theo ID
     * @param id ID của giáo viên cần xóa
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> remove(const std::string& id) override;
    
    /**
     * @brief Kiểm tra sự tồn tại của giáo viên theo ID
     * @param id ID của giáo viên cần kiểm tra
     * @return true nếu tồn tại, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> exists(const std::string& id) const override;

    /**
     * @brief Tìm kiếm giáo viên theo ID khoa
     * @param facultyId ID của khoa
     * @return Danh sách các đối tượng Teacher thuộc khoa, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Teacher>, Error> findByFacultyId(const std::string& facultyId) const override;
    
    /**
     * @brief Tìm kiếm giáo viên theo chức danh
     * @param designation Chức danh cần tìm
     * @return Danh sách các đối tượng Teacher có chức danh tương ứng, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Teacher>, Error> findByDesignation(const std::string& designation) const override;
    
    /**
     * @brief Tìm kiếm giáo viên theo email
     * @param email Địa chỉ email cần tìm
     * @return Đối tượng Teacher với email tương ứng, hoặc Error nếu thất bại
     */
    std::expected<Teacher, Error> findByEmail(const std::string& email) const override;
};

#endif // SQLTEACHERDAO_H