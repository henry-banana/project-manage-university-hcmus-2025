#ifndef ISTUDENTDAO_H
#define ISTUDENTDAO_H

#include "IDao.h"
#include "../../entities/Student.h"
#include "../../../common/LoginStatus.h" // (➕) Cần cho findByStatus và updateStatus
#include <string>
#include <vector>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)

class IStudentDao : public IDao<Student, std::string> {
public:
    virtual ~IStudentDao() override = default;

    virtual std::expected<std::vector<Student>, Error> findByFacultyId(const std::string& facultyId) const = 0;
    virtual std::expected<Student, Error> findByEmail(const std::string& email) const = 0;
    virtual std::expected<std::vector<Student>, Error> findByStatus(LoginStatus status) const = 0;
    virtual std::expected<bool, Error> updateStatus(const std::string& studentId, LoginStatus newStatus) = 0;
};

#endif // ISTUDENTDAO_H