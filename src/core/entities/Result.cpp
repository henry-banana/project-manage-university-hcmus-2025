#include "Result.h"
#include <algorithm> // For std::find_if, std::remove_if
#include <iostream>
#include <format>

// --- CourseResult Implementation ---

CourseResult::CourseResult(std::string studentId, std::string courseId, int marks)
    : _studentId(std::move(studentId)),
      _courseId(std::move(courseId)),
      _marks(-1), // Initialize explicitly
      _grade('-')
{
    setMarks(marks); // Use setter for validation and grade calculation
}

CourseResult::CourseResult() 
    : _studentId(""),
      _courseId(""),
      _marks(-1), // Initialize explicitly
      _grade('-')
{}

CourseResult::CourseResult(const CourseResult& other)
    : _studentId(other._studentId),
      _courseId(other._courseId),
      _marks(other._marks),
      _grade(other._grade)
{}

CourseResult& CourseResult::operator=(const CourseResult& other) {
    if (this != &other) { // Self-assignment check
        _studentId = other._studentId;
        _courseId = other._courseId;
        _marks = other._marks;
        _grade = other._grade;
    }
    return *this; // Return the current object
}

void CourseResult::calculateGrade() {
    if (_marks < 0) {
        _grade = '-'; // Not graded
    } else if (_marks > 100) {
        _grade = '?'; // Invalid marks > 100
    } else if (_marks >= 81) {
        _grade = 'A';
    } else if (_marks >= 66) {
        _grade = 'B';
    } else if (_marks >= 51) {
        _grade = 'C';
    } else if (_marks >= 46) {
        _grade = 'D';
    } else if (_marks >= 36) {
        _grade = 'E';
    } else { // 0-35
        _grade = 'F';
    }
}

// Getters
const std::string& CourseResult::studentId() const { return _studentId; }
const std::string& CourseResult::courseId() const { return _courseId; }
int CourseResult::marks() const { return _marks; }
char CourseResult::grade() const { return _grade; }

// Setter
bool CourseResult::setMarks(int marks) {
    if (marks >= -1 && marks <= 100) { // Allow -1 for "not graded"
        _marks = marks;
        calculateGrade(); // Update grade whenever marks change
        return true;
    }
    // Invalid marks
    _marks = -1; // Or keep old value? Resetting seems safer.
    _grade = '?'; // Indicate error or keep '-'? '?' might be clearer.
    return false;
}

bool CourseResult::isGraded() const {
    return _marks != -1;
}


// --- StudentSemesterResult Implementation ---

StudentSemesterResult::StudentSemesterResult() 
    : _studentId(""),
      _semester(0),
      _year(0) // Initialize to 0 or some default value
{}

StudentSemesterResult::StudentSemesterResult(std::string studentId, int semester, int year) 
    : _studentId(std::move(studentId)),
      _semester(semester),
      _year(year) // Initialize to provided values
{}

StudentSemesterResult::StudentSemesterResult(const StudentSemesterResult& other) 
    : _studentId(other._studentId),
      _semester(other._semester),
      _year(other._year),
      _results(other._results) // Copy the vector of results
{}

StudentSemesterResult& StudentSemesterResult::operator=(const StudentSemesterResult& other) {
    if (this != &other) { // Self-assignment check
        _studentId = other._studentId;
        _semester = other._semester;
        _year = other._year;
        _results = other._results; // Copy the vector of results
    }
    return *this; // Return the current object
}

// Getters
const std::string& StudentSemesterResult::studentId() const { return _studentId; }
int StudentSemesterResult::semester() const { return _semester; }
const std::vector<CourseResult>& StudentSemesterResult::results() const {
    return _results;
}

// Modifiers
void StudentSemesterResult::addResult(const CourseResult& result) {
    // Basic check: Ensure result belongs to the same student
    if (result.studentId() != _studentId) {
         throw std::invalid_argument("Cannot add result for a different student.");
         // Or just log and return
         // std::cerr << "Error: Mismatched student ID in addCourseResult." << std::endl;
         // return;
    }

    // Check if result for this course already exists, update if so? Or disallow?
    // For simplicity, let's allow duplicates for now, or replace existing.
    // Replacing seems more logical for results.
    auto it = std::find_if(_results.begin(), _results.end(),
                           [&](const CourseResult& cr) { return cr.courseId() == result.courseId(); });

    if (it != _results.end()) {
        *it = result; // Replace existing result
    } else {
        _results.push_back(result); // Add new result
    }
}

bool StudentSemesterResult::updateResult(const CourseResult& updatedResult) {
     if (updatedResult.studentId() != _studentId) {
         // Log error or throw
         return false;
    }
     auto it = std::find_if(_results.begin(), _results.end(),
                           [&](const CourseResult& cr) { return cr.courseId() == updatedResult.courseId(); });

    if (it != _results.end()) {
        *it = updatedResult; // Update the found result
        return true;
    }
    return false; // Result for the course not found
}


bool StudentSemesterResult::removeResult(const std::string& courseId) {
     auto it = std::remove_if(_results.begin(), _results.end(),
                             [&](const CourseResult& cr) { return cr.courseId() == courseId; });

    if (it != _results.end()) {
        _results.erase(it, _results.end()); // Erase the removed elements
        return true;
    }
    return false; // Course not found
}

// Find specific result
std::optional<CourseResult> StudentSemesterResult::findResultForCourse(const std::string& courseId) const {
    auto it = std::find_if(_results.begin(), _results.end(),
                           [&](const CourseResult& cr) { return cr.courseId() == courseId; });

    if (it != _results.end()) {
        return *it; // Return the found result
    }
    return std::nullopt; // Not found
}

void StudentSemesterResult::setSemester(int semester) {
    if (semester >= 1 && semester <= 3) { // Assuming 1: Fall, 2: Spring
        _semester = semester;
    } else {
        throw std::invalid_argument("Invalid semester value. Must be 1 or 2.");
    }
}

void StudentSemesterResult::setYear(int year) {
    if (year > 0) {
        _year = year;
    } else {
        throw std::invalid_argument("Invalid year value. Must be greater than 2000.");
    }
}