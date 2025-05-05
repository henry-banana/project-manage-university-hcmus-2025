#include "CsvEnrollmentRepo.h"
#include "../../../utils/Logger.h"
#include <stdexcept>
#include <algorithm> // For std::find_if, std::remove_if, std::copy_if

CsvEnrollmentRepo::CsvEnrollmentRepo(std::string csvFilePath) : _filePath(std::move(csvFilePath)) {
    // Optional: Initialize file/header
}

// --- Helper Functions ---
std::vector<IEnrollmentRepository::EnrollmentRecord> CsvEnrollmentRepo::loadFromFile() const {
    std::vector<EnrollmentRecord> enrollments;
    std::vector<std::vector<std::string>> csvData;
    try {
        std::lock_guard<std::mutex> lock(_fileMutex);
        csvData = CsvFileHandler::readAllLines(_filePath, DELIMITER);
    } catch (const std::runtime_error& e) {
        // Log error but maybe continue if file just doesn't exist yet?
        LOG_ERROR("Error reading enrollment data: " + std::string(e.what()));
        return enrollments; // Return empty list
    }

    bool isFirstRow = true;
    for (const auto& row : csvData) {
        if (isFirstRow) {
            isFirstRow = false; continue; // Skip header
        }
        if (row.size() >= NUM_COLUMNS) {
            enrollments.emplace_back(row[COL_STUDENT_ID], row[COL_COURSE_ID]);
        } else {
            LOG_WARN("Skipping invalid enrollment row: " + CsvFileHandler::join(row, DELIMITER));
        }
    }
    return enrollments;
}

bool CsvEnrollmentRepo::saveToFile(const std::vector<EnrollmentRecord>& enrollments) {
    std::vector<std::vector<std::string>> csvData;
    // Add header
    csvData.push_back({"StudentID", "CourseID"});

    for (const auto& record : enrollments) {
        csvData.push_back({record.first, record.second});
    }

    try {
        std::lock_guard<std::mutex> lock(_fileMutex);
        return CsvFileHandler::writeAllLines(_filePath, csvData, DELIMITER);
    } catch (const std::runtime_error& e) {
        LOG_ERROR("Error writing enrollment data: " + std::string(e.what()));
        return false;
    }
}

// --- IEnrollmentRepository Implementation ---
bool CsvEnrollmentRepo::addEnrollment(const std::string& studentId, const std::string& courseId) {
    if (isEnrolled(studentId, courseId)) {
        LOG_INFO("Enrollment already exists for Student: " + studentId + ", Course: " + courseId);
        return true; // Already exists, consider it success (idempotent)
    }
    auto enrollments = loadFromFile();
    enrollments.emplace_back(studentId, courseId);
    return saveToFile(enrollments);
}

bool CsvEnrollmentRepo::removeEnrollment(const std::string& studentId, const std::string& courseId) {
    auto enrollments = loadFromFile();
    auto originalSize = enrollments.size();
    auto it = std::remove_if(enrollments.begin(), enrollments.end(),
                             [&](const EnrollmentRecord& r) {
                                 return r.first == studentId && r.second == courseId;
                             });

    if (it != enrollments.end()) { // If something was removed
        enrollments.erase(it, enrollments.end());
        return saveToFile(enrollments);
    }
    LOG_WARN("Attempted to remove non-existent enrollment for Student: " + studentId + ", Course: " + courseId);
    return false; // Not found
}

bool CsvEnrollmentRepo::removeEnrollmentsByStudent(const std::string& studentId) {
     auto enrollments = loadFromFile();
     auto originalSize = enrollments.size();
     auto it = std::remove_if(enrollments.begin(), enrollments.end(),
                              [&](const EnrollmentRecord& r) { return r.first == studentId; });

     if (it != enrollments.end()) { // If something was removed
         enrollments.erase(it, enrollments.end());
         return saveToFile(enrollments);
     }
     // Return true even if none found, as the state matches the request (no enrollments for student)
     return true;
}

bool CsvEnrollmentRepo::removeEnrollmentsByCourse(const std::string& courseId) {
     auto enrollments = loadFromFile();
     auto originalSize = enrollments.size();
     auto it = std::remove_if(enrollments.begin(), enrollments.end(),
                              [&](const EnrollmentRecord& r) { return r.second == courseId; });

     if (it != enrollments.end()) { // If something was removed
         enrollments.erase(it, enrollments.end());
         return saveToFile(enrollments);
     }
      // Return true even if none found
     return true;
}


bool CsvEnrollmentRepo::isEnrolled(const std::string& studentId, const std::string& courseId) const {
    auto enrollments = loadFromFile();
    return std::any_of(enrollments.begin(), enrollments.end(),
                       [&](const EnrollmentRecord& r) {
                           return r.first == studentId && r.second == courseId;
                       });
}

std::vector<std::string> CsvEnrollmentRepo::findCoursesByStudent(const std::string& studentId) const {
    auto enrollments = loadFromFile();
    std::vector<std::string> courseIds;
    for (const auto& record : enrollments) {
        if (record.first == studentId) {
            courseIds.push_back(record.second);
        }
    }
    // Optional: sort and remove duplicates if necessary
    // std::sort(courseIds.begin(), courseIds.end());
    // courseIds.erase(std::unique(courseIds.begin(), courseIds.end()), courseIds.end());
    return courseIds;
}

std::vector<std::string> CsvEnrollmentRepo::findStudentsByCourse(const std::string& courseId) const {
    auto enrollments = loadFromFile();
    std::vector<std::string> studentIds;
    for (const auto& record : enrollments) {
        if (record.second == courseId) {
            studentIds.push_back(record.first);
        }
    }
     // Optional: sort and remove duplicates
    return studentIds;
}

std::vector<IEnrollmentRepository::EnrollmentRecord> CsvEnrollmentRepo::findAllEnrollments() const {
    return loadFromFile();
}