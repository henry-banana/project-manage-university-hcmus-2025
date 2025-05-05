#include "AdminService.h"
#include "../repo/interface/IStudentRepository.h"
#include "../repo/interface/ITeacherRepository.h"
#include "../repo/interface/IFacultyRepository.h"
// Include ICourseRepository if course info is needed
#include "../../utils/Logger.h"
#include <memory>

AdminService::AdminService(std::shared_ptr<IStudentRepository> studentRepo,
                std::shared_ptr<ITeacherRepository> teacherRepo,
                std::shared_ptr<IFacultyRepository> facultyRepo /*, courseRepo */ )
    : _studentRepo(studentRepo), _teacherRepo(teacherRepo), _facultyRepo(facultyRepo) /*, _courseRepo(courseRepo) */ {
    if (!_studentRepo || !_teacherRepo || !_facultyRepo /* || !_courseRepo */ ) {
        throw std::invalid_argument("Repositories cannot be null in AdminService.");
    }
}

// --- Student Info ---
std::optional<Student> AdminService::getStudentDetails(const std::string& studentId) const {
    return _studentRepo->findById(studentId);
}

std::vector<Student> AdminService::getStudentsByFaculty(const std::string& facultyId) const {
        if (!_facultyRepo->exists(facultyId)) {
            LOG_WARN("[AdminService] Faculty ID not found: " + facultyId);
            return {};
        }
    return _studentRepo->findByFacultyId(facultyId);
}

std::vector<Student> AdminService::getAllStudents() const {
    return _studentRepo->findAll();
}


// --- Teacher Info ---
std::optional<Teacher> AdminService::getTeacherDetails(const std::string& teacherId) const {
    return _teacherRepo->findById(teacherId);
}

std::vector<Teacher> AdminService::getTeachersByFaculty(const std::string& facultyId) const {
    if (!_facultyRepo->exists(facultyId)) {
            LOG_WARN("[AdminService] Faculty ID not found: " + facultyId);
            return {};
    }
    return _teacherRepo->findByFacultyId(facultyId);
}

std::vector<Teacher> AdminService::getAllTeachers() const {
    return _teacherRepo->findAll();
}

std::vector<Teacher> AdminService::getAllHODs() const {
    // Assumes designation "HOD" is used consistently
    return _teacherRepo->findByDesignation("HOD");
}

// --- Faculty Info ---
std::optional<Faculty> AdminService::getFacultyDetails(const std::string& facultyId) const {
    return _facultyRepo->findById(facultyId);
}

std::vector<Faculty> AdminService::getAllFaculties() const {
    return _facultyRepo->findAll();
}

// --- Course Info (Example if added) ---
/*
std::optional<Course> getCourseDetails(const std::string& courseId) const {
    return _courseRepo->findById(courseId);
}
std::vector<Course> getCoursesByFaculty(const std::string& facultyId) const {
        if (!_facultyRepo->exists(facultyId)) return {};
    return _courseRepo->findByFacultyId(facultyId);
}
std::vector<Course> getAllCourses() const {
    return _courseRepo->findAll();
}
*/