/**
 * @file IStudentDao.h
 * @brief Định nghĩa giao diện DAO cho truy cập dữ liệu sinh viên
 * 
 * Giao diện IStudentDao định nghĩa các phương thức để truy cập và
 * thao tác với dữ liệu sinh viên trong hệ thống.
 */
#ifndef ISTUDENTDAO_H
#define ISTUDENTDAO_H

#include "IDao.h"
#include "../../entities/Student.h"
#include "../../../common/LoginStatus.h" // (➕) Cần cho findByStatus và updateStatus
#include <string>
#include <vector>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)

/**
 * @class IStudentDao
 * @brief Giao diện DAO cho truy cập dữ liệu sinh viên
 * 
 * Giao diện này mở rộng từ IDao<Student, std::string> và định nghĩa
 * các phương thức bổ sung để truy cập và thao tác với dữ liệu sinh viên.
 */
class IStudentDao : public IDao<Student, std::string> {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IStudentDao() override = default;

    /**
     * @brief Tìm sinh viên theo ID khoa
     * @param facultyId ID của khoa
     * @return Danh sách các đối tượng Student thuộc khoa nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Student>, Error> findByFacultyId(const std::string& facultyId) const = 0;
    
    /**
     * @brief Tìm sinh viên theo địa chỉ email
     * @param email Địa chỉ email cần tìm
     * @return Đối tượng Student nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<Student, Error> findByEmail(const std::string& email) const = 0;
    
    /**
     * @brief Tìm sinh viên theo trạng thái đăng nhập
     * @param status Trạng thái đăng nhập cần lọc
     * @return Danh sách các đối tượng Student có trạng thái đăng nhập tương ứng, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Student>, Error> findByStatus(LoginStatus status) const = 0;
    
    /**
     * @brief Cập nhật trạng thái đăng nhập của sinh viên
     * @param studentId ID của sinh viên
     * @param newStatus Trạng thái đăng nhập mới
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> updateStatus(const std::string& studentId, LoginStatus newStatus) = 0;
};

#endif // ISTUDENTDAO_H