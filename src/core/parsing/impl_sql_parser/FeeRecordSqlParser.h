#ifndef FEERECORDSQLPARSER_H
#define FEERECORDSQLPARSER_H

#include "../interface/IEntityParser.h"
#include "../../entities/FeeRecord.h"
#include "StudentSqlParser.h" // Để dùng SqlParserUtils

class FeeRecordSqlParser : public IEntityParser<FeeRecord, DbQueryResultRow> {
public:
    FeeRecordSqlParser() = default;

    std::expected<FeeRecord, Error> parse(const DbQueryResultRow& row) const override;
    std::expected<DbQueryResultRow, Error> serialize(const FeeRecord& record) const override;
    std::expected<std::vector<std::any>, Error> toQueryInsertParams(const FeeRecord& record) const override;
    std::expected<std::vector<std::any>, Error> toQueryUpdateParams(const FeeRecord& record) const override;
};

#endif // FEERECORDSQLPARSER_H