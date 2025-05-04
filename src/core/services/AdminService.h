#ifndef ADMIN_SERVICE_H
#define ADMIN_SERVICE_H

#include <memory>
#include <vector>
#include <string>
#include <optional>
#include "../entities/Student.h"
#include "../entities/Faculty.h"
#include "../repo/interface/IStudentRepository.h"
#include "../repo/interface/ITeacherRepository.h"
#include "../repo/interface/IFacultyRepository.h"

// Interface for administrative overview/reporting tasks
// Note: Actual *management* (add/update/delete) should likely use
// the specific services (StudentService, TeacherService, etc.) for consistency.
// This service focuses on retrieving aggregated/filtered views useful for an admin.
class IAdminService {
public:
    virtual ~IAdminService() = default;

    // Student Information Retrieval
    virtual std::optional<Student> getStudentDetails(const std::string& studentId) const = 0;
    virtual std::vector<Student> getStudentsByFaculty(const std::string& facultyId) const = 0;
    virtual std::vector<Student> getAllStudents() const = 0;

    // Teacher Information Retrieval
    virtual std::optional<Teacher> getTeacherDetails(const std::string& teacherId) const = 0;
    virtual std::vector<Teacher> getTeachersByFaculty(const std::string& facultyId) const = 0;
    virtual std::vector<Teacher> getAllTeachers() const = 0;
    virtual std::vector<Teacher> getAllHODs() const = 0; // Get Head of Departments

    // Faculty (Department/School) Information Retrieval
    virtual std::optional<Faculty> getFacultyDetails(const std::string& facultyId) const = 0;
    virtual std::vector<Faculty> getAllFaculties() const = 0;

    // Course Information Retrieval (Optional here, could be separate service)
    // virtual std::optional<Course> getCourseDetails(const std::string& courseId) const = 0;
    // virtual std::vector<Course> getCoursesByFaculty(const std::string& facultyId) const = 0;
    // virtual std::vector<Course> getAllCourses() const = 0;
};

// Implementation focused on providing views/reports for Admin.
// Does not handle add/update/delete - delegates those to specific services if needed.
class AdminService : public IAdminService {
private:
    std::shared_ptr<IStudentRepository> _studentRepo;
    std::shared_ptr<ITeacherRepository> _teacherRepo;
    std::shared_ptr<IFacultyRepository> _facultyRepo;
    // std::shared_ptr<ICourseRepository> _courseRepo; // If needed

public:
    AdminService(std::shared_ptr<IStudentRepository> studentRepo,
                    std::shared_ptr<ITeacherRepository> teacherRepo,
                    std::shared_ptr<IFacultyRepository> facultyRepo /*, courseRepo */ );

    // --- Student Info ---
    std::optional<Student> getStudentDetails(const std::string& studentId) const override;

    std::vector<Student> getStudentsByFaculty(const std::string& facultyId) const override;

    std::vector<Student> getAllStudents() const override;

    // --- Teacher Info ---
    std::optional<Teacher> getTeacherDetails(const std::string& teacherId) const override;

    std::vector<Teacher> getTeachersByFaculty(const std::string& facultyId) const override;

    std::vector<Teacher> getAllTeachers() const override;

    std::vector<Teacher> getAllHODs() const override;

    // --- Faculty Info ---
    std::optional<Faculty> getFacultyDetails(const std::string& facultyId) const override;

    std::vector<Faculty> getAllFaculties() const override;

    // --- Course Info (Example if added) ---
    /*
    std::optional<Course> getCourseDetails(const std::string& courseId) const override {
        return _courseRepo->findById(courseId);
    }
    std::vector<Course> getCoursesByFaculty(const std::string& facultyId) const override {
            if (!_facultyRepo->exists(facultyId)) return {};
        return _courseRepo->findByFacultyId(facultyId);
    }
    std::vector<Course> getAllCourses() const override {
        return _courseRepo->findAll();
    }
    */
};

#endif // ADMIN_SERVICE_H