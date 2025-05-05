#ifndef CSVCOURSEREPO_H
#define CSVCOURSEREPO_H

#include "../interface/ICourseRepository.h" // Adjusted Interface (removed enrollment methods)
#include "../../entities/Course.h"
#include "../../../utils/FileHandler.h"
#include <string>
#include <vector>
#include <mutex>
#include <optional>

class CsvCourseRepo : public ICourseRepository {
private:
    std::string _filePath;
    mutable std::mutex _fileMutex;
    const char DELIMITER = ',';

    // Column indices (adjust to your CSV format)
    enum ColumnIndex {
        COL_ID = 0,
        COL_NAME,
        COL_CREDITS,
        COL_FACULTY_ID,
        NUM_COLUMNS
    };

    std::vector<Course> loadFromFile() const;
    bool saveToFile(const std::vector<Course>& courses);
    Course parseCourse(const std::vector<std::string>& csvRow) const;
    std::vector<std::string> formatCourse(const Course& course) const;

    // NOTE: Enrollment methods are now removed from this repo/interface
    // They belong in IEnrollmentRepository / CsvEnrollmentRepo

public:
    explicit CsvCourseRepo(std::string csvFilePath);

    // ICourseRepository implementation
    std::optional<Course> findById(const std::string& courseId) const override;
    std::vector<Course> findAll() const override;
    std::vector<Course> findByFacultyId(const std::string& facultyId) const override;
    bool add(const Course& course) override;
    bool update(const Course& course) override;
    bool remove(const std::string& courseId) override;
    bool exists(const std::string& courseId) const override;

};

#endif // CSVCOURSEREPO_H