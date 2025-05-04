#ifndef TEACHERSERVICE_H
#define TEACHERSERVICE_H

#include <string>
#include <vector>
#include <optional>
#include <memory>
#include "../entities/Teacher.h"
#include "../repo/interface/ITeacherRepository.h"
#include "../repo/interface/IFacultyRepository.h"
#include "../services/AuthService.h"

// Interface for services related to Teachers
class ITeacherService {
public:
    virtual ~ITeacherService() = default;

    virtual std::optional<Teacher> getTeacherDetails(const std::string& teacherId) const = 0;
    virtual std::vector<Teacher> getAllTeachers() const = 0;
    virtual std::vector<Teacher> getTeachersByFaculty(const std::string& facultyId) const = 0;
    virtual std::vector<Teacher> getTeachersByDesignation(const std::string& designation) const = 0;
    virtual std::vector<Teacher> getHODs() const = 0; // Convenience method

    virtual bool updateTeacherDetails(const Teacher& teacher) = 0;
    virtual bool addTeacher(const Teacher& teacher, const std::string& initialPassword) = 0;
    virtual bool removeTeacher(const std::string& teacherId) = 0;

    // Could add methods like assignCourseToTeacher, etc. if needed
};

class TeacherService : public ITeacherService {
private:
    std::shared_ptr<ITeacherRepository> _teacherRepo;
    std::shared_ptr<IFacultyRepository> _facultyRepo;
    std::shared_ptr<IAuthService> _authService;
    // std::shared_ptr<ISalaryRepository> _salaryRepo; // If needed

public:
    TeacherService(std::shared_ptr<ITeacherRepository> teacherRepo,
                    std::shared_ptr<IFacultyRepository> facultyRepo,
                    std::shared_ptr<IAuthService> authService);

    std::optional<Teacher> getTeacherDetails(const std::string& teacherId) const override;
    
    std::vector<Teacher> getAllTeachers() const override;

    std::vector<Teacher> getTeachersByFaculty(const std::string& facultyId) const override;

    std::vector<Teacher> getTeachersByDesignation(const std::string& designation) const override;

    std::vector<Teacher> getHODs() const override;

    bool updateTeacherDetails(const Teacher& teacher) override;

    bool addTeacher(const Teacher& teacher, const std::string& initialPassword) override;

    bool removeTeacher(const std::string& teacherId) override;

};

#endif // TEACHERSERVICE_H