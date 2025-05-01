#ifndef IFACULTYREPOSITORY_H
#define IFACULTYREPOSITORY_H

#include <optional>
#include <vector>
#include <string>
#include "../../entities/Faculty.h"

// Interface cho Faculty Repository
class IFacultyRepository {
private:

protected:

public:
    virtual ~IFacultyRepository() = default;

    // CRUD operations
    virtual std::optional<Faculty> findById(const std::string& facultyId) = 0;
    virtual std::vector<Faculty> findAll() = 0;
    virtual std::vector<Faculty> findByDepartment(const std::string& departmentId) = 0;
    virtual std::vector<Faculty> findHods() = 0; // Find all Faculty with HOD designation
    virtual bool add(const Faculty& faculty) = 0;
    virtual bool update(const Faculty& faculty) = 0;
    virtual bool remove(const std::string& facultyId) = 0;
};

#endif // IFACULTYREPOSITORY_H