#ifndef ISTUDENTSERVICE_H
#define ISTUDENTSERVICE_H

#include <string>
#include <vector>
#include <memory> // For std::shared_ptr for detailed Student
#include "../../../common/OperationResult.h"
#include "../../entities/Student.h"
#include "../../../common/LoginStatus.h" // Nếu Service cần thao tác với status

struct StudentUpdateData { // (➕) Struct để update, chỉ chứa các trường có thể update
    std::string studentIdToUpdate; // ID không đổi
    std::optional<std::string> firstName;
    std::optional<std::string> lastName;
    std::optional<Birthday> birthday;
    std::optional<std::string> address;
    std::optional<std::string> citizenId;
    std::optional<std::string> email;
    std::optional<std::string> phoneNumber;
    std::optional<std::string> facultyId;
    // Không cho update Role, Status qua đây, phải qua AdminService/AuthService
};


class IStudentService {
public:
    virtual ~IStudentService() = default;

    virtual OperationResult<Student> getStudentDetails(const std::string& studentId) const = 0;
    virtual OperationResult<std::vector<Student>> getAllStudents() const = 0;
    virtual OperationResult<std::vector<Student>> getStudentsByFaculty(const std::string& facultyId) const = 0;
    // Việc addStudent sẽ do AuthService (registerStudent) hoặc AdminService xử lý
    // StudentService chỉ chịu trách nhiệm update thông tin phi xác thực
    virtual OperationResult<bool> updateStudentDetails(const StudentUpdateData& data) = 0;
    // Việc removeStudent cũng nên do AdminService quản lý vì liên quan nhiều hệ thống con
};

#endif // ISTUDENTSERVICE_H