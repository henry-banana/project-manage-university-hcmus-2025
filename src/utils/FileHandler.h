#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>
#include <fstream>

// Lớp tiện ích cơ bản để đọc/ghi file CSV (ví dụ)
// Các Repository cụ thể (CsvStudentRepo) sẽ sử dụng lớp này hoặc logic tương tự
class CsvFileHandler {
private:
    std::string _filename;

    // Helper functions
    std::vector<std::string> split(const std::string& s, char delimiter);
    std::string join(const std::vector<std::string>& elements, char delimiter);

protected:

public:
    CsvFileHandler(const std::string& filename);

    // Đọc tất cả các dòng, mỗi dòng là vector các string (các cột)
    std::vector<std::vector<std::string>> readAllLines(char delimiter = ',');

    // Ghi tất cả dữ liệu vào file (ghi đè)
    bool writeAllLines(const std::vector<std::vector<std::string>>& data, char delimiter = ',');

    // Ghi thêm một dòng vào cuối file
    bool appendLine(const std::vector<std::string>& line, char delimiter = ',');
};

#endif // FILEHANDLER_H