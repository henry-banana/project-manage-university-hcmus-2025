#include "TeacherService.h"
#include "../repo/interface/ITeacherRepository.h"
#include "../repo/interface/IFacultyRepository.h"
#include "AuthService.h"
// Include ISalaryRepository if creating initial salary record
#include "../../utils/Logger.h"
#include <memory>

TeacherService::TeacherService(std::shared_ptr<ITeacherRepository> teacherRepo,
                std::shared_ptr<IFacultyRepository> facultyRepo,
                std::shared_ptr<IAuthService> authService /*, Add salaryRepo if needed */ )
    : _teacherRepo(teacherRepo), _facultyRepo(facultyRepo), _authService(authService) /*, _salaryRepo(salaryRepo) */
{
    if (!_teacherRepo || !_facultyRepo || !_authService /* || !_salaryRepo */ ) {
        throw std::invalid_argument("Repositories and AuthService cannot be null in TeacherService.");
    }
}

std::optional<Teacher> TeacherService::getTeacherDetails(const std::string& teacherId) const {
    return _teacherRepo->findById(teacherId);
}

std::vector<Teacher> TeacherService::getAllTeachers() const {
    return _teacherRepo->findAll();
}

std::vector<Teacher> TeacherService::getTeachersByFaculty(const std::string& facultyId) const {
    if (!_facultyRepo->exists(facultyId)) {
        LOG_WARN("Attempted to get teachers for non-existent faculty ID: " + facultyId);
        return {};
    }
    return _teacherRepo->findByFacultyId(facultyId);
}

std::vector<Teacher> TeacherService::getTeachersByDesignation(const std::string& designation) const {
    return _teacherRepo->findByDesignation(designation);
}

std::vector<Teacher> TeacherService::getHODs() const {
        // Assuming "HOD" is the designation string
    return _teacherRepo->findByDesignation("HOD");
}


bool TeacherService::updateTeacherDetails(const Teacher& teacher) {
    if (!_teacherRepo->exists(teacher.id())) {
            LOG_WARN("Attempted to update non-existent teacher: " + teacher.id());
            return false;
    }
    if (!_facultyRepo->exists(teacher.facultyId())) {
            LOG_WARN("Update failed for teacher " + teacher.id() + ": Invalid faculty ID " + teacher.facultyId());
            return false;
    }
    // Add other validations
    return _teacherRepo->update(teacher);
}

bool TeacherService::addTeacher(const Teacher& teacher, const std::string& initialPassword) {
        if (_teacherRepo->exists(teacher.id())) {
            LOG_WARN("Attempted to add teacher with existing ID: " + teacher.id());
            return false;
        }
        if (!_facultyRepo->exists(teacher.facultyId())) {
            LOG_WARN("Add teacher failed: Invalid faculty ID " + teacher.facultyId() + " for teacher " + teacher.id());
            return false;
        }
        // Add other validations (e.g., experience non-negative)

    if (!_teacherRepo->add(teacher)) {
        LOG_ERROR("Failed to add teacher details to repository for ID: " + teacher.id());
        return false;
    }

    if (!_authService->registerUser(teacher.id(), initialPassword, UserRole::TEACHER)) {
            LOG_ERROR("Failed to register login credentials for teacher ID: " + teacher.id() + ". Rolling back teacher addition.");
            if (!_teacherRepo->remove(teacher.id())) {
                LOG_CRITICAL("CRITICAL: Failed to rollback teacher addition after login registration failure for ID: " + teacher.id());
            }
            return false;
    }

        // Optional: Create initial Salary Record
        // Example (requires _salaryRepo injected)
        // SalaryRecord initialSalary(teacher.id(), DEFAULT_TEACHER_SALARY);
        // _salaryRepo->add(initialSalary);

    LOG_INFO("Teacher added successfully: " + teacher.id());
    return true;
}

bool TeacherService::removeTeacher(const std::string& teacherId) {
    if (!_teacherRepo->exists(teacherId)) {
            LOG_WARN("Attempted to remove non-existent teacher: " + teacherId);
            return false;
    }

    // Step 1: Remove login credentials
    if (!_authService->removeUserCredentials(teacherId)) { // Assuming method exists
        LOG_ERROR("Failed to remove login credentials for teacher ID: " + teacherId + ". Continuing removal.");
    }

        // Step 2: Remove related data (e.g., SalaryRecord)
        // Example (requires _salaryRepo injected):
        // if (!_salaryRepo->remove(teacherId)) { LOG_WARN("Could not remove salary record for teacher: " + teacherId); }
        // Consider implications if teacher is assigned to courses - might need unassign logic.

    // Step 3: Remove teacher details
    if (_teacherRepo->remove(teacherId)) {
        LOG_INFO("Teacher removed successfully: " + teacherId);
        return true;
    } else {
        LOG_ERROR("Failed to remove teacher details from repository for ID: " + teacherId);
        return false;
    }
}