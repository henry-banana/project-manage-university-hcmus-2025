#ifndef TEACHER_H
#define TEACHER_H

#include "User.h"
#include <string>
#include <vector>

// Forward declaration
class Faculty;
// class Course; // Nếu Teacher có danh sách Course dạy

class Teacher : public User {
private:
    std::string _facultyId;
    std::string _qualification;          // Bằng cấp (e.g., PhD, MSc)
    std::string _specializationSubjects; // Chuyên ngành giảng dạy (có thể là list)
    std::string _designation;            // Chức vụ (e.g., Lecturer, Professor, HOD)
    int _experienceYears;

public:
    Teacher(const std::string& id,
            const std::string& firstName,
            const std::string& lastName,
            const std::string& facultyId, // (➕) Thêm vào constructor
            LoginStatus status = LoginStatus::ACTIVE);

    // Getters & Setters
    const std::string& getFacultyId() const;
    const std::string& getQualification() const;
    const std::string& getSpecializationSubjects() const;
    const std::string& getDesignation() const;
    int getExperienceYears() const;

    bool setFacultyId(const std::string& facultyId);
    bool setQualification(const std::string& qualification);
    bool setSpecializationSubjects(const std::string& subjects);
    bool setDesignation(const std::string& designation);
    bool setExperienceYears(int years); // Phải >= 0

    // Override
    std::string display() const override;
    ValidationResult validateBasic() const override;
};

#endif // TEACHER_H