#ifndef IFEERECORDDAO_H
#define IFEERECORDDAO_H

#include "IDao.h" // FeeRecord là IEntity, có thể dùng IDao
#include "../../entities/FeeRecord.h"

// ID của FeeRecord là studentId
class IFeeRecordDao : public IDao<FeeRecord, std::string> {
public:
    virtual ~IFeeRecordDao() override = default;
    // getById(studentId) đã có từ IDao.
    // getAll() đã có từ IDao (lấy tất cả record học phí).
};

#endif // IFEERECORDDAO_H