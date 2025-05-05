#include "EnrollmentService.h"
#include "../repo/interface/ICourseRepository.h" // Contains enrollment methods for now
#include "../repo/interface/IStudentRepository.h"
#include "../../utils/Logger.h"
#include <memory>
#include <vector>
#include <algorithm> // For std::find

EnrollmentService::EnrollmentService(
    std::shared_ptr<IStudentRepository> studentRepo,
    std::shared_ptr<ICourseRepository> courseRepo
) : _studentRepo(studentRepo), _courseRepo(courseRepo) {
    if (!_studentRepo || !_courseRepo) {
        throw std::invalid_argument("Repositories cannot be null in EnrollmentService.");
    }
}

bool EnrollmentService::enrollStudentInCourse(const std::string& studentId, const std::string& courseId) {
    // Validation: Check if student and course exist
    if (!_studentRepo->exists(studentId)) {
        LOG_WARN("Enrollment failed: Student ID not found: " + studentId);
        return false;
    }
    if (!_courseRepo->exists(courseId)) {
        LOG_WARN("Enrollment failed: Course ID not found: " + courseId);
        return false;
    }
    // Validation: Check if already enrolled
    if (isStudentEnrolled(studentId, courseId)) {
        LOG_INFO("Enrollment skipped: Student " + studentId + " already enrolled in course " + courseId);
        return true; // Or false if considered an error? True seems better (idempotent)
    }

    // Add enrollment prerequisite checks here if needed
    // Add course capacity checks here if needed

    // Perform enrollment via repository
    if (_courseRepo->enrollStudent(studentId, courseId)) {
            LOG_INFO("Student " + studentId + " successfully enrolled in course " + courseId);
            return true;
    } else {
            LOG_ERROR("Failed to enroll student " + studentId + " in course " + courseId + " (repository error).");
            return false;
    }
}

bool EnrollmentService::dropCourse(const std::string& studentId, const std::string& courseId) {
        // Validation: Check if student and course exist (optional, repo might handle)
        if (!isStudentEnrolled(studentId, courseId)) {
            LOG_WARN("Drop course failed: Student " + studentId + " is not enrolled in course " + courseId);
            return false; // Cannot drop if not enrolled
        }

    // Perform unenrollment via repository
    if (_courseRepo->unenrollStudent(studentId, courseId)) {
        LOG_INFO("Student " + studentId + " successfully dropped course " + courseId);
        return true;
    } else {
        LOG_ERROR("Failed to drop course " + courseId + " for student " + studentId + " (repository error).");
        return false;
    }
}

bool EnrollmentService::isStudentEnrolled(const std::string& studentId, const std::string& courseId) {
    // This relies on the repository implementation
    return _courseRepo->isStudentEnrolled(studentId, courseId);
}

std::vector<Course> EnrollmentService::getEnrolledCourses(const std::string& studentId) {
    if (!_studentRepo->exists(studentId)) return {}; // Return empty if student doesn't exist

    std::vector<std::string> courseIds = _courseRepo->findCoursesByStudent(studentId);
    std::vector<Course> courses;
    courses.reserve(courseIds.size()); // Optimize allocation

    for (const auto& id : courseIds) {
        auto courseOpt = _courseRepo->findById(id);
        if (courseOpt) {
            courses.push_back(courseOpt.value());
        } else {
                LOG_WARN("Student " + studentId + " is enrolled in non-existent course ID: " + id);
        }
    }
    return courses;
}

std::vector<Student> EnrollmentService::getEnrolledStudents(const std::string& courseId) {
    if (!_courseRepo->exists(courseId)) return {}; // Return empty if course doesn't exist

    std::vector<std::string> studentIds = _courseRepo->findStudentsByCourse(courseId);
    std::vector<Student> students;
    students.reserve(studentIds.size());

    for (const auto& id : studentIds) {
        auto studentOpt = _studentRepo->findById(id);
        if (studentOpt) {
            students.push_back(studentOpt.value());
        } else {
                LOG_WARN("Course " + courseId + " has enrollment for non-existent student ID: " + id);
        }
    }
    return students;
}