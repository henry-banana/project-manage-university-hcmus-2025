#ifndef FACULTYSQLPARSER_H
#define FACULTYSQLPARSER_H

#include "../interface/IEntityParser.h"
#include "../../entities/Faculty.h"
#include "StudentSqlParser.h" // Để dùng SqlParserUtils

class FacultySqlParser : public IEntityParser<Faculty, DbQueryResultRow> {
public:
    FacultySqlParser() = default;

    std::expected<Faculty, Error> parse(const DbQueryResultRow& row) const override;
    std::expected<DbQueryResultRow, Error> serialize(const Faculty& faculty) const override;
    std::expected<std::vector<std::any>, Error> toQueryInsertParams(const Faculty& faculty) const override;
    std::expected<std::vector<std::any>, Error> toQueryUpdateParams(const Faculty& faculty) const override;
};

#endif // FACULTYSQLPARSER_H