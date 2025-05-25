/**
 * @file IStudentService.h
 * @brief Định nghĩa giao diện dịch vụ quản lý sinh viên
 * 
 * File này định nghĩa giao diện IStudentService, cung cấp các phương thức
 * để quản lý thông tin sinh viên trong hệ thống quản lý trường đại học,
 * bao gồm thêm, sửa, xóa, và truy vấn sinh viên.
 */
#ifndef ISTUDENTSERVICE_H
#define ISTUDENTSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/Student.h"
#include "../../entities/Birthday.h" // (➕) Cần cho StudentUpdateData
#include "../../../common/LoginStatus.h"

/**
 * @struct StudentUpdateData
 * @brief Cấu trúc chứa dữ liệu cập nhật thông tin sinh viên
 * 
 * Cấu trúc này được sử dụng để cập nhật thông tin của sinh viên,
 * chỉ cập nhật các trường được chỉ định mà không ảnh hưởng đến các trường khác.
 */
struct StudentUpdateData {
    std::string studentIdToUpdate; ///< ID của sinh viên cần cập nhật
    std::optional<std::string> firstName; ///< Tên mới (nếu có)
    std::optional<std::string> lastName; ///< Họ mới (nếu có)
    std::optional<Birthday> birthday; ///< Ngày sinh mới (nếu có)
    std::optional<std::string> address; ///< Địa chỉ mới (nếu có)
    std::optional<std::string> citizenId; ///< Số CCCD mới (nếu có)
    std::optional<std::string> email; ///< Email mới (nếu có)
    std::optional<std::string> phoneNumber; ///< Số điện thoại mới (nếu có)
    std::optional<std::string> facultyId; ///< Mã khoa mới (nếu có)
};

/**
 * @class IStudentService
 * @brief Giao diện dịch vụ quản lý sinh viên
 * 
 * Giao diện này định nghĩa các phương thức để quản lý thông tin sinh viên
 * trong hệ thống, bao gồm thêm, sửa, xóa, và truy vấn sinh viên.
 */
class IStudentService {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IStudentService() = default;    /**
     * @brief Lấy thông tin chi tiết của sinh viên
     * 
     * @param studentId ID của sinh viên cần lấy thông tin
     * @return Đối tượng Student nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<Student, Error> getStudentDetails(const std::string& studentId) const = 0;
    
    /**
     * @brief Lấy danh sách tất cả sinh viên
     * @return Danh sách sinh viên nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Student>, Error> getAllStudents() const = 0;
    
    /**
     * @brief Lấy danh sách sinh viên theo khoa
     * 
     * @param facultyId ID của khoa cần lấy danh sách sinh viên
     * @return Danh sách sinh viên nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Student>, Error> getStudentsByFaculty(const std::string& facultyId) const = 0;
    
    /**
     * @brief Cập nhật thông tin sinh viên
     * 
     * @param data Dữ liệu cập nhật thông tin sinh viên
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> updateStudentDetails(const StudentUpdateData& data) = 0;
};

#endif // ISTUDENTSERVICE_H