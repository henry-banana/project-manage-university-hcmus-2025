#ifndef ITEACHERREPOSITORY_H
#define ITEACHERREPOSITORY_H

#include <optional>
#include <vector>
#include <string>
#include "../../entities/Teacher.h"

// Interface for Teacher Repository
class ITeacherRepository {
public:
    virtual ~ITeacherRepository() = default;

    // CRUD operations for Teachers
    virtual std::optional<Teacher> findById(const std::string& teacherId) const = 0;
    virtual std::vector<Teacher> findAll() const = 0;
    // Find teachers belonging to a specific Faculty (Department/School)
    virtual std::vector<Teacher> findByFacultyId(const std::string& facultyId) const = 0;
    // Find teachers with a specific designation (e.g., "HOD")
    virtual std::vector<Teacher> findByDesignation(const std::string& designation) const = 0;
    virtual bool add(const Teacher& teacher) = 0;
    virtual bool update(const Teacher& teacher) = 0;
    virtual bool remove(const std::string& teacherId) = 0;
    virtual bool exists(const std::string& teacherId) const = 0;
};

#endif // ITEACHERREPOSITORY_H