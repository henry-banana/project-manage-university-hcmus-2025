#ifndef CSVRESULTREPO_H
#define CSVRESULTREPO_H

#include "../interface/IResultRepository.h"
#include "../../entities/Result.h" // Includes CourseResult
#include "../../../utils/FileHandler.h"
#include <string>
#include <vector>
#include <mutex>
#include <optional>

// Manages individual CourseResult records in a CSV file
class CsvResultRepo : public IResultRepository {
private:
    std::string _filePath;
    mutable std::mutex _fileMutex;
    const char DELIMITER = ',';

    // Column indices for results.csv
    enum ColumnIndex {
        COL_STUDENT_ID = 0,
        COL_COURSE_ID,
        COL_MARKS, // Store marks (-1 for ungraded)
        // Grade is derived, no need to store usually
        NUM_COLUMNS
    };

    // Loads all CourseResult records
    std::vector<CourseResult> loadFromFile() const;
    // Saves all CourseResult records
    bool saveToFile(const std::vector<CourseResult>& results);
    // Parses a single row into a CourseResult object
    CourseResult parseCourseResult(const std::vector<std::string>& csvRow) const;
    // Formats a CourseResult object into a CSV row (vector of strings)
    std::vector<std::string> formatCourseResult(const CourseResult& result) const;

public:
    explicit CsvResultRepo(std::string csvFilePath);

    // IResultRepository implementation
    std::optional<CourseResult> findCourseResult(const std::string& studentId, const std::string& courseId) const override;
    std::vector<CourseResult> findCourseResultsByStudent(const std::string& studentId) const override;
    std::vector<CourseResult> findCourseResultsByCourse(const std::string& courseId) const override;
    bool addOrUpdateCourseResult(const CourseResult& result) override;
    bool removeCourseResult(const std::string& studentId, const std::string& courseId) override;

    // Semester result operations are not implemented in this basic CSV repo
};

#endif // CSVRESULTREPO_H