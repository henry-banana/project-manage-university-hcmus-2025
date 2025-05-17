#ifndef ISALARYRECORDDAO_H
#define ISALARYRECORDDAO_H

#include "IDao.h"
#include "../../entities/SalaryRecord.h" // Cần full definition

class ISalaryRecordDao : public IDao<SalaryRecord> {
public:
    virtual ~ISalaryRecordDao() override = default;
};

#endif // ISALARYRECORDDAO_H