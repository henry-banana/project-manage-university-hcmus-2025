#ifndef ADMIN_SERVICE_H
#define ADMIN_SERVICE_H

#include <memory>
#include <vector>
#include <string>
#include <optional>
#include "../entities/Student.h"
#include "../entities/Faculty.h"
#include "../entities/Department.h"
#include "../repo/interface/IStudentRepository.h"
#include "../repo/interface/IFacultyRepository.h"
#include "../repo/interface/IDepartmentRepository.h"

class IAdminService {
public:
    virtual ~IAdminService() = default;
    
    // Student related operations
    virtual std::optional<Student> getStudentDetails(const std::string& studentId) const = 0;
    virtual std::vector<Student> getStudentsByDepartment(const std::string& departmentId) const = 0;
    
    // Faculty related operations
    virtual std::optional<Faculty> getFacultyDetails(const std::string& facultyId) const = 0;
    virtual std::vector<Faculty> getFacultyByDepartment(const std::string& departmentId) const = 0;
    virtual std::vector<Faculty> getAllHODs() const = 0;
    
    // Department operations
    virtual std::vector<Department> getAllDepartments() const = 0;
};

class AdminService : public IAdminService {
private:
    std::shared_ptr<IStudentRepository> _studentRepo;
    std::shared_ptr<IFacultyRepository> _facultyRepo;
    std::shared_ptr<IDepartmentRepository> _departmentRepo;

public:
    AdminService(
        std::shared_ptr<IStudentRepository> studentRepo,
        std::shared_ptr<IFacultyRepository> facultyRepo,
        std::shared_ptr<IDepartmentRepository> departmentRepo
    );

    // Student related operations
    std::optional<Student> getStudentDetails(const std::string& studentId) const override;
    std::vector<Student> getStudentsByDepartment(const std::string& departmentId) const override;
    
    // Faculty related operations
    std::optional<Faculty> getFacultyDetails(const std::string& facultyId) const override;
    std::vector<Faculty> getFacultyByDepartment(const std::string& departmentId) const override;
    std::vector<Faculty> getAllHODs() const override;
    
    // Department operations
    std::vector<Department> getAllDepartments() const override;
};

#endif // ADMIN_SERVICE_H