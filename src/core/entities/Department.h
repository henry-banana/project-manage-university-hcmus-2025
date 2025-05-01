#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <string>

class Faculty; // Forward declaration

class Department {
private:
    std::string _id; // Ví dụ: "CE", "IT"
    std::string _name; // Ví dụ: "Computer Engineering"
    std::string _hodFacultyId; // Tham chiếu đến ID của Faculty là HOD

protected:

public:
    Department(std::string id, std::string name, std::string hodFacultyId = ""); // hodFacultyId có thể trống

    const std::string& getId() const { return _id; }
    const std::string& getName() const { return _name; }
    const std::string& getHodFacultyId() const { return _hodFacultyId; }

    void setHodFacultyId(std::string hodId) { _hodFacultyId = std::move(hodId); }
};

#endif // DEPARTMENT_H