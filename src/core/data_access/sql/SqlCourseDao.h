#ifndef SQLCOURSEDAO_H
#define SQLCOURSEDAO_H

#include "../interface/ICourseDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h"

class SqlCourseDao : public ICourseDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter;
    std::shared_ptr<IEntityParser<Course, DbQueryResultRow>> _parser;

public:
    SqlCourseDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                 std::shared_ptr<IEntityParser<Course, DbQueryResultRow>> parser);
    ~SqlCourseDao() override = default;

    std::expected<Course, Error> getById(const std::string& id) const override;
    std::expected<std::vector<Course>, Error> getAll() const override;
    std::expected<Course, Error> add(const Course& course) override;
    std::expected<bool, Error> update(const Course& course) override;
    std::expected<bool, Error> remove(const std::string& id) override;
    std::expected<bool, Error> exists(const std::string& id) const override;
    std::expected<std::vector<Course>, Error> findByFacultyId(const std::string& facultyId) const override;
};

#endif // SQLCOURSEDAO_H