#ifndef IFACULTYDAO_H
#define IFACULTYDAO_H

#include "IDao.h"
#include "../../entities/Faculty.h"
#include <string>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)

class IFacultyDao : public IDao<Faculty, std::string> {
public:
    virtual ~IFacultyDao() override = default;
    virtual std::expected<Faculty, Error> findByName(const std::string& name) const = 0;
};

#endif // IFACULTYDAO_H