#ifndef TEACHER_H
#define TEACHER_H

#include "User.h"
#include <string>
#include <vector> // Có thể cần nếu Teacher chứa danh sách Course dạy

class Faculty; // Forward declaration
class SalaryRecord; // Forward declaration

class Teacher : public User {
private:
    std::string _facultyId;
    std::string _qualification;
    std::string _specializationSubjects; // Có thể tách thành vector<string>

public:
    // Constructor nhận thông tin giảng viên, bao gồm thông tin từ lớp User
    Teacher(std::string id, std::string firstName, std::string lastName,
        std::string facultyId, std::string institute,
        std::string qualification, std::string specializationSubjects,
        std::string designation, int experienceYears);

    // Getter & Setter
    std::string faculty() const;
    void setFaculty(const std::string& faculty);

    // Phương thức hiển thị thông tin giảng viên
    const std::string& facultyId() const { return _facultyId; }
    const std::string& qualification() const { return _qualification; }
    const std::string& specializationSubjects() const { return _specializationSubjects; }

    void display() const override; // Hiển thị thông tin giảng viên
};

#endif // TEACHER_H
