/**
 * @file IFacultyDao.h
 * @brief Định nghĩa giao diện DAO cho truy cập dữ liệu khoa
 * 
 * Giao diện IFacultyDao định nghĩa các phương thức để truy cập và
 * thao tác với dữ liệu khoa trong hệ thống.
 */
#ifndef IFACULTYDAO_H
#define IFACULTYDAO_H

#include "IDao.h"
#include "../../entities/Faculty.h"
#include <string>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)

/**
 * @class IFacultyDao
 * @brief Giao diện DAO cho truy cập dữ liệu khoa
 * 
 * Giao diện này mở rộng từ IDao<Faculty, std::string> và định nghĩa
 * các phương thức bổ sung để truy cập và thao tác với dữ liệu khoa.
 */
class IFacultyDao : public IDao<Faculty, std::string> {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IFacultyDao() override = default;
    
    /**
     * @brief Tìm khoa theo tên
     * @param name Tên khoa cần tìm
     * @return Đối tượng Faculty nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<Faculty, Error> findByName(const std::string& name) const = 0;
};

#endif // IFACULTYDAO_H