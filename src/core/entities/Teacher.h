#ifndef TEACHER_H
#define TEACHER_H

#include "User.h"
#include <string>
#include <vector> // Có thể cần nếu Teacher chứa danh sách Course dạy

class Department; // Forward declaration
class SalaryRecord; // Forward declaration

class Teacher : public User {
private:
    std::string _departmentId;
    std::string _institute;
    std::string _qualification;
    std::string _specializationSubjects; // Có thể tách thành vector<string>
    std::string _designation;
    int _experienceYears;

public:
    // Constructor nhận thông tin giảng viên, bao gồm thông tin từ lớp User
    Teacher(std::string id, std::string firstName, std::string lastName,
        std::string departmentId, std::string institute,
        std::string qualification, std::string specializationSubjects,
        std::string designation, int experienceYears);

    // Getter & Setter
    std::string getDepartment() const;
    void setDepartment(const std::string& department);

    // Phương thức hiển thị thông tin giảng viên
    const std::string& getDepartmentId() const { return _departmentId; }
    const std::string& getInstitute() const { return _institute; }
    const std::string& getQualification() const { return _qualification; }
    const std::string& getSpecializationSubjects() const { return _specializationSubjects; }
    const std::string& getDesignation() const { return _designation; }
    int getExperienceYears() const { return _experienceYears; }
    
    void showInfo() const;
    bool isHod() const; // Logic kiểm tra designation có phải HOD không
};

#endif // TEACHER_H
