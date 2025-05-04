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
    const char CSV_DELIMITER = ',';
    
    std::vector<Faculty> loadFromFile() const;
    bool saveToFile(const std::vector<Faculty>& faculties);
    Faculty parseFaculty(const std::vector<std::string>& csvRow) const;
    std::vector<std::string> formatFaculty(const Faculty& faculty) const;

    enum ColumnIndex {
        COL_ID = 0,
        COL_NAME,
        NUM_COLUMNS
    };

protected:

public:
    explicit CsvFacultyRepo(const std::string& csvFilePath);

    // IFacultyRepository implementation
    std::optional<Faculty> findById(const std::string& facultyId) const override;
    std::optional<Faculty> findByName(const std::string& name) const override;
    std::vector<Faculty> findAll() const override;
    bool add(const Faculty& faculty) override;
    bool update(const Faculty& faculty) override;
    bool remove(const std::string& facultyId) override;
    bool exists(const std::string& facultyId) const override;
};

#endif // CSV_FACULTY_REPO_H