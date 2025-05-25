#ifndef TEACHERSQLPARSER_H
#define TEACHERSQLPARSER_H

#include "../interface/IEntityParser.h"
#include "../../entities/Teacher.h"
#include "../../entities/User.h" // Vì Teacher kế thừa User
#include "../../entities/Birthday.h"
#include "../../../common/LoginStatus.h"
#include "../../../common/UserRole.h"
#include "StudentSqlParser.h" // Để dùng SqlParserUtils

class TeacherSqlParser : public IEntityParser<Teacher, DbQueryResultRow> {
public:
    TeacherSqlParser() = default;

    std::expected<Teacher, Error> parse(const DbQueryResultRow& row) const override;
    std::expected<DbQueryResultRow, Error> serialize(const Teacher& teacher) const override;
    std::expected<std::vector<std::any>, Error> toQueryInsertParams(const Teacher& teacher) const override;
    std::expected<std::vector<std::any>, Error> toQueryUpdateParams(const Teacher& teacher) const override;
};

#endif // TEACHERSQLPARSER_H