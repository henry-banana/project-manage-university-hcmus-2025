#include "CourseResult.h"
#include <stdexcept> // For std::to_string in toString
#include <sstream>   // For toString
#include "../../utils/StringUtils.h" 

CourseResult::CourseResult(std::string studentId, std::string courseId, int marks)
    : _studentId(std::move(studentId)), _courseId(std::move(courseId)), _marks(-1), _grade('-') {
    setMarks(marks); // Gọi setter để tính grade
}

void CourseResult::calculateGrade() {
    if (_marks < 0) _grade = '-';
    else if (_marks < 40) _grade = 'F';  // Dưới 40 là F
    else if (_marks < 55) _grade = 'D';  // 40-54 là D
    else if (_marks < 70) _grade = 'C';  // 55-69 là C
    else if (_marks < 85) _grade = 'B';  // 70-84 là B
    else if (_marks <= 100) _grade = 'A'; // 85-100 là A
    else _grade = '?'; 
}

const std::string& CourseResult::getStudentId() const { return _studentId; }
const std::string& CourseResult::getCourseId() const { return _courseId; }
int CourseResult::getMarks() const { return _marks; }
char CourseResult::getGrade() const { return _grade; }

bool CourseResult::setMarks(int marks) {
    if (marks < -1 || marks > 100) { // -1 cho phép "chưa nhập điểm"
        // Không set và trả về false nếu điểm không hợp lệ
        return false;
    }
    _marks = marks;
    calculateGrade();
    return true;
}

ValidationResult CourseResult::validate() const {
    ValidationResult vr;
    if (StringUtils::trim(_studentId).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Student ID in CourseResult cannot be empty.");
    if (StringUtils::trim(_courseId).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Course ID in CourseResult cannot be empty.");
    // Kiểm tra lại marks trong validate
    if (_marks < -1 || _marks > 100) { // -1 là cho phép (chưa có điểm)
        vr.addError(ErrorCode::VALIDATION_ERROR, "Marks must be between -1 (not graded) and 100.");
    }
    return vr;
}

std::string CourseResult::toString() const {
    std::ostringstream oss;
    oss << "Student ID: " << _studentId
        << ", Course ID: " << _courseId
        << ", Marks: " << (_marks == -1 ? "N/A" : std::to_string(_marks))
        << ", Grade: " << _grade;
    return oss.str();
}