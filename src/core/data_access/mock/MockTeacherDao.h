#ifndef MOCK_TEACHER_DAO_H
#define MOCK_TEACHER_DAO_H

#include "../interface/ITeacherDao.h"
// (Không cần include Teacher.h ở đây vì ITeacherDao.h đã include rồi)

class MockTeacherDao : public ITeacherDao {
private:
    // std::vector<Teacher> _mockData; // có thể chứa data ở đây
public:
    MockTeacherDao(); // Constructor có thể khởi tạo _mockData
    ~MockTeacherDao() override = default;

    std::expected<Teacher, Error> getById(const std::string& id) const override;
    std::expected<std::vector<Teacher>, Error> getAll() const override;
    std::expected<Teacher, Error> add(const Teacher& Teacher) override;
    std::expected<bool, Error> update(const Teacher& Teacher) override;
    std::expected<bool, Error> remove(const std::string& id) override;
    std::expected<bool, Error> exists(const std::string& id) const override;

    std::expected<std::vector<Teacher>, Error> findByFacultyId(const std::string& facultyId) const override;
    std::expected<std::vector<Teacher>, Error> findByDesignation(const std::string& designation) const override;
    std::expected<Teacher, Error> findByEmail(const std::string& email) const override;
};

#endif // MOCK_TEACHER_DAO_H