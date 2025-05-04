#ifndef ICOURSEREPOSITORY_H
#define ICOURSEREPOSITORY_H

#include <string>
#include <vector>
#include <optional>
#include "../../entities/Course.h"

class ICourseRepository {
public:
    virtual ~ICourseRepository() = default;

    // Course operations
    virtual std::optional<Course> findById(const std::string& courseId) const = 0;
    virtual std::vector<Course> findAll() const = 0;
    virtual std::vector<Course> findByFacultyId(const std::string& facultyId) const = 0; // Courses offered by a Faculty
    virtual bool add(const Course& course) = 0;
    virtual bool update(const Course& course) = 0;
    virtual bool remove(const std::string& courseId) = 0;
    virtual bool exists(const std::string& courseId) const = 0;

    // Enrollment related (Consider moving to a dedicated IEnrollmentRepository if complex)
    // These might modify a separate enrollment table/file
    virtual bool enrollStudent(const std::string& studentId, const std::string& courseId) = 0;
    virtual bool unenrollStudent(const std::string& studentId, const std::string& courseId) = 0;
    virtual std::vector<std::string> findStudentsByCourse(const std::string& courseId) const = 0; // Get student IDs enrolled
    virtual std::vector<std::string> findCoursesByStudent(const std::string& studentId) const = 0; // Get course IDs for student
    virtual bool isStudentEnrolled(const std::string& studentId, const std::string& courseId) const = 0;

};

#endif // ICOURSEREPOSITORY_H