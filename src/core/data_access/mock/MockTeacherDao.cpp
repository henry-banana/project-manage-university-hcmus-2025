// --- START OF MODIFIED FILE src/core/data_access/mock/MockTeacherDao.cpp ---
#include "MockTeacherDao.h"
#include "../../entities/Teacher.h"
#include "../../../common/ErrorType.h"
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <expected> 

namespace { 
    std::map<std::string, Teacher> mock_teachers_data;
    bool mock_teacher_data_initialized_flag = false;
}

void MockTeacherDao::initializeDefaultMockData() {
    if (!mock_teacher_data_initialized_flag) {
        mock_teachers_data.clear();
        Teacher t1("T001", "Severus", "Snape", "CHEM", LoginStatus::ACTIVE);
        t1.setBirthday(9, 1, 1960); t1.setEmail("snape@example.com"); t1.setCitizenId("100100100");
        t1.setQualification("Master of Potions"); t1.setSpecializationSubjects("Potions, Dark Arts Defense");
        t1.setDesignation("Professor, Head of Slytherin"); t1.setExperienceYears(20);
        mock_teachers_data.emplace(t1.getId(), t1); 

        Teacher t2("T002", "Minerva", "McGonagall", "TRAN", LoginStatus::ACTIVE);
        t2.setBirthday(4, 10, 1935); t2.setEmail("mcgonagall@example.com"); t2.setCitizenId("100100101");
        t2.setQualification("Animagus Mistress"); t2.setSpecializationSubjects("Transfiguration");
        t2.setDesignation("Professor, Head of Gryffindor, Deputy Headmistress"); t2.setExperienceYears(40);
        mock_teachers_data.emplace(t2.getId(), t2); 

        Teacher t3("T003", "Filius", "Flitwick", "CHAR", LoginStatus::DISABLED);
        t3.setBirthday(17,10,1958); t3.setEmail("flitwick@example.com"); t3.setCitizenId("100100102");
        t3.setQualification("Charms Master"); t3.setSpecializationSubjects("Charms");
        t3.setDesignation("Professor, Head of Ravenclaw"); t3.setExperienceYears(30);
        mock_teachers_data.emplace(t3.getId(), t3); 

        mock_teacher_data_initialized_flag = true;
    }
}
void MockTeacherDao::clearMockData() {
    mock_teachers_data.clear();
    mock_teacher_data_initialized_flag = false;
}

MockTeacherDao::MockTeacherDao() {
    // Constructor không tự động init data
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
    for(const auto& pair : mock_teachers_data){
        if(pair.second.getEmail() == teacher.getEmail()){
            return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Mock Teacher with email " + teacher.getEmail() + " already exists"});
        }
    }
    auto insert_result = mock_teachers_data.emplace(teacher.getId(), teacher);
    if (!insert_result.second) {
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to emplace teacher into mock data."});
    }
    return teacher;
}

std::expected<bool, Error> MockTeacherDao::update(const Teacher& teacher) {
    auto it = mock_teachers_data.find(teacher.getId());
    if (it != mock_teachers_data.end()) {
        for(const auto& pair : mock_teachers_data){
            if(pair.first != teacher.getId() && pair.second.getEmail() == teacher.getEmail()){
                return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Mock Teacher email " + teacher.getEmail() + " conflicts with another teacher."});
            }
        }
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
// --- END OF MODIFIED FILE src/core/data_access/mock/MockTeacherDao.cpp ---