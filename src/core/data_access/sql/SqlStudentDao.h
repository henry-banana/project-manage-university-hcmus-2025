#ifndef SQLSTUDENTDAO_H
#define SQLSTUDENTDAO_H

#include "../interface/IStudentDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h" // Để inject parser StudentSqlParser
#include "../../../common/LoginStatus.h"


class SqlStudentDao : public IStudentDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter;
    std::shared_ptr<IEntityParser<Student, DbQueryResultRow>> _parser;

public:
    SqlStudentDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                  std::shared_ptr<IEntityParser<Student, DbQueryResultRow>> parser);
    ~SqlStudentDao() override = default;

    std::expected<Student, Error> getById(const std::string& id) const override;
    std::expected<std::vector<Student>, Error> getAll() const override;
    std::expected<Student, Error> add(const Student& student) override;
    std::expected<bool, Error> update(const Student& student) override;
    std::expected<bool, Error> remove(const std::string& id) override;
    std::expected<bool, Error> exists(const std::string& id) const override;

    std::expected<std::vector<Student>, Error> findByFacultyId(const std::string& facultyId) const override;
    std::expected<Student, Error> findByEmail(const std::string& email) const override;
    std::expected<std::vector<Student>, Error> findByStatus(LoginStatus status) const override;
    std::expected<bool, Error> updateStatus(const std::string& studentId, LoginStatus newStatus) override;
};

#endif // SQLSTUDENTDAO_H