#include "Teacher.h"
#include <iostream>
#include <format>

Teacher::Teacher() : User(), _facultyId(""), _qualification(""),
    _specializationSubjects(""), _designation(""),
    _experienceYears(0) {}

Teacher::Teacher(std::string id,
    std::string firstName,
    std::string lastName,
    std::string address,
    std::string citizenID,
    std::string email, 
    std::string phoneNumber,
    std::string facultyId, std::string institute,
    std::string qualification, std::string specializationSubjects,
    std::string designation, int experienceYears) 
    : User(id,
        firstName,
        lastName,
        address,
        citizenID,
        email, 
        phoneNumber, 
        UserRole::TEACHER) ,
    _facultyId(facultyId), _qualification(qualification), 
    _specializationSubjects(specializationSubjects), _designation(designation),
    _experienceYears(experienceYears) {}

Teacher::Teacher(const Teacher& other) : User(other),
    _facultyId(other._facultyId), _qualification(other._qualification),
    _specializationSubjects(other._specializationSubjects), _designation(other._designation),
    _experienceYears(other._experienceYears) {}

Teacher& Teacher::operator=(const Teacher& other) {
    if (this != &other) { // Kiểm tra tự gán
        User::operator=(other); // Gọi toán tử gán của lớp cơ sở
        _facultyId = other._facultyId;
        _qualification = other._qualification;
        _specializationSubjects = other._specializationSubjects;
        _designation = other._designation;
        _experienceYears = other._experienceYears;
    }
    return *this;
}

// Getter & Setter
const std::string& Teacher::facultyId() const {
    return _facultyId;
}

const std::string& Teacher::qualification() const {
    return _qualification;
}  

const std::string& Teacher::specializationSubjects() const {
    return _specializationSubjects;
}

const std::string& Teacher::designation() const {
    return _designation;
}

int Teacher::experienceYears() const {
    return _experienceYears;
}

void Teacher::setFacultyId(const std::string& facultyId) {
    _facultyId = facultyId;
}

void Teacher::setQualification(const std::string& qualification) {
    _qualification = qualification;
}

void Teacher::setSpecializationSubjects(const std::string& specializationSubjects) {
    _specializationSubjects = specializationSubjects;
}

void Teacher::setDesignation(const std::string& designation) {
    _designation = designation;
}

void Teacher::setExperienceYears(int experienceYears) {
    _experienceYears = experienceYears >= 0 ? experienceYears : 0; // Đảm bảo không có số năm kinh nghiệm âm
}

void Teacher::display() const {
    std::cout << std::format(
        "--- Teacher Information ---\n"
        "ID:           {}\n"
        "Full Name:    {}\n"
        "Faculty ID:   {}\n"
        "Designation:  {}\n"
        "Qualification:{}\n"
        "Specializes:  {}\n"
        "Experience:   {} years\n"
        "Citizen ID:   {}\n"
        "Email:        {}\n"
        "Phone:        {}\n",
        id(), fullName(), facultyId(), designation(), qualification(),
        specializationSubjects(), experienceYears(), citizenID(), 
        email(), phoneNumber()
    );
}