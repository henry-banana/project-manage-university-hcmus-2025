/**
 * @file IFeeRecordDao.h
 * @brief Định nghĩa giao diện DAO cho truy cập dữ liệu học phí
 * 
 * Giao diện IFeeRecordDao định nghĩa các phương thức để truy cập và
 * thao tác với dữ liệu học phí của sinh viên trong hệ thống.
 */
#ifndef IFEERECORDDAO_H
#define IFEERECORDDAO_H

#include "IDao.h"
#include "../../entities/FeeRecord.h"
// expected và ErrorType đã được IDao.h include

/**
 * @class IFeeRecordDao
 * @brief Giao diện DAO cho truy cập dữ liệu học phí
 * 
 * Giao diện này mở rộng từ IDao<FeeRecord, std::string> và định nghĩa
 * các phương thức để truy cập và thao tác với dữ liệu học phí của sinh viên.
 * Kế thừa tất cả các phương thức từ giao diện IDao cơ sở.
 */
class IFeeRecordDao : public IDao<FeeRecord, std::string> {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IFeeRecordDao() override = default;
};

#endif // IFEERECORDDAO_H