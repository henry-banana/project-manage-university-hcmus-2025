// --- START OF MODIFIED FILE src/core/data_access/mock/MockSalaryRecordDao.h ---
#ifndef MOCKSALARYRECORDDAO_H
#define MOCKSALARYRECORDDAO_H

#include "../interface/ISalaryRecordDao.h"
#include <map>      // (➕)
#include <string>   // (➕)

class MockSalaryRecordDao : public ISalaryRecordDao {
public:
    MockSalaryRecordDao();
    ~MockSalaryRecordDao() override = default;

    std::expected<SalaryRecord, Error> getById(const std::string& teacherId) const override;
    std::expected<std::vector<SalaryRecord>, Error> getAll() const override;
    std::expected<SalaryRecord, Error> add(const SalaryRecord& salaryRecord) override;
    std::expected<bool, Error> update(const SalaryRecord& salaryRecord) override;
    std::expected<bool, Error> remove(const std::string& teacherId) override;
    std::expected<bool, Error> exists(const std::string& teacherId) const override;

    static void initializeDefaultMockData();
    static void clearMockData();
};

#endif // MOCKSALARYRECORDDAO_H
// --- END OF MODIFIED FILE src/core/data_access/mock/MockSalaryRecordDao.h ---