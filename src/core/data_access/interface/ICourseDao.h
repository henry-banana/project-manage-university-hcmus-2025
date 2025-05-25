/**
 * @file ICourseDao.h
 * @brief Định nghĩa giao diện DAO cho truy cập dữ liệu khóa học
 * 
 * Giao diện ICourseDao định nghĩa các phương thức để truy cập và
 * thao tác với dữ liệu khóa học trong hệ thống.
 */
#ifndef ICOURSEDAO_H
#define ICOURSEDAO_H

#include "IDao.h"
#include "../../entities/Course.h"
#include <string>
#include <vector>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)

/**
 * @class ICourseDao
 * @brief Giao diện DAO cho truy cập dữ liệu khóa học
 * 
 * Giao diện này mở rộng từ IDao<Course, std::string> và định nghĩa
 * các phương thức bổ sung để truy cập và thao tác với dữ liệu khóa học.
 */
class ICourseDao : public IDao<Course, std::string> {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~ICourseDao() override = default;
    
    /**
     * @brief Tìm khóa học theo ID khoa
     * @param facultyId ID của khoa
     * @return Danh sách các đối tượng Course thuộc khoa nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<Course>, Error> findByFacultyId(const std::string& facultyId) const = 0;
};

#endif // ICOURSEDAO_H