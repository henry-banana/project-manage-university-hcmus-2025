#include "Teacher.h"
#include <algorithm>

// Constructors
Teacher::Teacher() : salary(0.0f) {}

Teacher::Teacher(const std::string& name, const std::string& id,
                 const std::string& major, const std::string& department, float salary)
    : name(name), teacherId(id), major(major), department(department), salary(salary) {}

// Setters
void Teacher::setName(const std::string& name) {
    this->name = name;
}

void Teacher::setTeacherId(const std::string& id) {
    this->teacherId = id;
}

void Teacher::setMajor(const std::string& major) {
    this->major = major;
}

void Teacher::setDepartment(const std::string& department) {
    this->department = department;
}

void Teacher::setSalary(float salary) {
    this->salary = salary;
}

// Getters
std::string Teacher::getName() const {
    return name;
}

std::string Teacher::getTeacherId() const {
    return teacherId;
}

std::string Teacher::getMajor() const {
    return major;
}

std::string Teacher::getDepartment() const {
    return department;
}

float Teacher::getSalary() const {
    return salary;
}

// Class list management
void Teacher::addClass(const std::string& classId) {
    classesTaught.push_back(classId);
}

void Teacher::removeClass(const std::string& classId) {
    auto it = std::find(classesTaught.begin(), classesTaught.end(), classId);
    if (it != classesTaught.end()) {
        classesTaught.erase(it);
    }
}

std::vector<std::string> Teacher::getClassesTaught() const {
    return classesTaught;
}

// Display
void Teacher::displayInfo() const {
    std::cout << "=== Thong tin Giang vien ===\n";
    std::cout << "Ho ten       : " << name << "\n";
    std::cout << "Ma GV        : " << teacherId << "\n";
    std::cout << "Chuyen nganh : " << major << "\n";
    std::cout << "Bo mon       : " << department << "\n";
    std::cout << "Luong        : " << salary << "\n";
    std::cout << "Lop dang day: ";
    if (classesTaught.empty()) {
        std::cout << "Khong co lop nao.\n";
    } else {
        for (const auto& cls : classesTaught) {
            std::cout << cls << " ";
        }
        std::cout << "\n";
    }
}
