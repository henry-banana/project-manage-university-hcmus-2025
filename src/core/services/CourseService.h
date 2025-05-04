#ifndef COURSE_SERVICE_H
#define COURSE_SERVICE_H

#include <string>
#include <vector>
#include <optional>
#include <memory>
#include "../entities/Course.h"
#include "../repo/interface/ICourseRepository.h"
#include "../repo/interface/IFacultyRepository.h"

class ICourseService {
public:
    virtual ~ICourseService() = default;

    virtual std::optional<Course> getCourseById(const std::string& courseId) const = 0;
    virtual std::vector<Course> getAllCourses() const = 0;
    virtual std::vector<Course> getCoursesByFaculty(const std::string& facultyId) const = 0;

    // Admin operations
    virtual bool addCourse(const Course& course) = 0;
    virtual bool updateCourse(const Course& course) = 0;
    virtual bool removeCourse(const std::string& courseId) = 0; // Consider impact on enrollments/results

    // Maybe assign teacher to course? Or keep in TeacherService/EnrollmentService?
    // virtual bool assignTeacherToCourse(const std::string& teacherId, const std::string& courseId) = 0;
};

class CourseService : public ICourseService {
private:
    std::shared_ptr<ICourseRepository> _courseRepo;
    std::shared_ptr<IFacultyRepository> _facultyRepo;
    // std::shared_ptr<IEnrollmentRepository> _enrollmentRepo; // If separate repo exists
    // std::shared_ptr<IResultRepository> _resultRepo;

public:
    CourseService(std::shared_ptr<ICourseRepository> courseRepo,
                    std::shared_ptr<IFacultyRepository> facultyRepo /* Add others if needed */);

    std::optional<Course> getCourseById(const std::string& courseId) const override;

    std::vector<Course> getAllCourses() const override;

    std::vector<Course> getCoursesByFaculty(const std::string& facultyId) const override;

    bool addCourse(const Course& course) override;

    bool updateCourse(const Course& course) override;

    bool removeCourse(const std::string& courseId) override;
};

#endif // COURSE_SERVICE_H