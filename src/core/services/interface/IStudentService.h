#ifndef ISTUDENTSERVICE_H
#define ISTUDENTSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/Student.h"
#include "../../entities/Birthday.h" // (➕) Cần cho StudentUpdateData
#include "../../../common/LoginStatus.h"

struct StudentUpdateData {
    std::string studentIdToUpdate;
    std::optional<std::string> firstName;
    std::optional<std::string> lastName;
    std::optional<Birthday> birthday;
    std::optional<std::string> address;
    std::optional<std::string> citizenId;
    std::optional<std::string> email;
    std::optional<std::string> phoneNumber;
    std::optional<std::string> facultyId;
};

class IStudentService {
public:
    virtual ~IStudentService() = default;

    virtual std::expected<Student, Error> getStudentDetails(const std::string& studentId) const = 0;
    virtual std::expected<std::vector<Student>, Error> getAllStudents() const = 0;
    virtual std::expected<std::vector<Student>, Error> getStudentsByFaculty(const std::string& facultyId) const = 0;
    virtual std::expected<bool, Error> updateStudentDetails(const StudentUpdateData& data) = 0;
};

#endif // ISTUDENTSERVICE_H