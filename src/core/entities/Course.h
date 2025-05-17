#ifndef COURSE_H
#define COURSE_H

#include "IEntity.h"
#include <string>

class Course : public IEntity {
private:
    std::string _id;         // Mã môn học (e.g., "CS101")
    std::string _name;       // Tên môn học (e.g., "Introduction to Programming")
    int _credits;            // Số tín chỉ
    std::string _facultyId;  // Khoa quản lý môn học

public:
    Course(std::string id, std::string name, int credits, std::string facultyId);

    // Getters & Setters
    const std::string& getId() const;
    const std::string& getName() const;
    int getCredits() const;
    const std::string& getFacultyId() const;

    bool setName(const std::string& name);
    bool setCredits(int credits); // Phải > 0
    bool setFacultyId(const std::string& facultyId); // Cần check tồn tại ở Service

    // Implement IEntity
    std::string getStringId() const override;
    std::string toString() const override;
    ValidationResult validateBasic() const override;
};

#endif // COURSE_H