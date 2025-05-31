// --- START OF MODIFIED FILE src/core/data_access/mock/MockTeacherDao.h ---
#ifndef MOCK_TEACHER_DAO_H
#define MOCK_TEACHER_DAO_H

#include "../interface/ITeacherDao.h"
#include <map>      // (➕)
#include <string>   // (➕)

class MockTeacherDao : public ITeacherDao {
public:
    MockTeacherDao();
    ~MockTeacherDao() override = default;

    std::expected<Teacher, Error> getById(const std::string& id) const override;
    std::expected<std::vector<Teacher>, Error> getAll() const override;
    std::expected<Teacher, Error> add(const Teacher& teacher) override; // Sửa tên tham số
    std::expected<bool, Error> update(const Teacher& teacher) override; // Sửa tên tham số
    std::expected<bool, Error> remove(const std::string& id) override;
    std::expected<bool, Error> exists(const std::string& id) const override;

    std::expected<std::vector<Teacher>, Error> findByFacultyId(const std::string& facultyId) const override;
    std::expected<std::vector<Teacher>, Error> findByDesignation(const std::string& designation) const override;
    std::expected<Teacher, Error> findByEmail(const std::string& email) const override;

    static void initializeDefaultMockData();
    static void clearMockData();
};

#endif // MOCK_TEACHER_DAO_H
// --- END OF MODIFIED FILE src/core/data_access/mock/MockTeacherDao.h ---