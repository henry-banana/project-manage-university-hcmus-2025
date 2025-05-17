#ifndef ITEACHERSERVICE_H
#define ITEACHERSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include "../../../common/OperationResult.h"
#include "../../entities/Teacher.h"
#include "../../entities/Birthday.h" // Nếu dùng Birthday trong struct

struct TeacherUpdateData {
    std::string teacherIdToUpdate;
    std::optional<std::string> firstName;
    std::optional<std::string> lastName;
    std::optional<Birthday> birthday;
    std::optional<std::string> address;
    std::optional<std::string> citizenId;
    std::optional<std::string> email;
    std::optional<std::string> phoneNumber;
    std::optional<std::string> facultyId;
    std::optional<std::string> qualification;
    std::optional<std::string> specializationSubjects;
    std::optional<std::string> designation;
    std::optional<int> experienceYears;
};

class ITeacherService {
public:
    virtual ~ITeacherService() = default;
    virtual OperationResult<Teacher> getTeacherDetails(const std::string& teacherId) const = 0;
    virtual OperationResult<std::vector<Teacher>> getAllTeachers() const = 0;
    virtual OperationResult<std::vector<Teacher>> getTeachersByFaculty(const std::string& facultyId) const = 0;
    virtual OperationResult<std::vector<Teacher>> getTeachersByDesignation(const std::string& designation) const = 0;
    virtual OperationResult<bool> updateTeacherDetails(const TeacherUpdateData& data) = 0;
    // addTeacher, removeTeacher sẽ do AdminService gọi, kết hợp AuthService
};

#endif // ITEACHERSERVICE_H