#ifndef IFEERECORDDAO_H
#define IFEERECORDDAO_H

#include "IDao.h"
#include "../../entities/FeeRecord.h" // Cần full definition

class IFeeRecordDao : public IDao<FeeRecord> {
public:
    virtual ~IFeeRecordDao() override = default;
};

#endif // IFEERECORDDAO_H