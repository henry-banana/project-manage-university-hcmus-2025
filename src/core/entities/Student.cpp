#include "Student.h"
#include "User.h"
#include <iostream>
#include <format>
#include "../../utils/Logger.h"

Student::Student() : User(), _facultyId("") {}

Student::Student(std::string id,
    std::string firstName,
    std::string lastName,
    std::string address,
    std::string citizenID,
    std::string email, 
    std::string phoneNumber,
    std::string facultyId)
    : User(
        id,
        firstName,
        lastName,
        address,
        citizenID,
        email,
        phoneNumber,
        UserRole::STUDENT // Mặc định là sinh viên
    ),
    _facultyId(std::move(facultyId)) {
        LOG_INFO("Student object created with ID: " + id);
    }

Student::Student(const Student& other)
    : User(other), _facultyId(other._facultyId) {}
 
Student& Student::operator=(const Student& other) {
    if (this != &other) { // Kiểm tra tự gán
        User::operator=(other); // Gọi toán tử gán của lớp cơ sở
        _facultyId = other._facultyId; // Gán giá
    }
    return *this; // Trả về đối tượng hiện tại
}

// Getter và Setter cho các thuộc tính riêng của Student
const std::string& Student::facultyId() const {
    return _facultyId;
}

void Student::setFacultyId(const std::string& facultyId) {
    _facultyId = facultyId;
}

void Student::display() const {
    std::cout << std::format(
                "--- Student Information ---\n ",
                "ID: {}\n",
                "Full Name: {} {}\n",
                "Birthday: {}\n",
                "Citizen ID: {}\n",
                "Faculty ID: {}\n",
                "Email: {}\n",
                "Phone Number: {}\n",
                "Address: {}\n",
                id(), firstName(), lastName(), Birthday(), citizenID(), facultyId(), email(), phoneNumber(), address()    
    );
}