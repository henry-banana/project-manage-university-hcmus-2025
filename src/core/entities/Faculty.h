#ifndef FACULTY_H
#define FACULTY_H

#include "User.h"
#include <string>
#include <vector> // Có thể cần nếu Faculty dạy nhiều Course

class Department; // Forward declaration
class SalaryRecord; // Forward declaration

class Faculty : public User {
private:
    std::string _departmentId;
    std::string _institute;
    std::string _qualification;
    std::string _specializationSubjects; // Có thể tách thành vector<string>
    std::string _designation;
    int _experienceYears;
    // Không chứa SalaryRecord trực tiếp

protected:

public:
    Faculty(std::string id, std::string firstName, std::string lastName,
            std::string departmentId, std::string institute,
            std::string qualification, std::string specializationSubjects,
            std::string designation, int experienceYears);

    // Getters
    const std::string& getDepartmentId() const { return _departmentId; }
    const std::string& getInstitute() const { return _institute; }
    const std::string& getQualification() const { return _qualification; }
    const std::string& getSpecializationSubjects() const { return _specializationSubjects; }
    const std::string& getDesignation() const { return _designation; }
    int getExperienceYears() const { return _experienceYears; }

    // Setters (nếu cần)
    void setDesignation(std::string designation) { _designation = std::move(designation); }
    // ...

    bool isHod() const; // Logic kiểm tra designation có phải HOD không
};

#endif // FACULTY_H