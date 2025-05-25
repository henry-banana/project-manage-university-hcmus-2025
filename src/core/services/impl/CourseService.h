#ifndef COURSESERVICE_H
#define COURSESERVICE_H

#include "../interface/ICourseService.h" // Đã được cập nhật
#include "../../data_access/interface/ICourseDao.h"
#include "../../data_access/interface/IFacultyDao.h"
#include "../../data_access/interface/IEnrollmentDao.h" // (➕) Để check ràng buộc
#include "../../data_access/interface/ICourseResultDao.h"// (➕) Để check ràng buộc
#include "../../validators/interface/IValidator.h"
#include "../SessionContext.h" // (➕)

class CourseService : public ICourseService {
private:
    std::shared_ptr<ICourseDao> _courseDao;
    std::shared_ptr<IFacultyDao> _facultyDao;
    std::shared_ptr<IEnrollmentDao> _enrollmentDao; // (➕)
    std::shared_ptr<ICourseResultDao> _courseResultDao; // (➕)
    std::shared_ptr<IGeneralInputValidator> _inputValidator;
    std::shared_ptr<SessionContext> _sessionContext; // (➕)

public:
    CourseService(std::shared_ptr<ICourseDao> courseDao,
                  std::shared_ptr<IFacultyDao> facultyDao,
                  std::shared_ptr<IEnrollmentDao> enrollmentDao, // (➕)
                  std::shared_ptr<ICourseResultDao> courseResultDao, // (➕)
                  std::shared_ptr<IGeneralInputValidator> inputValidator,
                  std::shared_ptr<SessionContext> sessionContext); // (➕)
    ~CourseService() override = default;

    std::expected<Course, Error> getCourseById(const std::string& courseId) const override;
    std::expected<std::vector<Course>, Error> getAllCourses() const override;
    std::expected<std::vector<Course>, Error> getCoursesByFaculty(const std::string& facultyId) const override;
    std::expected<Course, Error> addCourse(const std::string& id, const std::string& name, int credits, const std::string& facultyId) override;
    std::expected<bool, Error> updateCourse(const std::string& courseId, const std::string& newName, int newCredits, const std::string& newFacultyId) override;
    std::expected<bool, Error> removeCourse(const std::string& courseId) override;
};

#endif // COURSESERVICE_H