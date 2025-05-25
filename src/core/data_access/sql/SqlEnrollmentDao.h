#ifndef SQLENROLLMENTDAO_H
#define SQLENROLLMENTDAO_H

#include "../interface/IEnrollmentDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h" // Để inject EnrollmentRecordSqlParser
#include <memory>

class SqlEnrollmentDao : public IEnrollmentDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter;
    std::shared_ptr<IEntityParser<EnrollmentRecord, DbQueryResultRow>> _parser;

public:
    SqlEnrollmentDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                     std::shared_ptr<IEntityParser<EnrollmentRecord, DbQueryResultRow>> parser);
    ~SqlEnrollmentDao() override = default;

    std::expected<bool, Error> addEnrollment(const std::string& studentId, const std::string& courseId) override;
    std::expected<bool, Error> removeEnrollment(const std::string& studentId, const std::string& courseId) override;
    std::expected<bool, Error> removeEnrollmentsByStudent(const std::string& studentId) override;
    std::expected<bool, Error> removeEnrollmentsByCourse(const std::string& courseId) override;
    std::expected<bool, Error> isEnrolled(const std::string& studentId, const std::string& courseId) const override;
    std::expected<std::vector<std::string>, Error> findCourseIdsByStudentId(const std::string& studentId) const override;
    std::expected<std::vector<std::string>, Error> findStudentIdsByCourseId(const std::string& courseId) const override;
    std::expected<std::vector<EnrollmentRecord>, Error> getAllEnrollments() const override;
};

#endif // SQLENROLLMENTDAO_H