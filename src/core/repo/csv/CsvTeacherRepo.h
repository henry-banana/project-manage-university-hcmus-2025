#ifndef CSVTEACHERREPO_H
#define CSVTEACHERREPO_H

#include "../interface/ITeacherRepository.h"
#include "../../entities/Teacher.h"
#include "../../../utils/FileHandler.h"
#include <string>
#include <vector>
#include <optional>

class CsvTeacherRepo : public ITeacherRepository {
private:
    std::string _filePath;
    const char DELIMITER = ',';

    // Column indices (adjust to your CSV format)
    enum ColumnIndex {
        COL_ID = 0,
        COL_FNAME,
        COL_LNAME,
        COL_CITIZEN_ID,
        COL_FACULTY_ID, // Department/School ID
        COL_QUALIFICATION,
        COL_SPECIALIZATION,
        COL_DESIGNATION,
        COL_EXPERIENCE,
        COL_EMAIL,
        COL_PHONE,
        COL_ADDRESS,
        COL_BIRTHDAY,
        // COL_PASSWORD_HASH, // If storing here
        // COL_SALT,          // If storing here
        NUM_COLUMNS
    };


    std::vector<Teacher> loadFromFile() const;
    bool saveToFile(const std::vector<Teacher>& teachers);
    Teacher parseTeacher(const std::vector<std::string>& csvRow) const;
    std::vector<std::string> formatTeacher(const Teacher& teacher) const;

public:
    explicit CsvTeacherRepo(std::string csvFilePath);

    std::optional<Teacher> findById(const std::string& teacherId) const override;
    std::vector<Teacher> findAll() const override;
    std::vector<Teacher> findByFacultyId(const std::string& facultyId) const override;
    std::vector<Teacher> findByDesignation(const std::string& designation) const override;
    bool add(const Teacher& teacher) override;
    bool update(const Teacher& teacher) override;
    bool remove(const std::string& teacherId) override;
    bool exists(const std::string& teacherId) const override;
};

#endif // CSVTEACHERREPO_H