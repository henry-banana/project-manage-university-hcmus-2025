#ifndef COURSE_H
#define COURSE_H

#include <string>

class Course {
private:
    std::string _id; // Ví dụ: "CE142"
    std::string _name; // Ví dụ: "OOPC"
    int _credits;
    std::string _facultyId; // ID khoa mà môn học thuộc về

protected:

public:
    Course(); // Constructor mặc định
    Course(std::string id, std::string name, int credits, std::string facultyId);
    Course(const Course& other); // Copy constructor
    Course& operator=(const Course& other); // Toán tử gán sao chép
    ~Course() = default; // Destructor ảo để đảm bảo dọn dẹp đúng cách trong kế thừa

    const std::string& id() const;
    const std::string& name() const;
    int credits() const;
    const std::string& facultyId() const;

    void setId(const std::string& id);
    void setName(const std::string& name);
    void setCredits(int credits);
    void setFacultyId(const std::string& facultyId);

    void display() const; // Hiển thị thông tin môn học
};

#endif // COURSE_H