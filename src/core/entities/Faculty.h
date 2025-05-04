#ifndef FACULTY_H
#define FACULTY_H

#include "User.h"
#include "Teacher.h"
#include <string>
#include <vector> // Có thể cần nếu Faculty dạy nhiều Course


#include <string>

class Faculty {
private:
    std::string _id; // Ví dụ: "CE", "IT"
    std::string _name; // Ví dụ: "Computer Engineering"
protected:

public:
    Faculty();
    // Rule of Three
    Faculty(std::string id, std::string name);
    Faculty(const Faculty& other); // Copy constructor
    Faculty& operator=(const Faculty& other); // Toán tử gán sao chép
    ~Faculty() = default; // Destructor ảo để đảm bảo dọn dẹp đúng cách trong kế thừa
    
    // Getter & Setter
    const std::string& id() const;
    const std::string& name() const;

    void setId(const std::string& id);
    void setName(const std::string& name);

    // Phương thức hiển thị thông tin khoa
    void display() const; // Hiển thị thông tin khoa
};

#endif // FACULTY_H