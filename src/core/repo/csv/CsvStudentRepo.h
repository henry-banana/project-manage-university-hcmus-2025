#ifndef CSVSTUDENTREPO_H
#define CSVSTUDENTREPO_H

#include "../interface/IStudentRepository.h"
#include "../../entities/Student.h"
#include "../../../utils/FileHandler.h" // Giả sử dùng FileHandler chung
#include <string>
#include <vector>
#include <memory> // For shared_ptr if needed

class CsvStudentRepo : public IStudentRepository {
private:
    std::string _filePath;
    // Có thể có một instance CsvFileHandler hoặc logic đọc/ghi trực tiếp
    // CsvFileHandler _fileHandler;
    const char DELIMITER = ',';

    enum ColumnIndex {
        COL_ID = 0,
        COL_FNAME,
        COL_LNAME,
        COL_CITIZEN_ID,
        COL_FACULTY_ID, // Department/School ID
        COL_EMAIL,
        COL_PHONE,
        COL_ADDRESS,
        COL_BIRTHDAY, // Store as "dd/mm/yyyy" string
        // Add password hash/salt if storing here (better in LoginRepo)
        // COL_PASSWORD_HASH,
        // COL_SALT,
        NUM_COLUMNS // Keep track of the total number of columns
    };

    // Helper functions
    std::vector<Student> loadFromFile() const;
    bool saveToFile(const std::vector<Student>& students);
    Student parseStudent(const std::vector<std::string>& csvRow) const;
    std::vector<std::string> formatStudent(const Student& student) const;

protected:

public:
    // Constructor nhận đường dẫn file CSV
    // Có thể inject FileHandler nếu muốn linh hoạt hơn
    explicit CsvStudentRepo(const std::string& csvFilePath);

    // Triển khai các phương thức từ interface IStudentRepository
    std::optional<Student> findById(const std::string& studentId) const override;
    std::vector<Student> findAll() const override;
    std::vector<Student> findByFacultyId(const std::string& facultyId) const override;
    bool add(const Student& student) override;
    bool update(const Student& student) override;
    bool remove(const std::string& studentId) override;
    bool exists(const std::string& studentId) const override;
};

#endif // CSVSTUDENTREPO_H