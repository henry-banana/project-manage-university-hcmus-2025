// --- START OF MODIFIED FILE src/core/data_access/mock/MockEnrollmentDao.h ---
#ifndef MOCK_ENROLLMENT_DAO_H
#define MOCK_ENROLLMENT_DAO_H

#include "../interface/IEnrollmentDao.h"
#include <vector> // (➕)
#include <string> // (➕)


class MockEnrollmentDao : public IEnrollmentDao {
public:
    MockEnrollmentDao();
    ~MockEnrollmentDao() override = default;
    
    std::expected<bool, Error> addEnrollment(const std::string& studentId, const std::string& courseId) override;
    std::expected<bool, Error> removeEnrollment(const std::string& studentId, const std::string& courseId) override;
    std::expected<bool, Error> removeEnrollmentsByStudent(const std::string& studentId) override;
    std::expected<bool, Error> removeEnrollmentsByCourse(const std::string& courseId) override;
    std::expected<bool, Error> isEnrolled(const std::string& studentId, const std::string& courseId) const override;
    std::expected<std::vector<std::string>, Error> findCourseIdsByStudentId(const std::string& studentId) const override;
    std::expected<std::vector<std::string>, Error> findStudentIdsByCourseId(const std::string& courseId) const override;
    std::expected<std::vector<EnrollmentRecord>, Error> getAllEnrollments() const  override;

    static void initializeDefaultMockData();
    static void clearMockData();
};

#endif // MOCK_ENROLLMENT_DAO_H
// --- END OF MODIFIED FILE src/core/data_access/mock/MockEnrollmentDao.h ---