#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include <string>
#include <vector> // Có thể cần nếu Student chứa danh sách Course đã đăng ký

// Forward declaration để tránh include vòng tròn nếu cần
class Department;
class Result;
class FeeRecord;

class Student : public User {
private:
    std::string _departmentId; // ID tham chiếu đến Department
    std::string _institute;
    std::string _admissionQuota;
    // Không nên chứa trực tiếp Result hoặc FeeRecord ở đây để tuân thủ SRP
    // Thông tin đó sẽ được quản lý bởi các Service và Repository tương ứng

protected:

public:
    Student(std::string id, std::string firstName, std::string lastName,
            std::string departmentId, std::string institute,
            std::string admissionQuota);

    // Getters cho các thuộc tính riêng của Student
    const std::string& getDepartmentId() const { return _departmentId; }
    const std::string& getInstitute() const { return _institute; }
    const std::string& getAdmissionQuota() const { return _admissionQuota; }

    // Setters (nếu cần)
    void setDepartmentId(std::string departmentId) { _departmentId = std::move(departmentId); }
    // ... các setters khác
};

#endif // STUDENT_H