#ifndef MOCK_STUDENT_DAO_H
#define MOCK_STUDENT_DAO_H

#include "../interface/IStudentDao.h"
// (Không cần include Student.h ở đây vì IStudentDao.h đã include rồi)

class MockStudentDao : public IStudentDao {
private:
    // std::vector<Student> _mockData; // có thể chứa data ở đây
public:
    MockStudentDao(); // Constructor có thể khởi tạo _mockData
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
};

#endif // MOCK_STUDENT_DAO_H