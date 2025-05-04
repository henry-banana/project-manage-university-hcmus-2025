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
    CourseResult();
    CourseResult(std::string studentId, std::string courseId, int marks = -1); // -1: Chưa có điểm
    CourseResult(const CourseResult& other); // Copy constructor
    CourseResult& operator=(const CourseResult& other); // Toán tử gán sao chép
    ~CourseResult() = default;

    // Getter & Setter
    const std::string& studentId() const;
    const std::string& courseId() const;
    int marks() const;
    char grade() const; // Logic tính grade dựa trên marks_

    void setMarks(int marks);
    bool isGreded() const; // Kiểm tra xem đã có điểm hay chưa
};

// Hoặc có thể thiết kế Result như một tập hợp điểm của sinh viên trong kỳ
class StudentSemesterResult {
private:
    std::string _studentId;
    int _semester; // Optional
    int _year;
    std::vector<CourseResult> _results;

protected:

public:
    StudentSemesterResult();
    StudentSemesterResult(std::string studentId, int semester, int year);
    StudentSemesterResult(const StudentSemesterResult& other); // Copy constructor
    StudentSemesterResult& operator=(const StudentSemesterResult& other); // Toán tử gán sao chép
    ~StudentSemesterResult() = default;

    // Getter & Setter
    const std::string& studentId() const;
    int semester() const; // Có thể không cần nếu không quản lý theo kỳ
    int year() const;
    const std::vector<CourseResult>& results() const; // Trả về danh sách điểm của sinh viên

    void setSemester(int semester);
    void setYear(int year);
    void addResult(const CourseResult& result); // Thêm điểm cho sinh viên
    void removeResult(const std::string& courseId); // Xóa điểm của môn học cụ thể
    void updateResult(const CourseResult& result); // Cập nhật điểm của môn học cụ thể
};


#endif // RESULT_H