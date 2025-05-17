#ifndef ICOURSEDAO_H
#define ICOURSEDAO_H

#include "IDao.h"
#include "../../entities/Course.h"
#include <string>
#include <vector>

class ICourseDao : public IDao<Course, std::string> {
public:
    virtual ~ICourseDao() override = default;
    virtual OperationResult<std::vector<Course>> findByFacultyId(const std::string& facultyId) const = 0;
};

#endif // ICOURSEDAO_H