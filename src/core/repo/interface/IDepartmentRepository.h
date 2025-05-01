#ifndef IDEPARTMENTREPOSITORY_H
#define IDEPARTMENTREPOSITORY_H

#include <optional>
#include <vector>
#include <string>
#include "../../entities/Department.h"

// Interface cho Department Repository
class IDepartmentRepository {
private:

protected:

public:
    virtual ~IDepartmentRepository() = default;

    // CRUD operations
    virtual std::optional<Department> findById(const std::string& departmentId) = 0;
    virtual std::vector<Department> findAll() = 0;
    virtual bool add(const Department& department) = 0;
    virtual bool update(const Department& department) = 0; // Mainly for HOD assignment
    virtual bool remove(const std::string& departmentId) = 0;
};

#endif // IDEPARTMENTREPOSITORY_H