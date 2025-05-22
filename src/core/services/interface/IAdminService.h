#ifndef IADMINSERVICE_H
#define IADMINSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../../common/LoginStatus.h"
#include "../../entities/Student.h"
#include "../../entities/Teacher.h"
#include "IAuthService.h" // Để dùng StudentRegistrationData

struct NewStudentDataByAdmin {
    StudentRegistrationData studentInfo;
    std::string initialPassword;
};
struct NewTeacherDataByAdmin {
    std::string id;
    std::string firstName;
    std::string lastName;
    std::string facultyId;
    std::string initialPassword;
    std::string qualification;
    std::string specializationSubjects;
    std::string designation;
    int experienceYears;
    // Thêm các trường thông tin cá nhân khác từ User nếu cần thiết cho việc tạo mới qua admin
    int birthDay, birthMonth, birthYear;
    std::string address;
    std::string citizenId;
    std::string email;       // Bắt buộc
    std::string phoneNumber;
};

class IAdminService {
public:
    virtual ~IAdminService() = default;

    virtual std::expected<bool, Error> approveStudentRegistration(const std::string& studentIdToApprove) = 0;
    virtual std::expected<std::vector<Student>, Error> getStudentsByStatus(LoginStatus status) const = 0;
    virtual std::expected<Student, Error> addStudentByAdmin(const NewStudentDataByAdmin& data) = 0;
    virtual std::expected<bool, Error> removeStudentAccount(const std::string& studentId) = 0;

    virtual std::expected<Teacher, Error> addTeacherByAdmin(const NewTeacherDataByAdmin& data) = 0;
    virtual std::expected<bool, Error> removeTeacherAccount(const std::string& teacherId) = 0;

    virtual std::expected<bool, Error> resetUserPassword(const std::string& userId, const std::string& newPassword) = 0;
    virtual std::expected<bool, Error> disableUserAccount(const std::string& userId) = 0;
    virtual std::expected<bool, Error> enableUserAccount(const std::string& userId) = 0;
};

#endif // IADMINSERVICE_H