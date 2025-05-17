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

    OperationResult<Student> getById(const std::string& id) const override;
    OperationResult<std::vector<Student>> getAll() const override;
    OperationResult<Student> add(const Student& student) override;
    OperationResult<bool> update(const Student& student) override;
    OperationResult<bool> remove(const std::string& id) override;
    OperationResult<bool> exists(const std::string& id) const override;

    OperationResult<std::vector<Student>> findByFacultyId(const std::string& facultyId) const override;
    OperationResult<Student> findByEmail(const std::string& email) const override;
    OperationResult<std::vector<Student>> findByStatus(LoginStatus status) const override;
    OperationResult<bool> updateStatus(const std::string& studentId, LoginStatus newStatus) override;
};

#endif // MOCK_STUDENT_DAO_H