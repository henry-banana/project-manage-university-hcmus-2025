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
    virtual std::optional<Faculty> findByName(const std::string& name) const = 0;
    virtual std::vector<Faculty> findAll() = 0;
    virtual bool add(const Faculty& faculty) = 0;
    virtual bool update(const Faculty& faculty) = 0;
    virtual bool remove(const std::string& facultyId) = 0;
    virtual bool exists(const std::string& facultyId) const = 0;
};

#endif // IFACULTYREPOSITORY_H