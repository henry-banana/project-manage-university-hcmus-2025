#ifndef SQLFEERECORDDAO_H
#define SQLFEERECORDDAO_H

#include "../interface/IFeeRecordDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h"

class SqlFeeRecordDao : public IFeeRecordDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter;
    std::shared_ptr<IEntityParser<FeeRecord, DbQueryResultRow>> _parser;

public:
    SqlFeeRecordDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                    std::shared_ptr<IEntityParser<FeeRecord, DbQueryResultRow>> parser);
    ~SqlFeeRecordDao() override = default;

    std::expected<FeeRecord, Error> getById(const std::string& studentId) const override;
    std::expected<std::vector<FeeRecord>, Error> getAll() const override;
    std::expected<FeeRecord, Error> add(const FeeRecord& feeRecord) override;
    std::expected<bool, Error> update(const FeeRecord& feeRecord) override;
    std::expected<bool, Error> remove(const std::string& studentId) override;
    std::expected<bool, Error> exists(const std::string& studentId) const override;
};

#endif // SQLFEERECORDDAO_H