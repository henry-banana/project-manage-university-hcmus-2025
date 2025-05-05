#include "CourseService.h"
#include "../repo/interface/ICourseRepository.h"
#include "../repo/interface/IFacultyRepository.h" // To validate Faculty ID
// Include Enrollment/Result repos if needed for cascade checks on remove
#include "../../utils/Logger.h"
#include <memory>

CourseService::CourseService(std::shared_ptr<ICourseRepository> courseRepo,
                std::shared_ptr<IFacultyRepository> facultyRepo /* Add others if needed */)
    : _courseRepo(courseRepo), _facultyRepo(facultyRepo) {
    if (!_courseRepo || !_facultyRepo) {
        throw std::invalid_argument("Repositories cannot be null in CourseService.");
    }
}

std::optional<Course> CourseService::getCourseById(const std::string& courseId) const {
    return _courseRepo->findById(courseId);
}

std::vector<Course> CourseService::getAllCourses() const {
    return _courseRepo->findAll();
}

std::vector<Course> CourseService::getCoursesByFaculty(const std::string& facultyId) const {
    if (!_facultyRepo->exists(facultyId)) {
        LOG_WARN("Attempted to get courses for non-existent faculty ID: " + facultyId);
        return {};
    }
    return _courseRepo->findByFacultyId(facultyId);
}

bool CourseService::addCourse(const Course& course) {
    if (_courseRepo->exists(course.id())) {
        LOG_WARN("Add course failed: ID already exists: " + course.id());
        return false;
    }
    if (!_facultyRepo->exists(course.facultyId())) {
            LOG_WARN("Add course failed: Invalid faculty ID " + course.facultyId() + " for course " + course.id());
            return false;
    }
    if (course.credits() <= 0) {
        LOG_WARN("Add course failed: Credits must be positive for course " + course.id());
        return false;
    }
    // Add other validations

    return _courseRepo->add(course);
}

bool CourseService::updateCourse(const Course& course) {
    if (!_courseRepo->exists(course.id())) {
            LOG_WARN("Update course failed: ID not found: " + course.id());
            return false;
    }
    if (!_facultyRepo->exists(course.facultyId())) {
            LOG_WARN("Update course failed: Invalid faculty ID " + course.facultyId() + " for course " + course.id());
            return false;
    }
        if (course.credits() <= 0) {
            LOG_WARN("Update course failed: Credits must be positive for course " + course.id());
            return false;
        }
        // Add other validations

    return _courseRepo->update(course);
}

bool CourseService::removeCourse(const std::string& courseId) {
    if (!_courseRepo->exists(courseId)) {
        LOG_WARN("Remove course failed: ID not found: " + courseId);
        return false;
    }

    // **Referential Integrity Check**
    // Check if any students are enrolled or have results for this course.
    // Policy: Disallow removal if referenced.
    // Note: This uses methods assumed to be on ICourseRepository or a dedicated IEnrollmentRepo/IResultRepo

    if (!_courseRepo->findStudentsByCourse(courseId).empty()) { // Check enrollments
            LOG_WARN("Remove course failed: Course " + courseId + " has enrolled students.");
            return false;
    }
    // if (_resultRepo && !_resultRepo->findCourseResultsByCourse(courseId).empty()) { // Check results
    //     LOG_WARN("Remove course failed: Course " + courseId + " has existing results.");
    //     return false;
    // }


    // If checks pass, proceed with removal
    if (_courseRepo->remove(courseId)) {
            LOG_INFO("Course removed successfully: " + courseId);
            return true;
    } else {
            LOG_ERROR("Failed to remove course from repository: " + courseId);
            return false;
    }
}