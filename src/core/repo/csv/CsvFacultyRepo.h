#ifndef CSV_FACULTY_REPO_H
#define CSV_FACULTY_REPO_H

#include "../interface/IFacultyRepository.h"
#include "../../entities/Faculty.h"
#include "../../../utils/FileHandler.h"
#include <string>
#include <vector>
#include <memory>

class CsvFacultyRepo : public IFacultyRepository {
private:
    std::string _filePath;
    // CsvFileHandler _fileHandler;

    // Helpers for CSV processing
    std::vector<Faculty> loadFacultyFromCsv();
    bool saveFacultyToCsv(const std::vector<Faculty>& faculty);
    Faculty parseFacultyFromCsvLine(const std::vector<std::string>& csvLine);
    std::vector<std::string> formatFacultyToCsvLine(const Faculty& faculty);
    const char CSV_DELIMITER = ',';
    
    // Column indexes
    const int COL_ID = 0;
    const int COL_FNAME = 1;
    const int COL_LNAME = 2;
    const int COL_DEPT = 3;
    const int COL_INSTITUTE = 4;
    const int COL_QUALIFICATION = 5;
    const int COL_SPECIALIZATION = 6;
    const int COL_DESIGNATION = 7;
    const int COL_EXPERIENCE = 8;

protected:

public:
    explicit CsvFacultyRepo(const std::string& csvFilePath);

    // IFacultyRepository implementation
    std::optional<Faculty> findById(const std::string& facultyId) override;
    std::vector<Faculty> findAll() override;
    std::vector<Faculty> findByDepartment(const std::string& departmentId) override;
    std::vector<Faculty> findHods() override;
    bool add(const Faculty& faculty) override;
    bool update(const Faculty& faculty) override;
    bool remove(const std::string& facultyId) override;
};

#endif // CSV_FACULTY_REPO_H