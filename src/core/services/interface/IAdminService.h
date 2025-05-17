#ifndef IADMINSERVICE_H
#define IADMINSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include "../../../common/OperationResult.h"
#include "../../../common/LoginStatus.h"
#include "../../entities/Student.h"
#include "../../entities/Teacher.h"
#include "IAuthService.h"
// Thêm các entities khác nếu admin cần get/list trực tiếp mà không qua service chuyên biệt.

// (➕) Struct để truyền data khi admin thêm mới SV/GV (bao gồm password ban đầu)
struct NewStudentDataByAdmin {
    StudentRegistrationData studentInfo; // Dùng lại struct từ IAuthService
    std::string initialPassword;
};
struct NewTeacherDataByAdmin {
    // Tương tự StudentRegistrationData, nhưng cho Teacher
    std::string id;
    std::string firstName;
    std::string lastName;
    // ... các trường của Teacher ...
    std::string facultyId;
    std::string initialPassword;
    std::string qualification;
    std::string specializationSubjects;
    std::string designation;
    int experienceYears;
    // thông tin birthday, address, citizenId, email, phoneNumber...
};


class IAdminService {
public:
    virtual ~IAdminService() = default;

    // Student Management (Admin can invoke underlying services or DAOs for views)
    virtual OperationResult<bool> approveStudentRegistration(const std::string& studentIdToApprove) = 0;
    virtual OperationResult<std::vector<Student>> getStudentsByStatus(LoginStatus status) const = 0; // Đặc biệt là PENDING_APPROVAL
    virtual OperationResult<Student> addStudentByAdmin(const NewStudentDataByAdmin& data) = 0; // Wrapper quanh AuthService và StudentService
    virtual OperationResult<bool> removeStudentAccount(const std::string& studentId) = 0; // Xóa Student, Login, Enrollments, Results, Fees

    // Teacher Management
    virtual OperationResult<Teacher> addTeacherByAdmin(const NewTeacherDataByAdmin& data) = 0;
    virtual OperationResult<bool> removeTeacherAccount(const std::string& teacherId) = 0; // Xóa Teacher, Login, Salary

    // User Account Management (chung cho Student, Teacher, Admin khác)
    virtual OperationResult<bool> resetUserPassword(const std::string& userId, const std::string& newPassword) = 0;
    virtual OperationResult<bool> disableUserAccount(const std::string& userId) = 0;
    virtual OperationResult<bool> enableUserAccount(const std::string& userId) = 0;

    // Faculty/Course management là gọi trực tiếp IFacultyService, ICourseService
    // AdminService chỉ là điểm điều phối và thực hiện các tác vụ cần quyền admin cao nhất.
};

#endif // IADMINSERVICE_H