#ifndef TEACHER_H
#define TEACHER_H

#include "User.h"
#include <string>
#include <vector> // Có thể cần nếu Teacher chứa danh sách Course dạy


class Teacher : public User {
private:
    std::string _department;

public:
    // Constructor nhận thông tin giảng viên, bao gồm thông tin từ lớp User
    Teacher(std::string id, std::string firstName, std::string lastName, std::string department);

    // Getter & Setter
    std::string getDepartment() const;
    void setDepartment(const std::string& department);

    // Phương thức hiển thị thông tin giảng viên
    void showInfo() const;
};

#endif // TEACHER_H
