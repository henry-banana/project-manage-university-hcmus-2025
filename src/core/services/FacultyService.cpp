#include "FacultyService.h"
#include "../repo/interface/IFacultyRepository.h"
// Include other repo if neededfor validation/cascade checks (Student, Teacher, Course)
#include "../repo/interface/IStudentRepository.h"
#include "../repo/interface/ITeacherRepository.h"
#include "../repo/interface/ICourseRepository.h"
#include "../../utils/Logger.h"
#include <memory>

FacultyService::FacultyService(std::shared_ptr<IFacultyRepository> facultyRepo,
                std::shared_ptr<IStudentRepository> studentRepo, // Optional for checks
                std::shared_ptr<ITeacherRepository> teacherRepo, // Optional for checks
                std::shared_ptr<ICourseRepository> courseRepo)   // Optional for checks
    : _facultyRepo(facultyRepo), _studentRepo(studentRepo), _teacherRepo(teacherRepo), _courseRepo(courseRepo)
        {
    if (!_facultyRepo) {
        throw std::invalid_argument("FacultyRepository cannot be null in FacultyService.");
    }
        // Null checks for optional repos if provided
        if (!_studentRepo || !_teacherRepo || !_courseRepo) {
            LOG_WARN("FacultyService initialized without Student/Teacher/Course repos; removal checks will be limited.");
        }
}

std::optional<Faculty> FacultyService::getFacultyById(const std::string& facultyId) const {
    return _facultyRepo->findById(facultyId);
}

    std::optional<Faculty> FacultyService::getFacultyByName(const std::string& name) const {
    return _facultyRepo->findByName(name);
}

std::vector<Faculty> FacultyService::getAllFaculties() const {
    return _facultyRepo->findAll();
}

bool FacultyService::addFaculty(const Faculty& faculty) {
    // Validation: Check for existing ID or Name
    if (_facultyRepo->exists(faculty.id())) {
        LOG_WARN("Add faculty failed: ID already exists: " + faculty.id());
        return false;
    }
    if (_facultyRepo->findByName(faculty.name())) {
            LOG_WARN("Add faculty failed: Name already exists: " + faculty.name());
            return false;
    }
    // Add other validations (e.g., non-empty ID/Name)

    return _facultyRepo->add(faculty);
}

bool FacultyService::updateFaculty(const Faculty& faculty) {
        // Validation: Check if faculty exists by ID
    if (!_facultyRepo->exists(faculty.id())) {
        LOG_WARN("Update faculty failed: ID not found: " + faculty.id());
        return false;
    }
        // Validation: Check if the new name conflicts with another faculty
        auto existingByName = _facultyRepo->findByName(faculty.name());
        if (existingByName && existingByName.value().id() != faculty.id()) {
            LOG_WARN("Update faculty failed: Name '" + faculty.name() + "' conflicts with existing faculty ID: " + existingByName.value().id());
            return false;
        }

    return _facultyRepo->update(faculty);
}

bool FacultyService::removeFaculty(const std::string& facultyId) {
    // Validation: Check if faculty exists
    if (!_facultyRepo->exists(facultyId)) {
        LOG_WARN("Remove faculty failed: ID not found: " + facultyId);
        return false;
    }

    // **Referential Integrity Check (Crucial!)**
    // Before removing a faculty, check if any students, teachers, or courses still reference it.
    // This prevents dangling references. Policy could be:
    // 1. Disallow removal if referenced.
    // 2. Allow removal and set references to null/default (if schema allows).
    // 3. Allow removal and cascade delete referenced entities (dangerous!).
    // Let's implement Policy 1 (Disallow).

    if (_studentRepo && !_studentRepo->findByFacultyId(facultyId).empty()) {
        LOG_WARN("Remove faculty failed: Faculty " + facultyId + " is still referenced by students.");
        return false;
    }
    if (_teacherRepo && !_teacherRepo->findByFacultyId(facultyId).empty()) {
            LOG_WARN("Remove faculty failed: Faculty " + facultyId + " is still referenced by teachers.");
            return false;
    }
        if (_courseRepo && !_courseRepo->findByFacultyId(facultyId).empty()) {
            LOG_WARN("Remove faculty failed: Faculty " + facultyId + " is still referenced by courses.");
            return false;
        }


    // If checks pass, proceed with removal
    if (_facultyRepo->remove(facultyId)) {
        LOG_INFO("Faculty removed successfully: " + facultyId);
        return true;
    } else {
            LOG_ERROR("Failed to remove faculty from repository: " + facultyId);
            return false;
    }
}