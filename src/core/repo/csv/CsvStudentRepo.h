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

    // Hàm helper private để đọc/ghi/parse dữ liệu CSV
    std::vector<Student> loadStudentsFromCsv();
    bool saveStudentsToCsv(const std::vector<Student>& students);
    Student parseStudentFromCsvLine(const std::vector<std::string>& csvLine);
    std::vector<std::string> formatStudentToCsvLine(const Student& student);
    const char CSV_DELIMITER = ','; // Hoặc đọc từ config
    // Cần chỉ định rõ index cột trong file CSV
    const int COL_ID = 0;
    const int COL_FNAME = 1;
    const int COL_LNAME = 2;
    const int COL_INSTITUTE = 3;
    const int COL_DEPT = 4;
    // ... thêm các cột khác như fee, quota từ file Sdata.txt cũ
    const int COL_FEE = 5; // Giả định từ Sdata.txt
    const int COL_QUOTA = 6; // Giả định từ Sdata.txt

    // Cần xử lý đọc cả file Sdata.txt và Sfees.txt để lấy đủ thông tin Student và FeeRecord
    // Hoặc tốt hơn là gộp thông tin vào một file students.csv duy nhất.
    // Ví dụ students.csv: id,fname,lname,institute,dept_id,quota
    // Ví dụ fees.csv: student_id,total_fee,pending_fee

protected:

public:
    // Constructor nhận đường dẫn file CSV
    // Có thể inject FileHandler nếu muốn linh hoạt hơn
    explicit CsvStudentRepo(const std::string& csvFilePath);

    // Triển khai các phương thức từ interface IStudentRepository
    std::optional<Student> findById(const std::string& studentId) override;
    std::vector<Student> findAll() override;
    std::vector<Student> findByDepartment(const std::string& departmentId) override;
    bool add(const Student& student) override;
    bool update(const Student& student) override;
    bool remove(const std::string& studentId) override;
};

#endif // CSVSTUDENTREPO_H