#ifndef ITEACHERDAO_H
#define ITEACHERDAO_H

#include "IDao.h"
#include "../../entities/Teacher.h"
#include <string>
#include <vector>

class ITeacherDao : public IDao<Teacher, std::string> {
public:
    virtual ~ITeacherDao() override = default;
    virtual OperationResult<std::vector<Teacher>> findByFacultyId(const std::string& facultyId) const = 0;
    virtual OperationResult<std::vector<Teacher>> findByDesignation(const std::string& designation) const = 0;
    virtual OperationResult<Teacher> findByEmail(const std::string& email) const = 0; // (➕)
};

#endif // ITEACHERDAO_H