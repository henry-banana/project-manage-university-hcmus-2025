#ifndef ENROLLMENTSERVICE_H
#define ENROLLMENTSERVICE_H

#include "../interface/IEnrollmentService.h"
#include "../../data_access/interface/IEnrollmentDao.h"
#include "../../data_access/interface/IStudentDao.h"  // Để kiểm tra Student tồn tại và status
#include "../../data_access/interface/ICourseDao.h"   // Để kiểm tra Course tồn tại
#include "../../validators/interface/IValidator.h"    // GeneralInputValidator
#include "../SessionContext.h"

class EnrollmentService : public IEnrollmentService {
private:
    std::shared_ptr<IEnrollmentDao> _enrollmentDao;
    std::shared_ptr<IStudentDao> _studentDao;
    std::shared_ptr<ICourseDao> _courseDao;
    std::shared_ptr<IGeneralInputValidator> _inputValidator;
    std::shared_ptr<SessionContext> _sessionContext;

public:
    EnrollmentService(std::shared_ptr<IEnrollmentDao> enrollmentDao,
                      std::shared_ptr<IStudentDao> studentDao,
                      std::shared_ptr<ICourseDao> courseDao,
                      std::shared_ptr<IGeneralInputValidator> inputValidator,
                      std::shared_ptr<SessionContext> sessionContext);
    ~EnrollmentService() override = default;

    std::expected<bool, Error> enrollStudentInCourse(const std::string& studentId, const std::string& courseId) override;
    std::expected<bool, Error> dropCourseForStudent(const std::string& studentId, const std::string& courseId) override;
    std::expected<std::vector<Course>, Error> getEnrolledCoursesByStudent(const std::string& studentId) const override;
    std::expected<std::vector<Student>, Error> getEnrolledStudentsByCourse(const std::string& courseId) const override;
    std::expected<bool, Error> isStudentEnrolled(const std::string& studentId, const std::string& courseId) const override;
};

#endif // ENROLLMENTSERVICE_H