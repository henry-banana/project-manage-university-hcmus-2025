#ifndef ISALARYRECORDDAO_H
#define ISALARYRECORDDAO_H

#include "IDao.h"
#include "../../entities/SalaryRecord.h"

// ID của SalaryRecord là teacherId
class ISalaryRecordDao : public IDao<SalaryRecord, std::string> {
public:
    virtual ~ISalaryRecordDao() override = default;
};

#endif // ISALARYRECORDDAO_H