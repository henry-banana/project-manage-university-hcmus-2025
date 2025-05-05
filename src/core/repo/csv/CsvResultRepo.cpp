#include "CsvResultRepo.h"
#include "../../../utils/Logger.h"
#include <stdexcept>
#include <algorithm>
#include <charconv>

CsvResultRepo::CsvResultRepo(std::string csvFilePath) : _filePath(std::move(csvFilePath)) {
    // Optional: Initialize file/header
}

// --- Helper Functions ---
std::vector<CourseResult> CsvResultRepo::loadFromFile() const {
     std::vector<CourseResult> results;
     std::vector<std::vector<std::string>> csvData;
     try {
         std::lock_guard<std::mutex> lock(_fileMutex);
         csvData = CsvFileHandler::readAllLines(_filePath, DELIMITER);
     } catch (const std::runtime_error& e) {
         LOG_ERROR("Error reading result data: " + std::string(e.what()));
         return results;
     }

     bool isFirstRow = true;
     for (const auto& row : csvData) {
         if (isFirstRow) {
             isFirstRow = false; continue; // Skip header
         }
         try {
             results.push_back(parseCourseResult(row));
         } catch (const std::exception& e) {
              LOG_ERROR("Skipping invalid result row: " + CsvFileHandler::join(row, DELIMITER) + " Error: " + e.what());
         }
     }
     return results;
}

bool CsvResultRepo::saveToFile(const std::vector<CourseResult>& results) {
     std::vector<std::vector<std::string>> csvData;
     csvData.push_back({"StudentID", "CourseID", "Marks"}); // Header

     for (const auto& result : results) {
          try {
               csvData.push_back(formatCourseResult(result));
          } catch (const std::exception& e) {
               LOG_ERROR("Error formatting result for saving: Student " + result.studentId() + ", Course " + result.courseId() + " Error: " + e.what());
               return false;
          }
     }

     try {
         std::lock_guard<std::mutex> lock(_fileMutex);
         return CsvFileHandler::writeAllLines(_filePath, csvData, DELIMITER);
     } catch (const std::runtime_error& e) {
         LOG_ERROR("Error writing result data: " + std::string(e.what()));
         return false;
     }
}

CourseResult CsvResultRepo::parseCourseResult(const std::vector<std::string>& csvRow) const {
    if (csvRow.size() < NUM_COLUMNS) {
         throw std::runtime_error("Incorrect number of columns in result CSV row.");
    }

    int marks = -1; // Default to ungraded
    auto res = std::from_chars(csvRow[COL_MARKS].data(), csvRow[COL_MARKS].data() + csvRow[COL_MARKS].size(), marks);

    if (res.ec != std::errc()) {
        LOG_WARN("Invalid marks value '" + csvRow[COL_MARKS] + "' for Student " + csvRow[COL_STUDENT_ID] + ", Course " + csvRow[COL_COURSE_ID] + ". Defaulting to -1.");
        marks = -1;
    }
    // Basic validation on parsed marks
    if (marks < -1 || marks > 100) {
        LOG_WARN("Marks value '" + std::to_string(marks) + "' out of range [-1, 100] for Student " + csvRow[COL_STUDENT_ID] + ", Course " + csvRow[COL_COURSE_ID] + ". Setting to -1.");
        marks = -1;
    }


    // Constructor handles setting grade based on marks
    return CourseResult(csvRow[COL_STUDENT_ID], csvRow[COL_COURSE_ID], marks);
}

std::vector<std::string> CsvResultRepo::formatCourseResult(const CourseResult& result) const {
    return {
        result.studentId(),
        result.courseId(),
        std::to_string(result.marks()) // Store marks, grade is derived
    };
}

// --- IResultRepository Implementation ---
std::optional<CourseResult> CsvResultRepo::findCourseResult(const std::string& studentId, const std::string& courseId) const {
    auto results = loadFromFile();
    auto it = std::find_if(results.begin(), results.end(), [&](const CourseResult& r) {
        return r.studentId() == studentId && r.courseId() == courseId;
    });
    return (it != results.end()) ? std::optional<CourseResult>(*it) : std::nullopt;
}

std::vector<CourseResult> CsvResultRepo::findCourseResultsByStudent(const std::string& studentId) const {
    auto all = loadFromFile();
    std::vector<CourseResult> filtered;
    std::copy_if(all.begin(), all.end(), std::back_inserter(filtered),
                 [&](const CourseResult& r) { return r.studentId() == studentId; });
    return filtered;
}

std::vector<CourseResult> CsvResultRepo::findCourseResultsByCourse(const std::string& courseId) const {
     auto all = loadFromFile();
     std::vector<CourseResult> filtered;
     std::copy_if(all.begin(), all.end(), std::back_inserter(filtered),
                  [&](const CourseResult& r) { return r.courseId() == courseId; });
     return filtered;
}

bool CsvResultRepo::addOrUpdateCourseResult(const CourseResult& result) {
    auto results = loadFromFile();
    auto it = std::find_if(results.begin(), results.end(), [&](const CourseResult& r) {
        return r.studentId() == result.studentId() && r.courseId() == result.courseId();
    });

    if (it != results.end()) {
        // Update existing record
        *it = result;
    } else {
        // Add new record
        results.push_back(result);
    }
    return saveToFile(results);
}

bool CsvResultRepo::removeCourseResult(const std::string& studentId, const std::string& courseId) {
    auto results = loadFromFile();
    auto it = std::remove_if(results.begin(), results.end(), [&](const CourseResult& r) {
         return r.studentId() == studentId && r.courseId() == courseId;
    });

    if (it != results.end()) {
        results.erase(it, results.end());
        return saveToFile(results);
    }
    return false; // Not found
}