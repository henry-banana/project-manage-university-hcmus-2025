/**
 * @file ITeacherService.h
 * @brief Định nghĩa giao diện dịch vụ quản lý giáo viên
 * 
 * Giao diện ITeacherService định nghĩa các phương thức cần thiết để
 * xử lý nghiệp vụ liên quan đến quản lý giáo viên.
 */
#ifndef ITEACHERSERVICE_H
#define ITEACHERSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/Teacher.h"
#include "../../entities/Birthday.h"

/**
 * @struct TeacherUpdateData
 * @brief Cấu trúc dữ liệu để cập nhật thông tin giáo viên
 * 
 * Cấu trúc này chứa các trường thông tin có thể cập nhật của giáo viên,
 * mỗi trường được bọc trong std::optional để chỉ cập nhật các trường có giá trị.
 */
struct TeacherUpdateData {
    std::string teacherIdToUpdate; ///< ID của giáo viên cần cập nhật
    std::optional<std::string> firstName; ///< Tên của giáo viên
    std::optional<std::string> lastName; ///< Họ của giáo viên
    std::optional<Birthday> birthday; ///< Ngày sinh
    std::optional<std::string> address; ///< Địa chỉ
    std::optional<std::string> citizenId; ///< Số CMND/CCCD
    std::optional<std::string> email; ///< Địa chỉ email
    std::optional<std::string> phoneNumber; ///< Số điện thoại
    std::optional<std::string> facultyId; ///< ID khoa
    std::optional<std::string> qualification; ///< Trình độ học vấn
    std::optional<std::string> specializationSubjects; ///< Môn học chuyên môn
    std::optional<std::string> designation; ///< Chức danh
    std::optional<int> experienceYears; ///< Số năm kinh nghiệm
};

/**
 * @class ITeacherService
 * @brief Giao diện dịch vụ quản lý giáo viên
 * 
 * Giao diện này định nghĩa các phương thức để xử lý nghiệp vụ
 * liên quan đến quản lý giáo viên.
 */
class ITeacherService {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */    virtual ~ITeacherService() = default;
    
    /**
     * @brief Lấy thông tin chi tiết của giáo viên theo ID
     * @param teacherId ID của giáo viên
     * @return Đối tượng Teacher nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<Teacher, Error> getTeacherDetails(const std::string& teacherId) const = 0;
    
    /**
     * @brief Lấy danh sách tất cả giáo viên
     * @return Danh sách các đối tượng Teacher nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Teacher>, Error> getAllTeachers() const = 0;
    
    /**
     * @brief Lấy danh sách giáo viên theo khoa
     * @param facultyId ID của khoa
     * @return Danh sách các đối tượng Teacher thuộc khoa, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Teacher>, Error> getTeachersByFaculty(const std::string& facultyId) const = 0;
    
    /**
     * @brief Lấy danh sách giáo viên theo chức danh
     * @param designation Chức danh cần tìm
     * @return Danh sách các đối tượng Teacher có chức danh tương ứng, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Teacher>, Error> getTeachersByDesignation(const std::string& designation) const = 0;
    
    /**
     * @brief Cập nhật thông tin giáo viên
     * @param data Cấu trúc dữ liệu chứa thông tin cần cập nhật
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> updateTeacherDetails(const TeacherUpdateData& data) = 0;
};

#endif // ITEACHERSERVICE_H