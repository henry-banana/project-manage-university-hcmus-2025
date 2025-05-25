#ifndef SQLFACULTYDAO_H
#define SQLFACULTYDAO_H

#include "../interface/IFacultyDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h" // Để inject parser

class SqlFacultyDao : public IFacultyDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter;
    std::shared_ptr<IEntityParser<Faculty, DbQueryResultRow>> _parser;

public:
    SqlFacultyDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                  std::shared_ptr<IEntityParser<Faculty, DbQueryResultRow>> parser);

    ~SqlFacultyDao() override = default;

    std::expected<Faculty, Error> getById(const std::string& id) const override;
    std::expected<std::vector<Faculty>, Error> getAll() const override;
    std::expected<Faculty, Error> add(const Faculty& faculty) override;
    std::expected<bool, Error> update(const Faculty& faculty) override;
    std::expected<bool, Error> remove(const std::string& id) override;
    std::expected<bool, Error> exists(const std::string& id) const override;
    std::expected<Faculty, Error> findByName(const std::string& name) const override;
};

#endif // SQLFACULTYDAO_H