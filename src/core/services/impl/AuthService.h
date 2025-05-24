#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include "../interface/IAuthService.h" // Đã được cập nhật để dùng std::expected
#include "../../data_access/interface/ILoginDao.h"
#include "../../data_access/interface/IStudentDao.h"
#include "../../data_access/interface/ITeacherDao.h" // (➕) Thêm nếu cần check email teacher
#include "../../../utils/PasswordInput.h"
#include "../../../utils/Logger.h"
#include "../../validators/interface/IValidator.h" // IGeneralInputValidator
#include "../../entities/Student.h"
#include "../../entities/Teacher.h" // (➕)
#include "../SessionContext.h"
#include <memory> // Cho std::shared_ptr

class AuthService : public IAuthService {
private:
    std::shared_ptr<ILoginDao> _loginDao;
    std::shared_ptr<IStudentDao> _studentDao;
    std::shared_ptr<ITeacherDao> _teacherDao; // (➕)
    std::shared_ptr<IGeneralInputValidator> _inputValidator;
    std::shared_ptr<SessionContext> _sessionContext;

public:
    AuthService(std::shared_ptr<ILoginDao> loginDao,
                std::shared_ptr<IStudentDao> studentDao,
                std::shared_ptr<ITeacherDao> teacherDao, // (➕)
                std::shared_ptr<IGeneralInputValidator> inputValidator,
                std::shared_ptr<SessionContext> sessionContext);
    
    ~AuthService() override = default;

    std::expected<std::shared_ptr<User>, Error> login(const std::string& userIdOrEmail, const std::string& password) override;
    void logout() override;
    bool isAuthenticated() const override;
    std::optional<std::string> getCurrentUserId() const override;
    std::optional<UserRole> getCurrentUserRole() const override;
    std::optional<std::shared_ptr<User>> getCurrentUser() const override;
    std::expected<bool, Error> registerStudent(const StudentRegistrationData& data, const std::string& plainPassword) override;
    std::expected<bool, Error> changePassword(const std::string& currentUserId, const std::string& oldPassword, const std::string& newPassword) override;
};

#endif // AUTHSERVICE_H