// --- START OF MODIFIED FILE src/core/data_access/mock/MockStudentDao.h ---
#ifndef MOCK_STUDENT_DAO_H
#define MOCK_STUDENT_DAO_H

#include "../interface/IStudentDao.h"
#include <map> // (➕)
#include <string> // (➕)

class MockStudentDao : public IStudentDao {
public:
    MockStudentDao();
    ~MockStudentDao() override = default;

    std::expected<Student, Error> getById(const std::string& id) const override;
    std::expected<std::vector<Student>, Error> getAll() const override;
    std::expected<Student, Error> add(const Student& student) override;
    std::expected<bool, Error> update(const Student& student) override;
    std::expected<bool, Error> remove(const std::string& id) override;
    std::expected<bool, Error> exists(const std::string& id) const override;

    std::expected<std::vector<Student>, Error> findByFacultyId(const std::string& facultyId) const override;
    std::expected<Student, Error> findByEmail(const std::string& email) const override;
    std::expected<std::vector<Student>, Error> findByStatus(LoginStatus status) const override;
    std::expected<bool, Error> updateStatus(const std::string& studentId, LoginStatus newStatus) override;

    static void initializeDefaultMockData();
    static void clearMockData();
};

#endif // MOCK_STUDENT_DAO_H
// --- END OF MODIFIED FILE src/core/data_access/mock/MockStudentDao.h ---