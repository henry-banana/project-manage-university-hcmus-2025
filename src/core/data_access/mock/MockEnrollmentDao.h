#ifndef MOCK_ENROLLMENT_DAO_H
#define MOCK_ENROLLMENT_DAO_H

#include "../interface/IEnrollmentDao.h"

class MockEnrollmentDao : public IEnrollmentDao {
public:
    MockEnrollmentDao();
    ~MockEnrollmentDao() override = default;
    // (➖) Bỏ MockEnrollmentDao:: ở đây
    std::expected<bool, Error> addEnrollment(const std::string& studentId, const std::string& courseId) override;
    std::expected<bool, Error> removeEnrollment(const std::string& studentId, const std::string& courseId) override;
    std::expected<bool, Error> removeEnrollmentsByStudent(const std::string& studentId) override;
    std::expected<bool, Error> removeEnrollmentsByCourse(const std::string& courseId) override;
    std::expected<bool, Error> isEnrolled(const std::string& studentId, const std::string& courseId) const override;
    std::expected<std::vector<std::string>, Error> findCourseIdsByStudentId(const std::string& studentId) const override;
    std::expected<std::vector<std::string>, Error> findStudentIdsByCourseId(const std::string& courseId) const override;
    std::expected<std::vector<EnrollmentRecord>, Error> getAllEnrollments() const  override;
};

#endif // MOCK_ENROLLMENT_DAO_H