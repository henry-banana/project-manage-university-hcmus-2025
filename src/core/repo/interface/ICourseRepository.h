#ifndef ICOURSE_REPOSITORY_H
#define ICOURSE_REPOSITORY_H

#include <string>
#include <vector>
#include <optional>
#include "../../entities/Course.h"

class ICourseRepository {
public:
    virtual ~ICourseRepository() = default;

    // Course operations
    virtual std::vector<Course> getAllCourses() const = 0;
    virtual std::optional<Course> getCourseById(const std::string& courseId) const = 0;
    virtual std::vector<Course> getCoursesByDepartment(const std::string& departmentId) const = 0;
    virtual std::vector<Course> getEnrolledCourses(const std::string& studentId) const = 0;
    virtual bool enrollStudentInCourse(const std::string& studentId, const std::string& courseId) = 0;
    virtual bool unenrollStudentFromCourse(const std::string& studentId, const std::string& courseId) = 0;
    virtual std::vector<std::string> getEnrolledStudents(const std::string& courseId) const = 0;
};

#endif // ICOURSE_REPOSITORY_H