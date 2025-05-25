#ifndef IENROLLMENTSERVICE_H
#define IENROLLMENTSERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/Course.h"
#include "../../entities/Student.h"

class IEnrollmentService {
public:
    virtual ~IEnrollmentService() = default;
    virtual std::expected<bool, Error> enrollStudentInCourse(const std::string& studentId, const std::string& courseId) = 0;
    virtual std::expected<bool, Error> dropCourseForStudent(const std::string& studentId, const std::string& courseId) = 0;
    virtual std::expected<std::vector<Course>, Error> getEnrolledCoursesByStudent(const std::string& studentId) const = 0;
    virtual std::expected<std::vector<Student>, Error> getEnrolledStudentsByCourse(const std::string& courseId) const = 0;
    virtual std::expected<bool, Error> isStudentEnrolled(const std::string& studentId, const std::string& courseId) const = 0;
};

#endif // IENROLLMENTSERVICE_H