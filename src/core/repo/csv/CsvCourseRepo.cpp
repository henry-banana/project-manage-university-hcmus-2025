#include "CsvCourseRepo.h"
#include "../../../utils/Logger.h"
#include <stdexcept>
#include <algorithm>
#include <charconv> // For std::from_chars (safer int conversion)

CsvCourseRepo::CsvCourseRepo(std::string csvFilePath) : _filePath(std::move(csvFilePath)) {
    // Optional: Initialize file/header
}

// --- Helper Functions ---
std::vector<Course> CsvCourseRepo::loadFromFile() const {
    std::vector<Course> courses;
    std::vector<std::vector<std::string>> csvData;
    try {
        std::lock_guard<std::mutex> lock(_fileMutex);
        csvData = CsvFileHandler::readAllLines(_filePath, DELIMITER);
    } catch (const std::runtime_error& e) {
        LOG_ERROR("Error reading course data: " + std::string(e.what()));
        return courses;
    }

    bool isFirstRow = true;
    for (const auto& row : csvData) {
        if (isFirstRow) {
            isFirstRow = false; continue; // Skip header
        }
        try {
            courses.push_back(parseCourse(row));
        } catch (const std::exception& e) {
             LOG_ERROR("Skipping invalid course row: " + CsvFileHandler::join(row, DELIMITER) + " Error: " + e.what());
        }
    }
    return courses;
}

bool CsvCourseRepo::saveToFile(const std::vector<Course>& courses) {
    std::vector<std::vector<std::string>> csvData;
    // Add header
    csvData.push_back({"ID", "Name", "Credits", "FacultyID"});

    for (const auto& course : courses) {
        try {
             csvData.push_back(formatCourse(course));
        } catch (const std::exception& e) {
             LOG_ERROR("Error formatting course for saving: " + course.id() + " Error: " + e.what());
             return false;
        }
    }

    try {
        std::lock_guard<std::mutex> lock(_fileMutex);
        return CsvFileHandler::writeAllLines(_filePath, csvData, DELIMITER);
    } catch (const std::runtime_error& e) {
        LOG_ERROR("Error writing course data: " + std::string(e.what()));
        return false;
    }
}

Course CsvCourseRepo::parseCourse(const std::vector<std::string>& csvRow) const {
    if (csvRow.size() < NUM_COLUMNS) {
        throw std::runtime_error("Incorrect number of columns in course CSV row.");
    }

    int credits = 0;
    auto result = std::from_chars(csvRow[COL_CREDITS].data(), csvRow[COL_CREDITS].data() + csvRow[COL_CREDITS].size(), credits);
    if (result.ec != std::errc()) {
        LOG_WARN("Invalid credits value '" + csvRow[COL_CREDITS] + "' for course ID: " + csvRow[COL_ID] + ". Defaulting to 0.");
        credits = 0; // Or throw?
    }

    return Course(
        csvRow[COL_ID],
        csvRow[COL_NAME],
        credits,
        csvRow[COL_FACULTY_ID]
    );
}

std::vector<std::string> CsvCourseRepo::formatCourse(const Course& course) const {
    return {
        course.id(),
        course.name(),
        std::to_string(course.credits()),
        course.facultyId()
    };
}

// --- ICourseRepository Implementation ---
std::optional<Course> CsvCourseRepo::findById(const std::string& courseId) const {
    auto courses = loadFromFile();
    auto it = std::find_if(courses.begin(), courses.end(), [&](const Course& c){ return c.id() == courseId; });
    return (it != courses.end()) ? std::optional<Course>(*it) : std::nullopt;
}

std::vector<Course> CsvCourseRepo::findAll() const {
    return loadFromFile();
}

std::vector<Course> CsvCourseRepo::findByFacultyId(const std::string& facultyId) const {
    auto all = loadFromFile();
    std::vector<Course> filtered;
    std::copy_if(all.begin(), all.end(), std::back_inserter(filtered),
                 [&](const Course& c){ return c.facultyId() == facultyId; });
    return filtered;
}

bool CsvCourseRepo::add(const Course& course) {
    if (exists(course.id())) return false;
    auto courses = loadFromFile();
    courses.push_back(course);
    return saveToFile(courses);
}

bool CsvCourseRepo::update(const Course& course) {
    auto courses = loadFromFile();
    auto it = std::find_if(courses.begin(), courses.end(), [&](const Course& c){ return c.id() == course.id(); });
    if (it != courses.end()) {
        *it = course;
        return saveToFile(courses);
    }
    return false; // Not found
}

bool CsvCourseRepo::remove(const std::string& courseId) {
    // Note: Service layer should check for enrollments/results before calling this
    auto courses = loadFromFile();
    auto it = std::remove_if(courses.begin(), courses.end(), [&](const Course& c){ return c.id() == courseId; });
    if (it != courses.end()) {
        courses.erase(it, courses.end());
        return saveToFile(courses);
    }
    return false; // Not found
}

bool CsvCourseRepo::exists(const std::string& courseId) const {
    return findById(courseId).has_value();
}

// Implementation of enrollment-related methods (temporary)
bool CsvCourseRepo::enrollStudent(const std::string& studentId, const std::string& courseId) {
    // Temporary implementation - this should be moved to a proper enrollment repository
    LOG_INFO("Enrolling student " + studentId + " in course " + courseId);
    return true; // Placeholder implementation
}

bool CsvCourseRepo::unenrollStudent(const std::string& studentId, const std::string& courseId) {
    // Temporary implementation - this should be moved to a proper enrollment repository
    LOG_INFO("Unenrolling student " + studentId + " from course " + courseId);
    return true; // Placeholder implementation
}

std::vector<std::string> CsvCourseRepo::findStudentsByCourse(const std::string& courseId) const {
    // Temporary implementation - this should be moved to a proper enrollment repository
    LOG_INFO("Finding students enrolled in course " + courseId);
    return {}; // Return empty vector as placeholder
}

std::vector<std::string> CsvCourseRepo::findCoursesByStudent(const std::string& studentId) const {
    // Temporary implementation - this should be moved to a proper enrollment repository
    LOG_INFO("Finding courses for student " + studentId);
    return {}; // Return empty vector as placeholder
}

bool CsvCourseRepo::isStudentEnrolled(const std::string& studentId, const std::string& courseId) const {
    // Temporary implementation - this should be moved to a proper enrollment repository
    LOG_INFO("Checking if student " + studentId + " is enrolled in course " + courseId);
    return false; // Placeholder implementation
}