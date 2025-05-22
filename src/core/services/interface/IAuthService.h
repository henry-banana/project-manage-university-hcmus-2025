#ifndef IAUTHSERVICE_H
#define IAUTHSERVICE_H

#include <string>
#include <optional>
#include <memory>
#include <expected> // (➕) Thay thế OperationResult
#include "../../../common/UserRole.h"
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/User.h"

struct StudentRegistrationData {
    std::string email;
    std::string firstName;
    std::string lastName;
    std::string facultyId;
    int birthDay, birthMonth, birthYear;
    std::string address;
    std::string citizenId;
    std::string phoneNumber;
};

class IAuthService {
public:
    virtual ~IAuthService() = default;
    virtual std::expected<std::shared_ptr<User>, Error> login(const std::string& userIdOrEmail, const std::string& password) = 0;
    virtual void logout() = 0;
    virtual bool isAuthenticated() const = 0;
    virtual std::optional<std::string> getCurrentUserId() const = 0;
    virtual std::optional<UserRole> getCurrentUserRole() const = 0;
    virtual std::optional<std::shared_ptr<User>> getCurrentUser() const = 0;
    virtual std::expected<bool, Error> registerStudent(const StudentRegistrationData& data, const std::string& plainPassword) = 0;
    virtual std::expected<bool, Error> changePassword(const std::string& currentUserId, const std::string& oldPassword, const std::string& newPassword) = 0;
};

#endif // IAUTHSERVICE_H