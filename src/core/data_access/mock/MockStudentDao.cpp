#include "MockStudentDao.h"
#include "../../entities/Student.h"
#include "../../../common/ErrorType.h"
#include "../../../common/LoginStatus.h"
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <expected> // (➕)

// Namespace ẩn danh và initializeMockStudentDataIfNeeded giữ nguyên như trước

namespace { // Copy lại phần này từ lần trước
    std::map<std::string, Student> mock_students_data;
    bool mock_student_data_initialized = false;

    void initializeMockStudentDataIfNeeded() {
        if (!mock_student_data_initialized) {
            Student s1("S001", "Alice", "Smith", "IT", LoginStatus::ACTIVE);
            s1.setBirthday(1, 1, 2000); s1.setEmail("alice.smith@example.com"); s1.setCitizenId("001001001");
            mock_students_data[s1.getId()] = s1;

            Student s2("S002", "Bob", "Johnson", "CS", LoginStatus::PENDING_APPROVAL);
            s2.setBirthday(15, 5, 2001); s2.setEmail("bob.j@example.com"); s2.setCitizenId("002002002");
            mock_students_data[s2.getId()] = s2;

            Student s3("S003", "Charlie", "Brown", "IT", LoginStatus::ACTIVE);
            s3.setBirthday(20, 8, 1999); s3.setEmail("charlie.b@example.com"); s3.setCitizenId("003003003");
            mock_students_data[s3.getId()] = s3;

            Student s4("S004", "Diana", "Prince", "LAW", LoginStatus::DISABLED);
            s4.setBirthday(10, 3, 2000); s4.setEmail("diana.p@example.com"); s4.setCitizenId("004004004");
            mock_students_data[s4.getId()] = s4;

            mock_student_data_initialized = true;
        }
    }
}

MockStudentDao::MockStudentDao() {
    initializeMockStudentDataIfNeeded();
}

// IDao methods
std::expected<Student, Error> MockStudentDao::getById(const std::string& id) const {
    auto it = mock_students_data.find(id);
    if (it != mock_students_data.end()) {
        return it->second;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Student with ID " + id + " not found"});
}

std::expected<std::vector<Student>, Error> MockStudentDao::getAll() const {
    std::vector<Student> all_students;
    for (const auto& pair : mock_students_data) {
        all_students.push_back(pair.second);
    }
    return all_students; // Luôn thành công trong mock, trả về vector (có thể rỗng)
}

std::expected<Student, Error> MockStudentDao::add(const Student& student) {
    if (mock_students_data.count(student.getId())) {
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Mock Student with ID " + student.getId() + " already exists"});
    }
    mock_students_data[student.getId()] = student;
    return student;
}

std::expected<bool, Error> MockStudentDao::update(const Student& student) {
    auto it = mock_students_data.find(student.getId());
    if (it != mock_students_data.end()) {
        it->second = student;
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Student with ID " + student.getId() + " not found for update"});
}

std::expected<bool, Error> MockStudentDao::remove(const std::string& id) {
    if (mock_students_data.erase(id) > 0) {
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Student with ID " + id + " not found for removal"});
}

std::expected<bool, Error> MockStudentDao::exists(const std::string& id) const {
    return mock_students_data.count(id) > 0; // Mock này đơn giản là trả về bool
}

// IStudentDao specific methods
std::expected<std::vector<Student>, Error> MockStudentDao::findByFacultyId(const std::string& facultyId) const {
    std::vector<Student> result;
    for (const auto& pair : mock_students_data) {
        if (pair.second.getFacultyId() == facultyId) {
            result.push_back(pair.second);
        }
    }
    return result; // Trả về vector (có thể rỗng)
}

std::expected<Student, Error> MockStudentDao::findByEmail(const std::string& email) const {
    for (const auto& pair : mock_students_data) {
        if (pair.second.getEmail() == email) {
            return pair.second;
        }
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Student with email " + email + " not found"});
}

std::expected<std::vector<Student>, Error> MockStudentDao::findByStatus(LoginStatus status) const {
    std::vector<Student> result;
    for (const auto& pair : mock_students_data) {
        if (pair.second.getStatus() == status) {
            result.push_back(pair.second);
        }
    }
    return result; // Trả về vector (có thể rỗng)
}

std::expected<bool, Error> MockStudentDao::updateStatus(const std::string& studentId, LoginStatus newStatus) {
    auto it = mock_students_data.find(studentId);
    if (it != mock_students_data.end()) {
        it->second.setStatus(newStatus);
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Student with ID " + studentId + " not found for status update"});
}