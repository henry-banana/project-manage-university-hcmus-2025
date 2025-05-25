#ifndef SQLCOURSERESULTDAO_H
#define SQLCOURSERESULTDAO_H

#include "../interface/ICourseResultDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h"
#include <memory>

class SqlCourseResultDao : public ICourseResultDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter;
    std::shared_ptr<IEntityParser<CourseResult, DbQueryResultRow>> _parser;

public:
    SqlCourseResultDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                       std::shared_ptr<IEntityParser<CourseResult, DbQueryResultRow>> parser);
    ~SqlCourseResultDao() override = default;

    std::expected<CourseResult, Error> find(const std::string& studentId, const std::string& courseId) const override;
    std::expected<std::vector<CourseResult>, Error> findByStudentId(const std::string& studentId) const override;
    std::expected<std::vector<CourseResult>, Error> findByCourseId(const std::string& courseId) const override;
    std::expected<bool, Error> addOrUpdate(const CourseResult& result) override;
    std::expected<bool, Error> remove(const std::string& studentId, const std::string& courseId) override;
    std::expected<bool, Error> removeAllForStudent(const std::string& studentId) override;
    std::expected<bool, Error> removeAllForCourse(const std::string& courseId) override;
};

#endif // SQLCOURSERESULTDAO_H