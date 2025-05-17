#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include <string>

// Forward declaration
class Faculty;

class Student : public User {
private:
    std::string _facultyId; // ID của Khoa mà sinh viên thuộc về

public:
    Student(const std::string& id,
            const std::string& firstName,
            const std::string& lastName,
            const std::string& facultyId, // (➕) Thêm facultyId vào constructor
            LoginStatus status = LoginStatus::PENDING_APPROVAL); // SV mới đăng ký mặc định chờ duyệt

    // Getters & Setters for Student-specific attributes
    const std::string& getFacultyId() const;
    bool setFacultyId(const std::string& facultyId); // Cần validate Faculty ID tồn tại (ở Service)

    // Override
    std::string toString() const override;
    ValidationResult validateBasic() const override;
};

#endif // STUDENT_H