#ifndef ISALARYREPOSITORY_H
#define ISALARYREPOSITORY_H

#include <optional>
#include <string>
#include "../../entities/SalaryRecord.h"

// Interface cho Salary Repository
class ISalaryRepository {
private:

protected:

public:
    virtual ~ISalaryRepository() = default;

    // CRUD operations
    virtual std::optional<SalaryRecord> findByFacultyId(const std::string& facultyId) = 0;
    virtual bool add(const SalaryRecord& salary) = 0;
    virtual bool update(const SalaryRecord& salary) = 0;
    virtual bool remove(const std::string& facultyId) = 0;
};

#endif // ISALARYREPOSITORY_H