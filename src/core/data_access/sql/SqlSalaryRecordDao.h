/**
 * @file SqlSalaryRecordDao.h
 * @brief Định nghĩa lớp DAO truy cập dữ liệu lương từ cơ sở dữ liệu SQL
 * 
 * Lớp SqlSalaryRecordDao cung cấp các phương thức truy cập và thao tác với
 * dữ liệu lương của giáo viên được lưu trữ trong cơ sở dữ liệu SQL.
 */
#ifndef SQLSALARYRECORDDAO_H
#define SQLSALARYRECORDDAO_H

#include "../interface/ISalaryRecordDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h"

/**
 * @class SqlSalaryRecordDao
 * @brief Lớp DAO truy cập dữ liệu lương từ cơ sở dữ liệu SQL
 * 
 * Lớp này triển khai giao diện ISalaryRecordDao và cung cấp các phương thức
 * để thao tác với dữ liệu lương của giáo viên trong cơ sở dữ liệu SQL.
 */
class SqlSalaryRecordDao : public ISalaryRecordDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter; ///< Adapter kết nối cơ sở dữ liệu
    std::shared_ptr<IEntityParser<SalaryRecord, DbQueryResultRow>> _parser; ///< Parser chuyển đổi dữ liệu

public:
    /**
     * @brief Hàm khởi tạo
     * @param dbAdapter Adapter kết nối cơ sở dữ liệu
     * @param parser Parser chuyển đổi dữ liệu
     */
    SqlSalaryRecordDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                       std::shared_ptr<IEntityParser<SalaryRecord, DbQueryResultRow>> parser);
    
    /**
     * @brief Hàm hủy mặc định
     */
    ~SqlSalaryRecordDao() override = default;

    /**
     * @brief Lấy thông tin lương theo ID giáo viên
     * @param teacherId ID của giáo viên
     * @return Đối tượng SalaryRecord nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<SalaryRecord, Error> getById(const std::string& teacherId) const override;
    
    /**
     * @brief Lấy danh sách tất cả bản ghi lương
     * @return Danh sách các đối tượng SalaryRecord nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::vector<SalaryRecord>, Error> getAll() const override;
    
    /**
     * @brief Thêm bản ghi lương mới
     * @param salaryRecord Đối tượng SalaryRecord cần thêm
     * @return Đối tượng SalaryRecord đã thêm nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<SalaryRecord, Error> add(const SalaryRecord& salaryRecord) override;
    
    /**
     * @brief Cập nhật thông tin lương
     * @param salaryRecord Đối tượng SalaryRecord cần cập nhật
     * @return true nếu thành công, hoặc Error nếu thất bại
     */    
    std::expected<bool, Error> update(const SalaryRecord& salaryRecord) override;
    
    /**
     * @brief Xóa bản ghi lương theo ID giáo viên
     * @param teacherId ID của giáo viên
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> remove(const std::string& teacherId) override;
    
    /**
     * @brief Kiểm tra sự tồn tại của bản ghi lương theo ID giáo viên
     * @param teacherId ID của giáo viên
     * @return true nếu tồn tại, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> exists(const std::string& teacherId) const override;
};

#endif // SQLSALARYRECORDDAO_H