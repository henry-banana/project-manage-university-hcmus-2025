// --- START OF MODIFIED FILE src/core/data_access/mock/MockCourseDao.h ---
#ifndef MOCKCOURSEDAO_H
#define MOCKCOURSEDAO_H

#include "../interface/ICourseDao.h"
#include <map>      // (➕)
#include <string>   // (➕)

class MockCourseDao : public ICourseDao {
public:
    MockCourseDao();
    ~MockCourseDao() override = default;
    std::expected<Course, Error> getById(const std::string& id) const override;
    std::expected<std::vector<Course>, Error> getAll() const override;
    std::expected<Course, Error> add(const Course& course) override;
    std::expected<bool, Error> update(const Course& course) override;
    std::expected<bool, Error> remove(const std::string& id) override;
    std::expected<bool, Error> exists(const std::string& id) const override;
    std::expected<std::vector<Course>, Error> findByFacultyId(const std::string& facultyId) const override;

    static void initializeDefaultMockData();
    static void clearMockData();
};

#endif // MOCKCOURSEDAO_H
// --- END OF MODIFIED FILE src/core/data_access/mock/MockCourseDao.h ---