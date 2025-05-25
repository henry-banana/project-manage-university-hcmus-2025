#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include "../interface/IAdminService.h"
#include "../../data_access/interface/IStudentDao.h"
#include "../../data_access/interface/ITeacherDao.h"
#include "../../data_access/interface/ILoginDao.h"
#include "../../data_access/interface/IFeeRecordDao.h"      // Để xóa khi remove student
#include "../../data_access/interface/ISalaryRecordDao.h"   // Để xóa khi remove teacher
#include "../../data_access/interface/IEnrollmentDao.h"   // Để xóa khi remove student
#include "../../data_access/interface/ICourseResultDao.h" // Để xóa khi remove student
#include "../../validators/interface/IValidator.h"
#include "../SessionContext.h"
#include "../../../utils/PasswordInput.h" // Để hash password mới

class AdminService : public IAdminService {
private:
    std::shared_ptr<IStudentDao> _studentDao;
    std::shared_ptr<ITeacherDao> _teacherDao;
    std::shared_ptr<ILoginDao> _loginDao;
    std::shared_ptr<IFeeRecordDao> _feeDao;
    std::shared_ptr<ISalaryRecordDao> _salaryDao;
    std::shared_ptr<IEnrollmentDao> _enrollmentDao;
    std::shared_ptr<ICourseResultDao> _courseResultDao;
    std::shared_ptr<IGeneralInputValidator> _inputValidator;
    std::shared_ptr<SessionContext> _sessionContext;
    // Có thể inject IAuthService để dùng lại logic register (tạo User + Login)
    // std::shared_ptr<IAuthService> _authServiceForRegistration; 

public:
    AdminService(std::shared_ptr<IStudentDao> studentDao,
                 std::shared_ptr<ITeacherDao> teacherDao,
                 std::shared_ptr<ILoginDao> loginDao,
                 std::shared_ptr<IFeeRecordDao> feeDao,
                 std::shared_ptr<ISalaryRecordDao> salaryDao,
                 std::shared_ptr<IEnrollmentDao> enrollmentDao,
                 std::shared_ptr<ICourseResultDao> courseResultDao,
                 std::shared_ptr<IGeneralInputValidator> inputValidator,
                 std::shared_ptr<SessionContext> sessionContext);
    ~AdminService() override = default;

    std::expected<bool, Error> approveStudentRegistration(const std::string& studentIdToApprove) override;
    std::expected<std::vector<Student>, Error> getStudentsByStatus(LoginStatus status) const override;
    std::expected<Student, Error> addStudentByAdmin(const NewStudentDataByAdmin& data) override;
    std::expected<bool, Error> removeStudentAccount(const std::string& studentId) override;

    std::expected<Teacher, Error> addTeacherByAdmin(const NewTeacherDataByAdmin& data) override;
    std::expected<bool, Error> removeTeacherAccount(const std::string& teacherId) override;

    std::expected<bool, Error> resetUserPassword(const std::string& userId, const std::string& newPassword) override;
    std::expected<bool, Error> disableUserAccount(const std::string& userId) override;
    std::expected<bool, Error> enableUserAccount(const std::string& userId) override;
    bool isAdminAuthenticated() const;
};

#endif // ADMINSERVICE_H