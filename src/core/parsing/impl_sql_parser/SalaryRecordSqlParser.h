#ifndef SALARYRECORDSQLPARSER_H
#define SALARYRECORDSQLPARSER_H

#include "../interface/IEntityParser.h"
#include "../../entities/SalaryRecord.h"
#include "StudentSqlParser.h" // Để dùng SqlParserUtils

class SalaryRecordSqlParser : public IEntityParser<SalaryRecord, DbQueryResultRow> {
public:
    SalaryRecordSqlParser() = default;

    std::expected<SalaryRecord, Error> parse(const DbQueryResultRow& row) const override;
    std::expected<DbQueryResultRow, Error> serialize(const SalaryRecord& record) const override;
    std::expected<std::vector<std::any>, Error> toQueryInsertParams(const SalaryRecord& record) const override;
    std::expected<std::vector<std::any>, Error> toQueryUpdateParams(const SalaryRecord& record) const override;
};

#endif // SALARYRECORDSQLPARSER_H