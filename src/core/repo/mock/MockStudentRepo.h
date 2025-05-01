#ifndef MOCKSTUDENTREPO_H
#define MOCKSTUDENTREPO_H

#include "../interface/IStudentRepository.h"
#include <vector>
#include <map>
#include <optional>
#include "../../entities/Student.h"

// Lớp Mock để thử nghiệm mà không cần file thật
class MockStudentRepo : public IStudentRepository {
private:
    std::map<std::string, Student> _students; // Lưu trữ trong bộ nhớ

protected:

public:
    MockStudentRepo(); // Constructor để thêm dữ liệu giả

    std::optional<Student> findById(const std::string& studentId) override;
    std::vector<Student> findAll() override;
    std::vector<Student> findByDepartment(const std::string& departmentId) override;
    bool add(const Student& student) override;
    bool update(const Student& student) override;
    bool remove(const std::string& studentId) override;

    // Hàm helper cho test
    void clearData() { _students.clear(); }
    size_t count() const { return _students.size(); }
};

#endif // MOCKSTUDENTREPO_H