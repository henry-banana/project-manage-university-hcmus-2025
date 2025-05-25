#ifndef TEACHERSERVICE_H
#define TEACHERSERVICE_H

#include "../interface/ITeacherService.h" // Đã được cập nhật
#include "../../data_access/interface/ITeacherDao.h"
#include "../../data_access/interface/IStudentDao.h" // (➕) Để check email student
#include "../../data_access/interface/IFacultyDao.h"
#include "../../validators/interface/IValidator.h"
#include "../SessionContext.h" // (➕)

class TeacherService : public ITeacherService {
private:
    std::shared_ptr<ITeacherDao> _teacherDao;
    std::shared_ptr<IStudentDao> _studentDao; // (➕)
    std::shared_ptr<IFacultyDao> _facultyDao;
    std::shared_ptr<IGeneralInputValidator> _inputValidator;
    std::shared_ptr<SessionContext> _sessionContext; // (➕)

public:
    TeacherService(std::shared_ptr<ITeacherDao> teacherDao,
                   std::shared_ptr<IStudentDao> studentDao, // (➕)
                   std::shared_ptr<IFacultyDao> facultyDao,
                   std::shared_ptr<IGeneralInputValidator> inputValidator,
                   std::shared_ptr<SessionContext> sessionContext); // (➕)
    ~TeacherService() override = default;

    std::expected<Teacher, Error> getTeacherDetails(const std::string& teacherId) const override;
    std::expected<std::vector<Teacher>, Error> getAllTeachers() const override;
    std::expected<std::vector<Teacher>, Error> getTeachersByFaculty(const std::string& facultyId) const override;
    std::expected<std::vector<Teacher>, Error> getTeachersByDesignation(const std::string& designation) const override;
    std::expected<bool, Error> updateTeacherDetails(const TeacherUpdateData& data) override;
};

#endif // TEACHERSERVICE_H