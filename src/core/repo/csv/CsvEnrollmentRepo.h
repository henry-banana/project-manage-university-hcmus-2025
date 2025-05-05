#ifndef CSVENROLLMENTREPO_H
#define CSVENROLLMENTREPO_H

#include "../interface/IEnrollmentRepository.h"
#include "../../../utils/FileHandler.h"
#include <string>
#include <vector>
#include <mutex>
#include <utility> // For std::pair

class CsvEnrollmentRepo : public IEnrollmentRepository {
private:
    std::string _filePath;
    mutable std::mutex _fileMutex;
    const char DELIMITER = ',';

    // Column indices for enrollments.csv
    enum ColumnIndex {
        COL_STUDENT_ID = 0,
        COL_COURSE_ID,
        NUM_COLUMNS
    };

    // Helper functions
    std::vector<EnrollmentRecord> loadFromFile() const;
    bool saveToFile(const std::vector<EnrollmentRecord>& enrollments);
    // No parse/format needed as EnrollmentRecord is simple std::pair<string, string>

public:
    explicit CsvEnrollmentRepo(std::string csvFilePath);

    // IEnrollmentRepository implementation
    bool addEnrollment(const std::string& studentId, const std::string& courseId) override;
    bool removeEnrollment(const std::string& studentId, const std::string& courseId) override;
    bool removeEnrollmentsByStudent(const std::string& studentId) override;
    bool removeEnrollmentsByCourse(const std::string& courseId) override;
    bool isEnrolled(const std::string& studentId, const std::string& courseId) const override;
    std::vector<std::string> findCoursesByStudent(const std::string& studentId) const override;
    std::vector<std::string> findStudentsByCourse(const std::string& courseId) const override;
    std::vector<EnrollmentRecord> findAllEnrollments() const override;
};

#endif // CSVENROLLMENTREPO_H