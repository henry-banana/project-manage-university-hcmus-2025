#ifndef MOCKSALARYRECORDDAO_H
#define MOCKSALARYRECORDDAO_H

#include "../interface/ISalaryRecordDao.h"
// SalaryRecord đã được ISalaryRecordDao.h include

class MockSalaryRecordDao : public ISalaryRecordDao {
private:
    // std::map<std::string, SalaryRecord> _mockData; // Key là teacherId
public:
    MockSalaryRecordDao();
    ~MockSalaryRecordDao() override = default;

    std::expected<SalaryRecord, Error> getById(const std::string& teacherId) const override;
    std::expected<std::vector<SalaryRecord>, Error> getAll() const override;
    std::expected<SalaryRecord, Error> add(const SalaryRecord& salaryRecord) override;
    std::expected<bool, Error> update(const SalaryRecord& salaryRecord) override;
    std::expected<bool, Error> remove(const std::string& teacherId) override;
    std::expected<bool, Error> exists(const std::string& teacherId) const override;
};

#endif // MOCKSALARYRECORDDAO_H