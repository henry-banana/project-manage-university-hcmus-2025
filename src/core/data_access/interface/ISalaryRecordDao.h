/**
 * @file ISalaryRecordDao.h
 * @brief Định nghĩa giao diện DAO cho truy cập dữ liệu lương
 * 
 * Giao diện ISalaryRecordDao định nghĩa các phương thức để truy cập và
 * thao tác với dữ liệu lương của giáo viên trong hệ thống.
 */
#ifndef ISALARYRECORDDAO_H
#define ISALARYRECORDDAO_H

#include "IDao.h"
#include "../../entities/SalaryRecord.h"
// expected và ErrorType đã được IDao.h include

/**
 * @class ISalaryRecordDao
 * @brief Giao diện DAO cho truy cập dữ liệu lương
 * 
 * Giao diện này mở rộng từ IDao<SalaryRecord, std::string> và định nghĩa
 * các phương thức để truy cập và thao tác với dữ liệu lương của giáo viên.
 * Kế thừa tất cả các phương thức từ giao diện IDao cơ sở.
 */
class ISalaryRecordDao : public IDao<SalaryRecord, std::string> {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~ISalaryRecordDao() override = default;
};

#endif // ISALARYRECORDDAO_H