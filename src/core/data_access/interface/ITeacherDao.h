/**
 * @file ITeacherDao.h
 * @brief Định nghĩa giao diện DAO cho việc truy cập dữ liệu giáo viên
 * 
 * Giao diện ITeacherDao định nghĩa các phương thức cần thiết để truy cập
 * và thao tác với dữ liệu giáo viên, mở rộng từ giao diện DAO cơ sở.
 */
#ifndef ITEACHERDAO_H
#define ITEACHERDAO_H

#include "IDao.h"
#include "../../entities/Teacher.h"
#include <string>
#include <vector>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)

/**
 * @class ITeacherDao
 * @brief Giao diện DAO cho việc truy cập dữ liệu giáo viên
 * 
 * Giao diện này mở rộng từ IDao cơ sở và cung cấp các phương thức
 * cụ thể để truy cập và thao tác với dữ liệu giáo viên.
 */
class ITeacherDao : public IDao<Teacher, std::string> {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~ITeacherDao() override = default;
    
    /**
     * @brief Tìm kiếm giáo viên theo ID khoa
     * @param facultyId ID của khoa
     * @return Danh sách các đối tượng Teacher thuộc khoa, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Teacher>, Error> findByFacultyId(const std::string& facultyId) const = 0;
    
    /**
     * @brief Tìm kiếm giáo viên theo chức danh
     * @param designation Chức danh cần tìm
     * @return Danh sách các đối tượng Teacher có chức danh tương ứng, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Teacher>, Error> findByDesignation(const std::string& designation) const = 0;
    
    /**
     * @brief Tìm kiếm giáo viên theo email
     * @param email Địa chỉ email cần tìm
     * @return Đối tượng Teacher với email tương ứng, hoặc Error nếu thất bại
     */
    virtual std::expected<Teacher, Error> findByEmail(const std::string& email) const = 0;
};
#endif // ITEACHERDAO_H