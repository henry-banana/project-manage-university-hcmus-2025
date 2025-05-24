#ifndef FACULTYSERVICE_H
#define FACULTYSERVICE_H

#include "../interface/IFacultyService.h" // Đã được cập nhật
#include "../../data_access/interface/IFacultyDao.h"
#include "../../data_access/interface/IStudentDao.h"  // (➕) Để check ràng buộc
#include "../../data_access/interface/ITeacherDao.h"  // (➕)
#include "../../data_access/interface/ICourseDao.h"   // (➕)
#include "../../validators/interface/IValidator.h"
#include "../SessionContext.h" // (➕)

class FacultyService : public IFacultyService {
private:
    std::shared_ptr<IFacultyDao> _facultyDao;
    std::shared_ptr<IStudentDao> _studentDao; // (➕)
    std::shared_ptr<ITeacherDao> _teacherDao; // (➕)
    std::shared_ptr<ICourseDao> _courseDao;   // (➕)
    std::shared_ptr<IGeneralInputValidator> _inputValidator;
    std::shared_ptr<SessionContext> _sessionContext; // (➕)

public:
    FacultyService(std::shared_ptr<IFacultyDao> facultyDao,
                   std::shared_ptr<IStudentDao> studentDao, // (➕)
                   std::shared_ptr<ITeacherDao> teacherDao, // (➕)
                   std::shared_ptr<ICourseDao> courseDao,   // (➕)
                   std::shared_ptr<IGeneralInputValidator> inputValidator,
                   std::shared_ptr<SessionContext> sessionContext); // (➕)
    ~FacultyService() override = default;

    std::expected<Faculty, Error> getFacultyById(const std::string& facultyId) const override;
    std::expected<Faculty, Error> getFacultyByName(const std::string& name) const override;
    std::expected<std::vector<Faculty>, Error> getAllFaculties() const override;
    std::expected<Faculty, Error> addFaculty(const std::string& id, const std::string& name) override;
    std::expected<bool, Error> updateFaculty(const std::string& facultyId, const std::string& newName) override;
    std::expected<bool, Error> removeFaculty(const std::string& facultyId) override;
};

#endif // FACULTYSERVICE_H