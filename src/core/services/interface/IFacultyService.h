/**
 * @file IFacultyService.h
 * @brief Định nghĩa giao diện dịch vụ quản lý khoa
 * 
 * File này định nghĩa giao diện IFacultyService, cung cấp các phương thức
 * để quản lý thông tin khoa trong hệ thống quản lý trường đại học,
 * bao gồm thêm, sửa, xóa, và truy vấn khoa.
 */
#ifndef IFACULTYSERVICE_H
#define IFACULTYSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/Faculty.h"

/**
 * @class IFacultyService
 * @brief Giao diện dịch vụ quản lý khoa
 * 
 * Giao diện này định nghĩa các phương thức để quản lý thông tin khoa
 * trong hệ thống, bao gồm thêm, sửa, xóa, và truy vấn khoa.
 */
class IFacultyService {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IFacultyService() = default;
    
    /**
     * @brief Lấy thông tin khoa theo ID
     * 
     * @param facultyId ID của khoa cần lấy thông tin
     * @return Đối tượng Faculty nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<Faculty, Error> getFacultyById(const std::string& facultyId) const = 0;
    
    /**
     * @brief Lấy thông tin khoa theo tên
     * 
     * @param name Tên của khoa cần lấy thông tin
     * @return Đối tượng Faculty nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<Faculty, Error> getFacultyByName(const std::string& name) const = 0;
    
    /**
     * @brief Lấy danh sách tất cả khoa
     * @return Danh sách khoa nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Faculty>, Error> getAllFaculties() const = 0;
    
    /**
     * @brief Thêm khoa mới
     * 
     * @param id ID của khoa mới
     * @param name Tên của khoa mới
     * @return Đối tượng Faculty mới nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<Faculty, Error> addFaculty(const std::string& id, const std::string& name) = 0;
    
    /**
     * @brief Cập nhật thông tin khoa
     * 
     * @param facultyId ID của khoa cần cập nhật
     * @param newName Tên mới của khoa
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> updateFaculty(const std::string& facultyId, const std::string& newName) = 0;
    
    /**
     * @brief Xóa khoa
     * 
     * @param facultyId ID của khoa cần xóa
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> removeFaculty(const std::string& facultyId) = 0;
};

#endif // IFACULTYSERVICE_H