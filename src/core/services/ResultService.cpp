#include "ResultService.h"
#include "../repo/interface/IResultRepository.h"
#include "../repo/interface/IStudentRepository.h"
#include "../repo/interface/ICourseRepository.h" // Needed for credits and validation
#include "EnrollmentService.h" // Needed to check if student is enrolled before entering marks
#include "../../utils/Logger.h"
#include <memory>
#include <numeric> // For std::accumulate
#include <cmath>   // For std::round, isnan etc.
#include <iomanip> // For formatting report
#include <sstream> // For formatting report

// Define grade points mapping (adjust as per university rules)
const std::map<char, double> GRADE_POINTS = {
    {'A', 10.0}, {'B', 8.0}, {'C', 6.0}, {'D', 5.0}, {'E', 4.0}, {'F', 0.0}, {'-', 0.0} // Treat ungraded as 0 for calculation? Or ignore? Ignore seems better.
};

double getGradePoint(char grade) {
    auto it = GRADE_POINTS.find(grade);
    return (it != GRADE_POINTS.end()) ? it->second : 0.0; // Default to 0 if grade not found
}

char IResultService::calculateGradeFromMarks(int marks) {
     if (marks < 0) return '-';
     if (marks > 100) return '?'; // Invalid
     if (marks >= 81) return 'A';
     if (marks >= 66) return 'B';
     if (marks >= 51) return 'C';
     if (marks >= 46) return 'D';
     if (marks >= 36) return 'E';
     return 'F';
}

ResultService::ResultService(std::shared_ptr<IResultRepository> resultRepo,
                std::shared_ptr<IStudentRepository> studentRepo,
                std::shared_ptr<ICourseRepository> courseRepo,
                std::shared_ptr<IEnrollmentService> enrollmentService)
    : _resultRepo(resultRepo), _studentRepo(studentRepo), _courseRepo(courseRepo), _enrollmentService(enrollmentService) {
    if (!_resultRepo || !_studentRepo || !_courseRepo || !_enrollmentService) {
        throw std::invalid_argument("Repositories and EnrollmentService cannot be null in ResultService.");
    }
}

std::vector<CourseResult> ResultService::getStudentResults(const std::string& studentId) const {
    if (!_studentRepo->exists(studentId)) return {};
    return _resultRepo->findCourseResultsByStudent(studentId);
}

std::vector<CourseResult> ResultService::getCourseResults(const std::string& courseId) const {
        if (!_courseRepo->exists(courseId)) return {};
    return _resultRepo->findCourseResultsByCourse(courseId);
}

    std::optional<CourseResult> ResultService::getSpecificResult(const std::string& studentId, const std::string& courseId) const {
        // Optional validation: check student/course existence first
        return _resultRepo->findCourseResult(studentId, courseId);
    }

bool ResultService::enterMarks(const std::string& studentId, const std::string& courseId, int marks) {
    // Validation
    if (!_studentRepo->exists(studentId)) {
        LOG_WARN("Enter marks failed: Student not found: " + studentId); return false;
    }
    if (!_courseRepo->exists(courseId)) {
        LOG_WARN("Enter marks failed: Course not found: " + courseId); return false;
    }
    if (!_enrollmentService->isStudentEnrolled(studentId, courseId)) {
            LOG_WARN("Enter marks failed: Student " + studentId + " not enrolled in course " + courseId);
            return false; // Cannot enter marks if not enrolled
    }
    if (marks < -1 || marks > 100) { // Allow -1 for "not graded yet"
            LOG_WARN("Enter marks failed: Invalid marks (" + std::to_string(marks) + ") for " + studentId + " in " + courseId);
            return false;
    }

    // Create/Update result object
    CourseResult result(studentId, courseId);
    if (!result.setMarks(marks)) { // setMarks handles grade calculation
            LOG_ERROR("Internal error setting marks for " + studentId + " in " + courseId); // Should not happen if validation above passed
            return false;
    }

    // Save to repository (adds or updates)
    if (_resultRepo->addOrUpdateCourseResult(result)) {
        LOG_INFO("Marks entered successfully for " + studentId + " in " + courseId);
        return true;
    } else {
        LOG_ERROR("Failed to save marks to repository for " + studentId + " in " + courseId);
        return false;
    }
}

bool ResultService::enterMultipleMarks(const std::string& studentId, const std::map<std::string, int>& courseMarks) {
        if (!_studentRepo->exists(studentId)) {
            LOG_WARN("Enter multiple marks failed: Student not found: " + studentId); return false;
        }

        bool allSuccess = true;
        for (const auto& pair : courseMarks) {
            const std::string& courseId = pair.first;
            int marks = pair.second;
            // Use the single entry method for validation and saving
            if (!enterMarks(studentId, courseId, marks)) {
                allSuccess = false;
                LOG_ERROR("Failed to enter marks for course " + courseId + " for student " + studentId + ". Continuing with others.");
                // Decide: Stop on first error or try all? Trying all seems better for bulk entry.
            }
        }
        return allSuccess;
}


std::optional<double> ResultService::calculateCGPA(const std::string& studentId) const {
        if (!_studentRepo->exists(studentId)) return std::nullopt;

    auto results = _resultRepo->findCourseResultsByStudent(studentId);
    if (results.empty()) return 0.0; // No results, CGPA is 0? Or nullopt? 0 seems reasonable.

    double totalGradePoints = 0.0;
    int totalCreditsAttempted = 0; // Sum of credits for all courses with results (even 'F' or '-')

    for (const auto& result : results) {
        auto courseOpt = _courseRepo->findById(result.courseId());
        if (!courseOpt) {
                LOG_WARN("CGPA Calc: Skipping result for non-existent course ID: " + result.courseId());
                continue;
        }
        int credits = courseOpt.value().credits();
        if (credits <= 0) continue; // Skip courses with 0 credits

        totalCreditsAttempted += credits;

        // Only include graded results ('F' gives 0 points) in the numerator calculation
        if (result.isGraded() && result.grade() != '-') {
            totalGradePoints += getGradePoint(result.grade()) * credits;
        }
    }

    if (totalCreditsAttempted == 0) {
            return 0.0; // Avoid division by zero if only 0-credit courses attempted
    }

    double cgpa = totalGradePoints / totalCreditsAttempted;

        // Check for NaN or infinity just in case
        if (std::isnan(cgpa) || std::isinf(cgpa)) {
            LOG_ERROR("CGPA calculation resulted in NaN or Infinity for student: " + studentId);
            return std::nullopt;
        }

        // Optional: Round CGPA to 2 decimal places
        // cgpa = std::round(cgpa * 100.0) / 100.0;

    return cgpa;
}

// SGPA calculation would be similar but filter results by semester
std::optional<double> ResultService::calculateSGPA(const std::string& studentId /*, int semester */) const {
    // Implementation would involve:
    // 1. Get results for the specific semester (modify ResultRepo interface if needed)
    // 2. Filter results by semester
    // 3. Perform calculation similar to CGPA but only on semester results.
    LOG_WARN("SGPA calculation not fully implemented.");
    return std::nullopt; // Placeholder
}


std::string ResultService::generateResultReport(const std::string& studentId) const {
    auto studentOpt = _studentRepo->findById(studentId);
    if (!studentOpt) return "Error: Student not found.";

    auto results = getStudentResults(studentId);
    auto cgpaOpt = calculateCGPA(studentId);

    std::ostringstream report;
    report << "--- Result Report ---" << std::endl;
    report << "Student ID:   " << studentOpt.value().id() << std::endl;
    report << "Student Name: " << studentOpt.value().fullName() << std::endl;
    report << "---------------------" << std::endl;
    report << std::left << std::setw(12) << "CourseID"
            << std::setw(35) << "Course Name"
            << std::setw(10) << "Credits"
            << std::setw(8) << "Marks"
            << std::setw(8) << "Grade" << std::endl;
    report << std::setfill('-') << std::setw(73) << "" << std::setfill(' ') << std::endl; // Separator


    if (results.empty()) {
        report << "No results found." << std::endl;
    } else {
        for (const auto& result : results) {
                auto courseOpt = _courseRepo->findById(result.courseId());
                std::string courseName = courseOpt ? courseOpt.value().name() : "Unknown";
                int credits = courseOpt ? courseOpt.value().credits() : 0;

                report << std::left << std::setw(12) << result.courseId()
                    << std::setw(35) << courseName.substr(0, 33) // Truncate name if too long
                    << std::setw(10) << credits
                    << std::setw(8) << (result.isGraded() ? std::to_string(result.marks()) : "N/A")
                    << std::setw(8) << result.grade() << std::endl;
        }
    }

    report << std::setfill('-') << std::setw(73) << "" << std::setfill(' ') << std::endl;
    report << "CGPA: " << (cgpaOpt ? std::to_string(cgpaOpt.value()) : "N/A") << std::endl; // Format CGPA nicely
    report << "---------------------" << std::endl;

    return report.str();
}

// Added the missing methods for ResultService

bool ResultService::updateResult(const std::string& studentId, const std::map<std::string, int>& marks) {
    // This method is similar to enterMultipleMarks
    if (!_studentRepo->exists(studentId)) {
        LOG_WARN("Update result failed: Student not found: " + studentId);
        return false;
    }

    bool allSuccess = true;
    for (const auto& [courseId, mark] : marks) {
        auto existingResult = _resultRepo->findCourseResult(studentId, courseId);
        if (!existingResult) {
            LOG_WARN("Update result failed: No existing result for student " + studentId + " in course " + courseId);
            allSuccess = false;
            continue;
        }

        // Update the result
        CourseResult updatedResult(studentId, courseId, mark);
        if (!_resultRepo->addOrUpdateCourseResult(updatedResult)) {
            LOG_ERROR("Failed to update marks for course " + courseId + " for student " + studentId);
            allSuccess = false;
        }
    }

    return allSuccess;
}

std::optional<char> ResultService::getGrade(int marks) const {
    if (marks < 0 || marks > 100) {
        LOG_WARN("Invalid marks value for grade calculation: " + std::to_string(marks));
        return std::nullopt;
    }
    
    return IResultService::calculateGradeFromMarks(marks);
}

bool ResultService::declareResult(const std::string& studentId) {
    // This method would typically perform operations like finalizing results,
    // marking them as official, or performing any additional validation
    // before results are published/declared for a student
    
    if (!_studentRepo->exists(studentId)) {
        LOG_WARN("Declare result failed: Student not found: " + studentId);
        return false;
    }
    
    auto results = _resultRepo->findCourseResultsByStudent(studentId);
    if (results.empty()) {
        LOG_WARN("Declare result failed: No results found for student: " + studentId);
        return false;
    }
    
    // Verify all enrolled courses have results
    auto enrolledCourses = _enrollmentService->getEnrolledCourses(studentId);
    for (const auto& course : enrolledCourses) {
        auto result = _resultRepo->findCourseResult(studentId, course.id());
        if (!result) {
            LOG_WARN("Student " + studentId + " is missing results for enrolled course: " + course.id());
            // You could decide to return false here or just log a warning
        }
    }
    
    // In a real system, you might mark these results as "declared" in the database
    LOG_INFO("Results declared for student: " + studentId);
    return true;
}