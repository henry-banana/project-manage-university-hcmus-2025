#ifndef SQLSALARYRECORDDAO_H
#define SQLSALARYRECORDDAO_H

#include "../interface/ISalaryRecordDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h"

class SqlSalaryRecordDao : public ISalaryRecordDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter;
    std::shared_ptr<IEntityParser<SalaryRecord, DbQueryResultRow>> _parser;

public:
    SqlSalaryRecordDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                       std::shared_ptr<IEntityParser<SalaryRecord, DbQueryResultRow>> parser);
    ~SqlSalaryRecordDao() override = default;

    std::expected<SalaryRecord, Error> getById(const std::string& teacherId) const override;
    std::expected<std::vector<SalaryRecord>, Error> getAll() const override;
    std::expected<SalaryRecord, Error> add(const SalaryRecord& salaryRecord) override;
    std::expected<bool, Error> update(const SalaryRecord& salaryRecord) override;
    std::expected<bool, Error> remove(const std::string& teacherId) override;
    std::expected<bool, Error> exists(const std::string& teacherId) const override;
};

#endif // SQLSALARYRECORDDAO_H