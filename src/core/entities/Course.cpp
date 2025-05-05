#include "Course.h"
#include <iostream>
#include <format>

Course::Course(std::string id, std::string name, int credits, std::string facultyId)
    : _id(std::move(id)),
      _name(std::move(name)),
      _credits(credits > 0 ? credits : 0), // Basic validation
      _facultyId(std::move(facultyId))
{}

Course::Course() 
    : _id(""),
      _name(""),
      _credits(0),
      _facultyId("") // Default constructor
{}

Course::Course(const Course& other) // Copy constructor
    : _id(other._id),
      _name(other._name),
      _credits(other._credits),
      _facultyId(other._facultyId)
{}

Course& Course::operator=(const Course& other) { // Copy assignment operator
    if (this != &other) { // Self-assignment check
        _id = other._id;
        _name = other._name;
        _credits = other._credits;
        _facultyId = other._facultyId;
    }
    
    return *this;
}

// Getters
const std::string& Course::id() const { return _id; }
const std::string& Course::name() const { return _name; }
int Course::credits() const { return _credits; }
const std::string& Course::facultyId() const { return _facultyId; }

// Setters
void Course::setId(const std::string& id) { _id = id; }
void Course::setName(const std::string& name) { _name = name; }
void Course::setCredits(int credits) { _credits = (credits >= 0) ? credits : 0; }
void Course::setFacultyId(const std::string& facultyId) { _facultyId = facultyId; }

// Display
void Course::display() const {
    std::cout << std::format(
        "--- Course Details ---\n"
        "ID:         {}\n"
        "Name:       {}\n"
        "Credits:    {}\n"
        "Faculty ID: {}\n"
        "----------------------\n",
        _id, _name, _credits, _facultyId
    );
}