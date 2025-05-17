#ifndef IENROLLMENTDAO_H
#define IENROLLMENTDAO_H

#include <string>
#include <vector>
#include "../../../common/OperationResult.h"

struct EnrollmentRecord {
    std::string studentId;
    std::string courseId;
};

class IEnrollmentDao {
public:
    virtual ~IEnrollmentDao() = default;
    virtual OperationResult<bool> addEnrollment(const std::string& studentId, const std::string& courseId) = 0;
    virtual OperationResult<bool> removeEnrollment(const std::string& studentId, const std::string& courseId) = 0;
    virtual OperationResult<bool> removeEnrollmentsByStudent(const std::string& studentId) = 0;
    virtual OperationResult<bool> removeEnrollmentsByCourse(const std::string& courseId) = 0;
    virtual OperationResult<bool> isEnrolled(const std::string& studentId, const std::string& courseId) const = 0;
    virtual OperationResult<std::vector<std::string>> findCourseIdsByStudentId(const std::string& studentId) const = 0;
    virtual OperationResult<std::vector<std::string>> findStudentIdsByCourseId(const std::string& courseId) const = 0;
    virtual OperationResult<std::vector<EnrollmentRecord>> getAllEnrollments() const = 0;
};

#endif // IENROLLMENTDAO_H