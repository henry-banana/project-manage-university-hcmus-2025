#ifndef STUDENTSERVICE_H
#define STUDENTSERVICE_H

#include "../interface/IStudentService.h" // Đã được cập nhật
#include "../../data_access/interface/ITeacherDao.h" // (➕) Thêm TeacherDao
#include "../../data_access/interface/IStudentDao.h"
#include "../../data_access/interface/IFacultyDao.h"
#include "../../validators/interface/IValidator.h"
#include "../SessionContext.h" // (Optional)

class StudentService : public IStudentService {
private:
    std::shared_ptr<IStudentDao> _studentDao;
    std::shared_ptr<ITeacherDao> _teacherDao;
    std::shared_ptr<IFacultyDao> _facultyDao;
    std::shared_ptr<IGeneralInputValidator> _inputValidator;
    std::shared_ptr<SessionContext> _sessionContext; // (➕) Thêm để check quyền

public:
    StudentService(std::shared_ptr<IStudentDao> studentDao,
                   std::shared_ptr<ITeacherDao> teacherDao,
                   std::shared_ptr<IFacultyDao> facultyDao,
                   std::shared_ptr<IGeneralInputValidator> inputValidator,
                   std::shared_ptr<SessionContext> sessionContext); // (➕)
    ~StudentService() override = default;

    std::expected<Student, Error> getStudentDetails(const std::string& studentId) const override;
    std::expected<std::vector<Student>, Error> getAllStudents() const override;
    std::expected<std::vector<Student>, Error> getStudentsByFaculty(const std::string& facultyId) const override;
    std::expected<bool, Error> updateStudentDetails(const StudentUpdateData& data) override;
};

#endif // STUDENTSERVICE_H