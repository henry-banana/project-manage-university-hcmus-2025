#ifndef SQLTEACHERDAO_H
#define SQLTEACHERDAO_H

#include "../interface/ITeacherDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h"

class SqlTeacherDao : public ITeacherDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter;
    std::shared_ptr<IEntityParser<Teacher, DbQueryResultRow>> _parser;

public:
    SqlTeacherDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                  std::shared_ptr<IEntityParser<Teacher, DbQueryResultRow>> parser);
    ~SqlTeacherDao() override = default;

    std::expected<Teacher, Error> getById(const std::string& id) const override;
    std::expected<std::vector<Teacher>, Error> getAll() const override;
    std::expected<Teacher, Error> add(const Teacher& teacher) override;
    std::expected<bool, Error> update(const Teacher& teacher) override;
    std::expected<bool, Error> remove(const std::string& id) override;
    std::expected<bool, Error> exists(const std::string& id) const override;

    std::expected<std::vector<Teacher>, Error> findByFacultyId(const std::string& facultyId) const override;
    std::expected<std::vector<Teacher>, Error> findByDesignation(const std::string& designation) const override;
    std::expected<Teacher, Error> findByEmail(const std::string& email) const override;
};

#endif // SQLTEACHERDAO_H