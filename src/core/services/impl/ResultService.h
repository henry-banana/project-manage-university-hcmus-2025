#ifndef RESULTSERVICE_H
#define RESULTSERVICE_H

#include "../interface/IResultService.h"
#include "../../data_access/interface/ICourseResultDao.h"
#include "../../data_access/interface/IStudentDao.h"
#include "../../data_access/interface/ICourseDao.h"
#include "../../data_access/interface/IFacultyDao.h"
#include "../../data_access/interface/IEnrollmentDao.h" // Để kiểm tra SV có đăng ký môn đó không
#include "../../validators/interface/IValidator.h"
#include "../SessionContext.h"
#include <iomanip> // For setprecision in report

class ResultService : public IResultService {
private:
    std::shared_ptr<ICourseResultDao> _resultDao;
    std::shared_ptr<IFacultyDao> _facultyDao;
    std::shared_ptr<IStudentDao> _studentDao;
    std::shared_ptr<ICourseDao> _courseDao;
    std::shared_ptr<IEnrollmentDao> _enrollmentDao;
    std::shared_ptr<IGeneralInputValidator> _inputValidator;
    std::shared_ptr<SessionContext> _sessionContext;

public:
    ResultService(std::shared_ptr<ICourseResultDao> resultDao,
                    std::shared_ptr<IFacultyDao> _facultyDao,
                  std::shared_ptr<IStudentDao> studentDao,
                  std::shared_ptr<ICourseDao> courseDao,
                  std::shared_ptr<IEnrollmentDao> enrollmentDao,
                  std::shared_ptr<IGeneralInputValidator> inputValidator,
                  std::shared_ptr<SessionContext> sessionContext);
    ~ResultService() override = default;

    std::expected<bool, Error> enterMarks(const std::string& studentId, const std::string& courseId, int marks) override;
    std::expected<bool, Error> enterMultipleMarks(const std::string& studentId, const std::map<std::string, int>& courseMarksMap) override;
    std::expected<CourseResult, Error> getSpecificResult(const std::string& studentId, const std::string& courseId) const override;
    std::expected<std::vector<CourseResult>, Error> getResultsByStudent(const std::string& studentId) const override;
    std::expected<std::vector<CourseResult>, Error> getResultsByCourse(const std::string& courseId) const override;
    std::expected<std::string, Error> generateStudentResultReport(const std::string& studentId) const override;
    std::expected<double, Error> calculateCGPA(const std::string& studentId) const override;
};

#endif // RESULTSERVICE_H