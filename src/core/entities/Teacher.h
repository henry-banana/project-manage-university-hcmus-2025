#ifndef TEACHER_H
#define TEACHER_H

#include "User.h"
#include <string>
#include <vector> // Có thể cần nếu Teacher chứa danh sách Course dạy

class Teacher : public User {
private:
    std::string _facultyId;
    std::string _qualification;
    std::string _specializationSubjects; // Có thể tách thành vector<string>
    std::string _designation; // Chức vụ của giảng viên
    int _experienceYears; // Số năm kinh nghiệm

public:
    // Constructor nhận thông tin giảng viên, bao gồm thông tin từ lớp User
    Teacher();
    Teacher(std::string id, std::string firstName, std::string lastName,
        std::string facultyId, std::string institute,
        std::string qualification, std::string specializationSubjects,
        std::string designation, int experienceYears);
    Teacher(const Teacher& other);
    Teacher& operator=(const Teacher& other);
    ~Teacher() override = default; // Destructor ảo để đảm bảo dọn dẹp đúng cách trong kế thừa

    // Getter & Setter
    const std::string& facultyId() const;
    const std::string& qualification() const;
    const std::string& specializationSubjects() const;
    const std::string& designation() const;
    int experienceYears() const;

    void setFacultyId(const std::string& facultyId);
    void setQualification(const std::string& qualification);
    void setSpecializationSubjects(const std::string& specializationSubjects);
    void setDesignation(const std::string& designation);
    void setExperienceYears(int experienceYears);

    void display() const override; // Hiển thị thông tin giảng viên
};

#endif // TEACHER_H
