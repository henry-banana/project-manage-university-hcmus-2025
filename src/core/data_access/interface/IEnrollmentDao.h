#ifndef IENROLLMENTDAO_H
#define IENROLLMENTDAO_H

#include <string>
#include <vector>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)

// EnrollmentRecord đã được khai báo trong file header của IEnrollmentDao trước đó, giữ nguyên
struct EnrollmentRecord {
    std::string studentId;
    std::string courseId;
};

class IEnrollmentDao {
public:
    virtual ~IEnrollmentDao() = default;

    virtual std::expected<bool, Error> addEnrollment(const std::string& studentId, const std::string& courseId) = 0;
    virtual std::expected<bool, Error> removeEnrollment(const std::string& studentId, const std::string& courseId) = 0;
    virtual std::expected<bool, Error> removeEnrollmentsByStudent(const std::string& studentId) = 0;
    virtual std::expected<bool, Error> removeEnrollmentsByCourse(const std::string& courseId) = 0;
    virtual std::expected<bool, Error> isEnrolled(const std::string& studentId, const std::string& courseId) const = 0; // Trả về bool (true/false) hoặc Error
    virtual std::expected<std::vector<std::string>, Error> findCourseIdsByStudentId(const std::string& studentId) const = 0;
    virtual std::expected<std::vector<std::string>, Error> findStudentIdsByCourseId(const std::string& courseId) const = 0;
    virtual std::expected<std::vector<EnrollmentRecord>, Error> getAllEnrollments() const = 0;
};

#endif // IENROLLMENTDAO_H