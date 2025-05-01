#ifndef FACULTY_SERVICE_H
#define FACULTY_SERVICE_H

#include <memory>
#include <vector>
#include <string>
#include <optional>
#include "../entities/Faculty.h"
#include "../entities/Department.h"
#include "../repo/interface/IFacultyRepository.h"
#include "../repo/interface/IDepartmentRepository.h"

class IFacultyService {
public:
    virtual ~IFacultyService() = default;
    
    // Faculty operations
    virtual std::optional<Faculty> getFacultyById(const std::string& facultyId) const = 0;
    virtual std::vector<Faculty> getAllFaculty() const = 0;
    virtual std::vector<Faculty> getFacultyByDepartment(const std::string& departmentId) const = 0;
    virtual std::vector<Faculty> getFacultyByDesignation(const std::string& designation) const = 0;
    virtual bool addFaculty(const Faculty& faculty) = 0;
    virtual bool updateFaculty(const Faculty& faculty) = 0;
    virtual bool removeFaculty(const std::string& facultyId) = 0;
};

class FacultyService : public IFacultyService {
private:
    std::shared_ptr<IFacultyRepository> _facultyRepo;
    std::shared_ptr<IDepartmentRepository> _departmentRepo;
    
public:
    FacultyService(
        std::shared_ptr<IFacultyRepository> facultyRepo,
        std::shared_ptr<IDepartmentRepository> departmentRepo
    );
    
    // Faculty operations
    std::optional<Faculty> getFacultyById(const std::string& facultyId) const override;
    std::vector<Faculty> getAllFaculty() const override;
    std::vector<Faculty> getFacultyByDepartment(const std::string& departmentId) const override;
    std::vector<Faculty> getFacultyByDesignation(const std::string& designation) const override;
    bool addFaculty(const Faculty& faculty) override;
    bool updateFaculty(const Faculty& faculty) override;
    bool removeFaculty(const std::string& facultyId) override;
};

#endif // FACULTY_SERVICE_H