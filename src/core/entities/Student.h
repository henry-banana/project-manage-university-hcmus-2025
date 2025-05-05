#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include <string>
#include <vector> 


class Student : public User {
private:
    std::string _facultyId; // ID khoa
    // Không nên chứa trực tiếp Result hoặc FeeRecord ở đây để tuân thủ SRP
    // Thông tin đó sẽ được quản lý bởi các Service và Repository tương ứng

protected:

public:
    Student();
    Student(std::string id,
        std::string firstName,
        std::string lastName,
        std::string address,
        std::string citizenID,
        std::string email, 
        std::string phoneNumber,
        std::string facultyId);

    Student(const Student& other);
    Student& operator=(const Student& other); // Toán tử gán sao chép
    ~Student() override = default; // Destructor ảo để đảm bảo dọn dẹp đúng cách trong kế thừa
    
    // Getter và Setter cho các thuộc tính riêng của Student
    const std::string& facultyId() const;
    void setFacultyId(const std::string& facultyId);

    void display() const override; // Hiển thị thông tin sinh viên
};

#endif // STUDENT_H