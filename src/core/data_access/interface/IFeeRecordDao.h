#ifndef IFEERECORDDAO_H
#define IFEERECORDDAO_H

#include "IDao.h"
#include "../../entities/FeeRecord.h"
// expected và ErrorType đã được IDao.h include

class IFeeRecordDao : public IDao<FeeRecord, std::string> {
public:
    virtual ~IFeeRecordDao() override = default;
};

#endif // IFEERECORDDAO_H