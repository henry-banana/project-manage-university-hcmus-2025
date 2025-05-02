#ifndef TEACHER_H
#define TEACHER_H

#include <iostream>
#include <string>
#include <vector>

class Teacher {
private:
    std::string name;
    std::string teacherId;
    std::string major;
    std::string department;
    float salary;
    std::vector<std::string> classesTaught; // Danh sách mã lớp đang dạy

public:
    // Constructor
    Teacher();
    Teacher(const std::string& name, const std::string& id,
            const std::string& major, const std::string& department, float salary);

    // Setters
    void setName(const std::string& name);
    void setTeacherId(const std::string& id);
    void setMajor(const std::string& major);
    void setDepartment(const std::string& department);
    void setSalary(float salary);

    // Getters
    std::string getName() const;
    std::string getTeacherId() const;
    std::string getMajor() const;
    std::string getDepartment() const;
    float getSalary() const;

    // Classes management
    void addClass(const std::string& classId);
    void removeClass(const std::string& classId);
    std::vector<std::string> getClassesTaught() const;

    // Display
    void displayInfo() const;
};

#endif
