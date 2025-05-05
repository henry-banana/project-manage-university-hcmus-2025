#ifndef ISALARYREPOSITORY_H
#define ISALARYREPOSITORY_H

#include <optional>
#include <string>
#include <vector>
#include "../../entities/SalaryRecord.h"

// Interface cho Salary Repository
class ISalaryRepository {
private:

protected:

public:
    virtual ~ISalaryRepository() = default;

    // CRUD operations
    virtual std::optional<SalaryRecord> findByTeacherId(const std::string& teacherId) const = 0;
    virtual std::vector<SalaryRecord> findAll() const = 0; // For reports/admin
    virtual bool add(const SalaryRecord& salaryRecord) = 0;
    virtual bool update(const SalaryRecord& salaryRecord) = 0;
    virtual bool remove(const std::string& teacherId) = 0;
};

#endif // ISALARYREPOSITORY_H