#include "StudentService.h"
#include "../repo/interface/IStudentRepository.h"
#include "../repo/interface/ILoginRepository.h" // To validate Faculty ID
#include "../repo/interface/IFacultyRepository.h" // To validate Faculty ID
#include "AuthService.h" // To register/remove user credentials
// Include other repos if needed for cascade deletes (Fee, Result, Enrollment)
#include "../../utils/Logger.h"
#include <memory>

StudentService::StudentService(std::shared_ptr<IStudentRepository> studentRepo,
                std::shared_ptr<IFacultyRepository> facultyRepo,
                std::shared_ptr<IAuthService> authService)
    : _studentRepo(studentRepo), _facultyRepo(facultyRepo), _authService(authService)
{
    if (!_studentRepo || !_facultyRepo || !_authService) {
        throw std::invalid_argument("Repositories and AuthService cannot be null in StudentService.");
    }
}

std::optional<Student> StudentService::getStudentDetails(const std::string& studentId) const {
    return _studentRepo->findById(studentId);
}

std::vector<Student> StudentService::getAllStudents() const {
    return _studentRepo->findAll();
}

std::vector<Student> StudentService::getStudentsByFaculty(const std::string& facultyId) const {
        // Optional: Check if facultyId exists first using _facultyRepo->exists(facultyId)
        if (!_facultyRepo->exists(facultyId)) {
            LOG_WARN("Attempted to get students for non-existent faculty ID: " + facultyId);
            return {}; // Return empty vector
        }
    return _studentRepo->findByFacultyId(facultyId);
}

bool StudentService::updateStudentDetails(const Student& student) {
        // Validation: Check if student exists
    if (!_studentRepo->exists(student.id())) {
        LOG_WARN("Attempted to update non-existent student: " + student.id());
        return false;
    }
    // Validation: Check if the assigned faculty ID exists
    if (!_facultyRepo->exists(student.facultyId())) {
            LOG_WARN("Update failed for student " + student.id() + ": Invalid faculty ID " + student.facultyId());
            return false;
    }
    // Add other validations (email format, phone format, etc.) if needed
    // Consider validating citizen ID uniqueness if required

    return _studentRepo->update(student);
}

bool StudentService::addStudent(const Student& student, const std::string& initialPassword) {
        // Validation: Check for existing ID
        if (_studentRepo->exists(student.id())) {
            LOG_WARN("Attempted to add student with existing ID: " + student.id());
            return false;
        }
        // Validation: Check if faculty ID is valid
        if (!_facultyRepo->exists(student.facultyId())) {
            LOG_WARN("Add student failed: Invalid faculty ID " + student.facultyId() + " for student " + student.id());
            return false;
        }
        // Add other validations

        // Step 1: Add student details to student repository
        if (!_studentRepo->add(student)) {
            LOG_ERROR("Failed to add student details to repository for ID: " + student.id());
            return false; // Abort if student data cannot be saved
        }

        // Step 2: Register login credentials via AuthService
        // The role should be STUDENT
        if (!_authService->registerUser(student.id(), initialPassword, UserRole::STUDENT)) {
            LOG_ERROR("Failed to register login credentials for student ID: " + student.id() + ". Rolling back student addition.");
            // Attempt to rollback: Remove the student added in step 1
            if (!_studentRepo->remove(student.id())) {
                LOG_CRITICAL("CRITICAL: Failed to rollback student addition after login registration failure for ID: " + student.id());
                // This indicates an inconsistent state! Needs monitoring/manual intervention.
            }
            return false; // Registration failed
        }

        // Step 3: Optionally create initial FeeRecord, Result structure etc.
        // Example: create initial fee record (needs IFeeRepository injected)
        // FeeRecord initialFee(student.id(), DEFAULT_TOTAL_FEE, 0);
        // _feeRepo->add(initialFee);


        LOG_INFO("Student added successfully: " + student.id());
        return true;
}

bool StudentService::removeStudent(const std::string& studentId) {
        // Validation: Check if student exists
        if (!_studentRepo->exists(studentId)) {
            LOG_WARN("Attempted to remove non-existent student: " + studentId);
            return false;
        }

        // Step 1: Remove login credentials (handle potential failure)
        // Implement cascade removal in LoginRepo or handle failure here
        // For now, assume LoginRepo handles it or log error if it fails but continue
        if (!_authService->removeUserCredentials(studentId)) { // Assuming AuthService has this method or calls repo
            LOG_ERROR("Failed to remove login credentials for student ID: " + studentId + ". Continuing removal process.");
            // Decide policy: Stop? Or just log and continue removing student data? Continuing might be required.
        }

        // Step 2: Remove related data (Fees, Results, Enrollments) - Requires respective repositories
        // Example (needs IFeeRepository etc injected):
        // if (!_feeRepo->remove(studentId)) { LOG_WARN("Could not remove fee record for student: " + studentId); }
        // if (!_resultRepo->removeAllResultsForStudent(studentId)) { LOG_WARN("Could not remove results for student: " + studentId); }
        // if (!_enrollmentRepo->unenrollStudentFromAll(studentId)) { LOG_WARN("Could not remove enrollments for student: " + studentId); }


        // Step 3: Remove student details from student repository
        if (_studentRepo->remove(studentId)) {
            LOG_INFO("Student removed successfully: " + studentId);
            return true;
        } else {
            LOG_ERROR("Failed to remove student details from repository for ID: " + studentId);
            // This might indicate an inconsistent state if credentials/related data were removed.
            return false;
        }
}