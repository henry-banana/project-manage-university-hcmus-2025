#ifndef MOCK_FACULTY_DAO_H
#define MOCK_FACULTY_DAO_H

#include "../interface/IFacultyDao.h"

class MockFacultyDao : public IFacultyDao {
public:
    MockFacultyDao();
    ~MockFacultyDao() override = default;
    
    std::expected<Faculty, Error> getById(const std::string& id) const override;
    std::expected<std::vector<Faculty>, Error> getAll() const override;
    std::expected<Faculty, Error> add(const Faculty& faculty) override;
    std::expected<bool, Error> update(const Faculty& faculty) override;
    std::expected<bool, Error> remove(const std::string& id) override;
    std::expected<bool, Error> exists(const std::string& id) const override;
    std::expected<Faculty, Error> findByName(const std::string& name) const override;

    static void resetMockData();
};
#endif // MOCK_FACULTY_DAO_H