#ifndef COURSERESULTSQLPARSER_H
#define COURSERESULTSQLPARSER_H

#include "../interface/IEntityParser.h"
#include "../../entities/CourseResult.h"
#include "StudentSqlParser.h" // Để dùng SqlParserUtils

class CourseResultSqlParser : public IEntityParser<CourseResult, DbQueryResultRow> {
public:
    CourseResultSqlParser() = default;

    std::expected<CourseResult, Error> parse(const DbQueryResultRow& row) const override;
    std::expected<DbQueryResultRow, Error> serialize(const CourseResult& result) const override;
    // CourseResult thường được add/update bằng một hàm duy nhất addOrUpdate.
    // Tham số sẽ là studentId, courseId, marks.
    std::expected<std::vector<std::any>, Error> toQueryInsertParams(const CourseResult& result) const override; // studentId, courseId, marks
    std::expected<std::vector<std::any>, Error> toQueryUpdateParams(const CourseResult& result) const override; // marks, studentId, courseId (for WHERE)
};

#endif // COURSERESULTSQLPARSER_H