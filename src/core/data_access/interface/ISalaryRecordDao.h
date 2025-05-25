#ifndef ISALARYRECORDDAO_H
#define ISALARYRECORDDAO_H

#include "IDao.h"
#include "../../entities/SalaryRecord.h"
// expected và ErrorType đã được IDao.h include

class ISalaryRecordDao : public IDao<SalaryRecord, std::string> {
public:
    virtual ~ISalaryRecordDao() override = default;
};

#endif // ISALARYRECORDDAO_H