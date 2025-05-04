#ifndef FACULTY_H
#define FACULTY_H

#include "User.h"
#include "Teacher.h"
#include <string>
#include <vector> // Có thể cần nếu Faculty dạy nhiều Course


#include <string>

class Teacher; // Forward declaration

class Faculty {
private:
    std::string _id; // Ví dụ: "CE", "IT"
    std::string _name; // Ví dụ: "Computer Engineering"
protected:

public:
    Faculty(std::string id, std::string name);


    // Getter & Setter
    const std::string& id() const;
    const std::string& name() const;

    void setId(const std::string& id);
    void setName(const std::string& name);
};

#endif // FACULTY_H