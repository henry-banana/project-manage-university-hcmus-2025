#ifndef ISTUDENTDAO_H
#define ISTUDENTDAO_H

#include "IDao.h"
#include "../../entities/Student.h" // Cần full definition
#include <string>
#include <vector>

class IStudentDao : public IDao<Student, std::string> {
public:
    virtual ~IStudentDao() override = default;

    // Specific finders for Student
    virtual OperationResult<std::vector<Student>> findByFacultyId(const std::string& facultyId) const = 0;
    virtual OperationResult<Student> findByEmail(const std::string& email) const = 0;
    virtual OperationResult<std::vector<Student>> findByStatus(LoginStatus status) const = 0; // (➕) Để admin duyệt
    virtual OperationResult<bool> updateStatus(const std::string& studentId, LoginStatus newStatus) = 0; // (➕) Để admin duyệt
};

#endif // ISTUDENTDAO_H