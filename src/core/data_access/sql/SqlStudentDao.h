/**
 * @file SqlStudentDao.h
 * @brief Định nghĩa lớp DAO truy cập dữ liệu sinh viên từ cơ sở dữ liệu SQL
 * 
 * Lớp SqlStudentDao cung cấp các phương thức truy cập và thao tác với dữ liệu
 * sinh viên được lưu trữ trong cơ sở dữ liệu SQL.
 */
#ifndef SQLSTUDENTDAO_H
#define SQLSTUDENTDAO_H

#include "../interface/IStudentDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h" // Để inject parser StudentSqlParser
#include "../../../common/LoginStatus.h"


/**
 * @class SqlStudentDao
 * @brief Lớp DAO truy cập dữ liệu sinh viên từ cơ sở dữ liệu SQL
 * 
 * Lớp này triển khai giao diện IStudentDao và cung cấp các phương thức
 * để thao tác với dữ liệu sinh viên trong cơ sở dữ liệu SQL.
 */
class SqlStudentDao : public IStudentDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter; ///< Adapter kết nối cơ sở dữ liệu
    std::shared_ptr<IEntityParser<Student, DbQueryResultRow>> _parser; ///< Parser chuyển đổi dữ liệu

public:
    /**
     * @brief Hàm khởi tạo
     * @param dbAdapter Adapter kết nối cơ sở dữ liệu
     * @param parser Parser chuyển đổi dữ liệu
     */
    SqlStudentDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                  std::shared_ptr<IEntityParser<Student, DbQueryResultRow>> parser);
    
    /**
     * @brief Hàm hủy ảo mặc định
     */
    ~SqlStudentDao() override = default;    /**
     * @brief Lấy sinh viên theo ID
     * @param id ID của sinh viên
     * @return Đối tượng Student nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Student, Error> getById(const std::string& id) const override;
    
    /**
     * @brief Lấy danh sách tất cả sinh viên
     * @return Danh sách các đối tượng Student nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Student>, Error> getAll() const override;
    
    /**
     * @brief Thêm sinh viên mới
     * @param student Đối tượng Student cần thêm
     * @return Đối tượng Student đã thêm nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Student, Error> add(const Student& student) override;
    
    /**
     * @brief Cập nhật thông tin sinh viên
     * @param student Đối tượng Student cần cập nhật
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> update(const Student& student) override;
    
    /**
     * @brief Xóa sinh viên theo ID
     * @param id ID của sinh viên cần xóa
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> remove(const std::string& id) override;
    
    /**
     * @brief Kiểm tra sự tồn tại của sinh viên theo ID
     * @param id ID của sinh viên cần kiểm tra
     * @return true nếu tồn tại, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> exists(const std::string& id) const override;    /**
     * @brief Tìm sinh viên theo ID khoa
     * @param facultyId ID của khoa
     * @return Danh sách các đối tượng Student thuộc khoa nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Student>, Error> findByFacultyId(const std::string& facultyId) const override;
    
    /**
     * @brief Tìm sinh viên theo địa chỉ email
     * @param email Địa chỉ email cần tìm
     * @return Đối tượng Student nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Student, Error> findByEmail(const std::string& email) const override;
    
    /**
     * @brief Tìm sinh viên theo trạng thái đăng nhập
     * @param status Trạng thái đăng nhập cần lọc
     * @return Danh sách các đối tượng Student có trạng thái đăng nhập tương ứng, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Student>, Error> findByStatus(LoginStatus status) const override;
    
    /**
     * @brief Cập nhật trạng thái đăng nhập của sinh viên
     * @param studentId ID của sinh viên
     * @param newStatus Trạng thái đăng nhập mới
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> updateStatus(const std::string& studentId, LoginStatus newStatus) override;
};

#endif // SQLSTUDENTDAO_H