#ifndef ITEACHERDAO_H
#define ITEACHERDAO_H

#include "IDao.h"
#include "../../entities/Teacher.h"
#include <string>
#include <vector>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)

class ITeacherDao : public IDao<Teacher, std::string> {
public:
    virtual ~ITeacherDao() override = default;
    virtual std::expected<std::vector<Teacher>, Error> findByFacultyId(const std::string& facultyId) const = 0;
    virtual std::expected<std::vector<Teacher>, Error> findByDesignation(const std::string& designation) const = 0;
    virtual std::expected<Teacher, Error> findByEmail(const std::string& email) const = 0;
};
#endif // ITEACHERDAO_H