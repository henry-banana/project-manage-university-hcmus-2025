#include "MockTeacherDao.h"
#include "../../entities/Teacher.h"
#include "../../../common/ErrorType.h"
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <expected> // (➕)

// Namespace ẩn danh và initializeMockTeacherDataIfNeeded giữ nguyên như trước

namespace { // Copy lại phần này
    std::map<std::string, Teacher> mock_teachers_data;
    bool mock_teacher_data_initialized = false;

    void initializeMockTeacherDataIfNeeded() {
        if (!mock_teacher_data_initialized) {
            Teacher t1("T001", "Severus", "Snape", "CHEM", LoginStatus::ACTIVE);
            t1.setBirthday(9, 1, 1960); t1.setEmail("snape@example.com"); t1.setCitizenId("100100100");
            t1.setQualification("Master of Potions"); t1.setSpecializationSubjects("Potions, Dark Arts Defense");
            t1.setDesignation("Professor, Head of Slytherin"); t1.setExperienceYears(20);
            mock_teachers_data[t1.getId()] = t1;

            Teacher t2("T002", "Minerva", "McGonagall", "TRAN", LoginStatus::ACTIVE);
            t2.setBirthday(4, 10, 1935); t2.setEmail("mcgonagall@example.com"); t2.setCitizenId("100100101");
            t2.setQualification("Animagus Mistress"); t2.setSpecializationSubjects("Transfiguration");
            t2.setDesignation("Professor, Head of Gryffindor, Deputy Headmistress"); t2.setExperienceYears(40);
            mock_teachers_data[t2.getId()] = t2;

            Teacher t3("T003", "Filius", "Flitwick", "CHAR", LoginStatus::DISABLED);
            t3.setBirthday(17,10,1958); t3.setEmail("flitwick@example.com"); t3.setCitizenId("100100102");
            t3.setQualification("Charms Master"); t3.setSpecializationSubjects("Charms");
            t3.setDesignation("Professor, Head of Ravenclaw"); t3.setExperienceYears(30);
            mock_teachers_data[t3.getId()] = t3;

            mock_teacher_data_initialized = true;
        }
    }
}

MockTeacherDao::MockTeacherDao() {
    initializeMockTeacherDataIfNeeded();
}

std::expected<Teacher, Error> MockTeacherDao::getById(const std::string& id) const {
    auto it = mock_teachers_data.find(id);
    if (it != mock_teachers_data.end()) {
        return it->second;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Teacher with ID " + id + " not found"});
}

std::expected<std::vector<Teacher>, Error> MockTeacherDao::getAll() const {
    std::vector<Teacher> all_teachers;
    for (const auto& pair : mock_teachers_data) {
        all_teachers.push_back(pair.second);
    }
    return all_teachers;
}

std::expected<Teacher, Error> MockTeacherDao::add(const Teacher& teacher) {
    if (mock_teachers_data.count(teacher.getId())) {
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Mock Teacher with ID " + teacher.getId() + " already exists"});
    }
    mock_teachers_data[teacher.getId()] = teacher;
    return teacher;
}

std::expected<bool, Error> MockTeacherDao::update(const Teacher& teacher) {
    auto it = mock_teachers_data.find(teacher.getId());
    if (it != mock_teachers_data.end()) {
        it->second = teacher;
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Teacher with ID " + teacher.getId() + " not found for update"});
}

std::expected<bool, Error> MockTeacherDao::remove(const std::string& id) {
    if (mock_teachers_data.erase(id) > 0) {
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Teacher with ID " + id + " not found for removal"});
}

std::expected<bool, Error> MockTeacherDao::exists(const std::string& id) const {
    return mock_teachers_data.count(id) > 0;
}

std::expected<std::vector<Teacher>, Error> MockTeacherDao::findByFacultyId(const std::string& facultyId) const {
    std::vector<Teacher> result;
    for (const auto& pair : mock_teachers_data) {
        if (pair.second.getFacultyId() == facultyId) {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::expected<std::vector<Teacher>, Error> MockTeacherDao::findByDesignation(const std::string& designation) const {
    std::vector<Teacher> result;
    for (const auto& pair : mock_teachers_data) {
        // Tìm kiếm chính xác hoặc tìm chuỗi con tùy yêu cầu
        if (pair.second.getDesignation().find(designation) != std::string::npos) {
            result.push_back(pair.second);
        }
    }
    return result;
}
std::expected<Teacher, Error> MockTeacherDao::findByEmail(const std::string& email) const {
     for (const auto& pair : mock_teachers_data) {
        if (pair.second.getEmail() == email) {
            return pair.second;
        }
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Teacher with email " + email + " not found"});
}