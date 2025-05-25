#ifndef COURSESQLPARSER_H
#define COURSESQLPARSER_H

#include "../interface/IEntityParser.h"
#include "../../entities/Course.h"
#include "StudentSqlParser.h" // Để dùng SqlParserUtils

class CourseSqlParser : public IEntityParser<Course, DbQueryResultRow> {
public:
    CourseSqlParser() = default;

    std::expected<Course, Error> parse(const DbQueryResultRow& row) const override;
    std::expected<DbQueryResultRow, Error> serialize(const Course& course) const override;
    std::expected<std::vector<std::any>, Error> toQueryInsertParams(const Course& course) const override;
    std::expected<std::vector<std::any>, Error> toQueryUpdateParams(const Course& course) const override;
};

#endif // COURSESQLPARSER_H