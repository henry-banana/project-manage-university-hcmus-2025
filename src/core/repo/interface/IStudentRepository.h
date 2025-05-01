#ifndef ISTUDENTREPOSITORY_H
#define ISTUDENTREPOSITORY_H

#include <optional>
#include <vector>
#include <string>
#include "../../entities/Student.h"

// Interface cho Student Repository
class IStudentRepository {
private:

protected:

public:
    virtual ~IStudentRepository() = default;

    // CRUD operations
    virtual std::optional<Student> findById(const std::string& studentId) = 0;
    virtual std::vector<Student> findAll() = 0;
    virtual std::vector<Student> findByDepartment(const std::string& departmentId) = 0;
    virtual bool add(const Student& student) = 0;
    virtual bool update(const Student& student) = 0;
    virtual bool remove(const std::string& studentId) = 0;
};

#endif // ISTUDENTREPOSITORY_H