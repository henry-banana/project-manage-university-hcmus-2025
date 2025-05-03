#ifndef FACULTY_H
#define FACULTY_H

#include "User.h"
#include "Teacher.h"
#include <string>
#include <vector> // Có thể cần nếu Faculty dạy nhiều Course


#include <string>

class Teacher; // Forward declaration

class Faculty {
private:
    std::string _id; // Ví dụ: "CE", "IT"
    std::string _name; // Ví dụ: "Computer Engineering"
    std::string _hodFacultyId; // Tham chiếu đến ID của Faculty là HOD

protected:

public:
    Faculty(std::string id, std::string name, std::string hodFacultyId = ""); // hodFacultyId có thể trống

    const std::string& getId() const { return _id; }
    const std::string& getName() const { return _name; }
    const std::string& getHodFacultyId() const { return _hodFacultyId; }

    void setHodFacultyId(std::string hodId) { _hodFacultyId = std::move(hodId); }
};

#endif // FACULTY_H