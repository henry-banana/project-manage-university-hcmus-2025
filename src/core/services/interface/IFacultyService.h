#ifndef IFACULTYSERVICE_H
#define IFACULTYSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/Faculty.h"

class IFacultyService {
public:
    virtual ~IFacultyService() = default;
    virtual std::expected<Faculty, Error> getFacultyById(const std::string& facultyId) const = 0;
    virtual std::expected<Faculty, Error> getFacultyByName(const std::string& name) const = 0;
    virtual std::expected<std::vector<Faculty>, Error> getAllFaculties() const = 0;
    virtual std::expected<Faculty, Error> addFaculty(const std::string& id, const std::string& name) = 0;
    virtual std::expected<bool, Error> updateFaculty(const std::string& facultyId, const std::string& newName) = 0;
    virtual std::expected<bool, Error> removeFaculty(const std::string& facultyId) = 0;
};

#endif // IFACULTYSERVICE_H