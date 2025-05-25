#ifndef ITEACHERSERVICE_H
#define ITEACHERSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/Teacher.h"
#include "../../entities/Birthday.h"

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
    virtual std::expected<Teacher, Error> getTeacherDetails(const std::string& teacherId) const = 0;
    virtual std::expected<std::vector<Teacher>, Error> getAllTeachers() const = 0;
    virtual std::expected<std::vector<Teacher>, Error> getTeachersByFaculty(const std::string& facultyId) const = 0;
    virtual std::expected<std::vector<Teacher>, Error> getTeachersByDesignation(const std::string& designation) const = 0;
    virtual std::expected<bool, Error> updateTeacherDetails(const TeacherUpdateData& data) = 0;
};

#endif // ITEACHERSERVICE_H