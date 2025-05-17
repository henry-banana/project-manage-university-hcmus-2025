#ifndef IENROLLMENTSERVICE_H
#define IENROLLMENTSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include "../../../common/OperationResult.h"
#include "../../entities/Course.h" // Để trả về danh sách Course
#include "../../entities/Student.h"// Để trả về danh sách Student

class IEnrollmentService {
public:
    virtual ~IEnrollmentService() = default;
    virtual OperationResult<bool> enrollStudentInCourse(const std::string& studentId, const std::string& courseId) = 0;
    virtual OperationResult<bool> dropCourseForStudent(const std::string& studentId, const std::string& courseId) = 0;
    virtual OperationResult<std::vector<Course>> getEnrolledCoursesByStudent(const std::string& studentId) const = 0;
    virtual OperationResult<std::vector<Student>> getEnrolledStudentsByCourse(const std::string& courseId) const = 0;
    virtual OperationResult<bool> isStudentEnrolled(const std::string& studentId, const std::string& courseId) const = 0;
};

#endif // IENROLLMENTSERVICE_H