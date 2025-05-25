#ifndef COURSERESULT_H
#define COURSERESULT_H

#include <string>
#include "../../common/ValidationResult.h"

class CourseResult {
private:
    std::string _studentId;
    std::string _courseId;
    int _marks;   // Điểm số (từ -1 đến 100, -1 là chưa có điểm)
    char _grade;  // Xếp loại (A, B, C, D, F, - (chưa xếp loại))

    void calculateGrade(); // Helper private để tính grade từ marks

public:
    CourseResult(std::string studentId, std::string courseId, int marks = -1);

    // Getters
    const std::string& getStudentId() const;
    const std::string& getCourseId() const;
    int getMarks() const;
    char getGrade() const;

    // Setters
    bool setMarks(int marks); // Recalculate grade khi set marks

    ValidationResult validate() const; // Marks phải trong khoảng hợp lệ
    std::string toString() const; // Để debug
};

#endif // COURSERESULT_H