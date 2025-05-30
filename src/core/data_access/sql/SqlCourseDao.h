/**
 * @file SqlCourseDao.h
 * @brief Định nghĩa lớp DAO truy cập dữ liệu khóa học từ cơ sở dữ liệu SQL
 * 
 * Lớp SqlCourseDao cung cấp các phương thức truy cập và thao tác với dữ liệu
 * khóa học được lưu trữ trong cơ sở dữ liệu SQL.
 */
#ifndef SQLCOURSEDAO_H
#define SQLCOURSEDAO_H

#include "../interface/ICourseDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h"

/**
 * @class SqlCourseDao
 * @brief Lớp DAO truy cập dữ liệu khóa học từ cơ sở dữ liệu SQL
 * 
 * Lớp này triển khai giao diện ICourseDao và cung cấp các phương thức
 * để thao tác với dữ liệu khóa học trong cơ sở dữ liệu SQL.
 */
class SqlCourseDao : public ICourseDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter; ///< Adapter kết nối cơ sở dữ liệu
    std::shared_ptr<IEntityParser<Course, DbQueryResultRow>> _parser; ///< Parser chuyển đổi dữ liệu

public:
    /**
     * @brief Hàm khởi tạo
     * @param dbAdapter Adapter kết nối cơ sở dữ liệu
     * @param parser Parser chuyển đổi dữ liệu
     */
    SqlCourseDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                 std::shared_ptr<IEntityParser<Course, DbQueryResultRow>> parser);
    
    /**
     * @brief Hàm hủy ảo mặc định
     */
    ~SqlCourseDao() override = default;    /**
     * @brief Lấy khóa học theo ID
     * @param id ID của khóa học
     * @return Đối tượng Course nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Course, Error> getById(const std::string& id) const override;
    
    /**
     * @brief Lấy danh sách tất cả khóa học
     * @return Danh sách các đối tượng Course nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Course>, Error> getAll() const override;
    
    /**
     * @brief Thêm khóa học mới
     * @param course Đối tượng Course cần thêm
     * @return Đối tượng Course đã thêm nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Course, Error> add(const Course& course) override;
    
    /**
     * @brief Cập nhật thông tin khóa học
     * @param course Đối tượng Course cần cập nhật
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> update(const Course& course) override;
    
    /**
     * @brief Xóa khóa học theo ID
     * @param id ID của khóa học cần xóa
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> remove(const std::string& id) override;
    
    /**
     * @brief Kiểm tra sự tồn tại của khóa học theo ID
     * @param id ID của khóa học cần kiểm tra
     * @return true nếu tồn tại, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> exists(const std::string& id) const override;
    
    /**
     * @brief Tìm khóa học theo ID khoa
     * @param facultyId ID của khoa
     * @return Danh sách các đối tượng Course thuộc khoa nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Course>, Error> findByFacultyId(const std::string& facultyId) const override;
};

#endif // SQLCOURSEDAO_H