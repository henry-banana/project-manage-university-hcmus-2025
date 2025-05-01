#ifndef RESULT_H
#define RESULT_H

#include <string>
#include <vector>
#include <map>

// Lớp này có thể đại diện cho điểm của MỘT môn học cho MỘT sinh viên
class CourseResult {
private:
    std::string _studentId;
    std::string _courseId;
    int _marks;
    char _grade; // Tính toán khi cần hoặc khi setMarks

    void calculateGrade(); // Hàm private để tính grade

protected:

public:
    CourseResult(std::string studentId, std::string courseId, int marks = -1); // -1: Chưa có điểm

    const std::string& getStudentId() const { return _studentId; }
    const std::string& getCourseId() const { return _courseId; }
    int getMarks() const { return _marks; }
    char getGrade() const; // Logic tính grade dựa trên marks_

    void setMarks(int marks);
};

// Hoặc có thể thiết kế Result như một tập hợp điểm của sinh viên trong kỳ
class StudentSemesterResult {
private:
    std::string _studentId;
    int _semester; // Optional
    std::vector<CourseResult> _results;

protected:

public:
    StudentSemesterResult(std::string studentId, int semester); // Hoặc không cần semester nếu chỉ quản lý tổng thể

    void addCourseResult(const CourseResult& result);
    const std::vector<CourseResult>& getCourseResults() const;

    // Các phương thức tính toán tổng hợp (có thể đặt ở ResultService)
    // double calculateSGPA(const std::map<std::string, int>& courseCredits) const;
    // int getTotalCreditsEarned(const std::map<std::string, int>& courseCredits) const;
    // int getTotalCreditsAttempted(const std::map<std::string, int>& courseCredits) const;
};


#endif // RESULT_H