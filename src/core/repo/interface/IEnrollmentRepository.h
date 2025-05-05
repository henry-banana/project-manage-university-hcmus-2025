#ifndef IENROLLMENTREPOSITORY_H
#define IENROLLMENTREPOSITORY_H

#include <string>
#include <vector>
#include <utility> // For std::pair

// Interface specifically for managing enrollment relationships
class IEnrollmentRepository {
public:
    // Represents a single enrollment record
    using EnrollmentRecord = std::pair<std::string, std::string>; // {studentId, courseId}

    virtual ~IEnrollmentRepository() = default;

    // Add an enrollment record
    virtual bool addEnrollment(const std::string& studentId, const std::string& courseId) = 0;

    // Remove an enrollment record
    virtual bool removeEnrollment(const std::string& studentId, const std::string& courseId) = 0;

    // Remove all enrollments for a specific student
    virtual bool removeEnrollmentsByStudent(const std::string& studentId) = 0;

    // Remove all enrollments for a specific course
    virtual bool removeEnrollmentsByCourse(const std::string& courseId) = 0;

    // Check if a specific enrollment exists
    virtual bool isEnrolled(const std::string& studentId, const std::string& courseId) const = 0;

    // Find all course IDs a student is enrolled in
    virtual std::vector<std::string> findCoursesByStudent(const std::string& studentId) const = 0;

    // Find all student IDs enrolled in a course
    virtual std::vector<std::string> findStudentsByCourse(const std::string& courseId) const = 0;

    // Get all enrollment records (for internal use or reporting)
    virtual std::vector<EnrollmentRecord> findAllEnrollments() const = 0;
};

#endif // IENROLLMENTREPOSITORY_H