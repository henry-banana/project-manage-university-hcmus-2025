#include "MockStudentDao.h"
#include "../../entities/Student.h"
#include "../../../common/ErrorType.h"
#include "../../../common/LoginStatus.h"
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <expected> 

namespace { 
    std::map<std::string, Student> mock_students_data;
    bool mock_student_data_initialized_flag = false; // Đổi tên cờ để tránh nhầm lẫn
}

void MockStudentDao::initializeDefaultMockData() {
    if (!mock_student_data_initialized_flag) {
        mock_students_data.clear(); // Đảm bảo sạch
        Student s1("S001", "Alice", "Smith", "IT", LoginStatus::ACTIVE);
        s1.setBirthday(1, 1, 2000); s1.setEmail("alice.smith@example.com"); s1.setCitizenId("001001001");
        mock_students_data.emplace(s1.getId(), s1);

        Student s2("S002", "Bob", "Johnson", "CS", LoginStatus::PENDING_APPROVAL);
        s2.setBirthday(15, 5, 2001); s2.setEmail("bob.j@example.com"); s2.setCitizenId("002002002");
        mock_students_data.emplace(s2.getId(), s2);

        Student s3("S003", "Charlie", "Brown", "IT", LoginStatus::ACTIVE);
        s3.setBirthday(20, 8, 1999); s3.setEmail("charlie.b@example.com"); s3.setCitizenId("003003003");
        mock_students_data.emplace(s3.getId(), s3);
        
        // Hòa có thể thêm S004 ở đây nếu muốn nó là mặc định
        // Student s4("S004", "Diana", "Prince", "LAW", LoginStatus::DISABLED);
        // s4.setBirthday(10, 3, 2000); s4.setEmail("diana.p@example.com"); s4.setCitizenId("004004004");
        // mock_students_data.emplace(s4.getId(), s4);

        mock_student_data_initialized_flag = true;
    }
}

void MockStudentDao::clearMockData() {
    mock_students_data.clear();
    mock_student_data_initialized_flag = false;
}

MockStudentDao::MockStudentDao() {
    // Constructor không tự động init data
}

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
    return all_students; 
}

std::expected<Student, Error> MockStudentDao::add(const Student& student) {
    if (mock_students_data.count(student.getId())) {
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Mock Student with ID " + student.getId() + " already exists"});
    }
    // Kiểm tra email trùng
    for(const auto& pair : mock_students_data){
        if(pair.second.getEmail() == student.getEmail()){
            return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Mock Student with email " + student.getEmail() + " already exists"});
        }
    }
    auto insert_result = mock_students_data.emplace(student.getId(), student);
    if (!insert_result.second) {
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to emplace student into mock data."});
    }
    return student;
}

std::expected<bool, Error> MockStudentDao::update(const Student& student) {
    auto it = mock_students_data.find(student.getId());
    if (it != mock_students_data.end()) {
        // Kiểm tra email trùng với student khác (ngoại trừ chính student đang update)
        for(const auto& pair : mock_students_data){
            if(pair.first != student.getId() && pair.second.getEmail() == student.getEmail()){
                return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Mock Student email " + student.getEmail() + " conflicts with another student."});
            }
        }
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
    return mock_students_data.count(id) > 0; 
}

std::expected<std::vector<Student>, Error> MockStudentDao::findByFacultyId(const std::string& facultyId) const {
    std::vector<Student> result;
    for (const auto& pair : mock_students_data) {
        if (pair.second.getFacultyId() == facultyId) {
            result.push_back(pair.second);
        }
    }
    return result; 
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
    return result; 
}

std::expected<bool, Error> MockStudentDao::updateStatus(const std::string& studentId, LoginStatus newStatus) {
    auto it = mock_students_data.find(studentId);
    if (it != mock_students_data.end()) {
        it->second.setStatus(newStatus); 
        // Giả sử UserRole không thay đổi khi chỉ updateStatus, trừ khi có logic cụ thể
        // Nếu PENDING_STUDENT -> ACTIVE thì role cũng đổi thành STUDENT
        if (it->second.getRole() == UserRole::PENDING_STUDENT && newStatus == LoginStatus::ACTIVE) {
            it->second.setRole(UserRole::STUDENT);
        }
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Student with ID " + studentId + " not found for status update"});
}