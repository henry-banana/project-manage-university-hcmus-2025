#ifndef STUDENTSERVICE_H
#define STUDENTSERVICE_H

#include <string>
#include <vector>
#include <optional>
#include <memory>
#include "../entities/Student.h"
#include "../entities/Course.h" 
#include "../entities/Result.h"
#include "../repo/interface/IStudentRepository.h"
#include "../repo/interface/IFacultyRepository.h"
#include "../services/AuthService.h" 

class IStudentService {
public:
    virtual ~IStudentService() = default;

    // Get student details
    virtual std::optional<Student> getStudentDetails(const std::string& studentId) const = 0;

    // Get all students or filter by faculty
    virtual std::vector<Student> getAllStudents() const = 0;
    virtual std::vector<Student> getStudentsByFaculty(const std::string& facultyId) const = 0;

    // Update student information (requires authorization checks in UI/Controller)
    virtual bool updateStudentDetails(const Student& student) = 0;

    // Add a new student (requires authorization)
    // Note: Also needs to coordinate with AuthService to register login credentials
    virtual bool addStudent(const Student& student, const std::string& initialPassword) = 0;

    // Remove a student (requires authorization)
    // Note: Also needs to coordinate with AuthService to remove login credentials
    // And potentially remove related Fee/Result/Enrollment records
    virtual bool removeStudent(const std::string& studentId) = 0;

    // Student-specific actions (could be in other services too)
    // Moved enrollment/results/fees to dedicated services for better SRP

};

class StudentService : public IStudentService {
private:
    std::shared_ptr<IStudentRepository> _studentRepo;
    std::shared_ptr<IFacultyRepository> _facultyRepo;
    std::shared_ptr<IAuthService> _authService; // Needed for add/remove

public:
    StudentService(std::shared_ptr<IStudentRepository> studentRepo,
                    std::shared_ptr<IFacultyRepository> facultyRepo,
                    std::shared_ptr<IAuthService> authService);

    std::optional<Student> getStudentDetails(const std::string& studentId) const override;

    std::vector<Student> getAllStudents() const override;

    std::vector<Student> getStudentsByFaculty(const std::string& facultyId) const override;

    bool updateStudentDetails(const Student& student) override;

    bool addStudent(const Student& student, const std::string& initialPassword) override;

    bool removeStudent(const std::string& studentId) override;
};

#endif // STUDENTSERVICE_H