// --- START OF MODIFIED FILE src/core/data_access/mock/MockCourseDao.cpp ---
#include "MockCourseDao.h"
#include "../../entities/Course.h"
#include "../../../common/ErrorType.h"
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <expected>

namespace { 
    std::map<std::string, Course> mock_courses_data;
    bool mock_course_data_initialized_flag = false;
}

void MockCourseDao::initializeDefaultMockData() {
    if (!mock_course_data_initialized_flag) {
        mock_courses_data.clear();
        mock_courses_data.emplace("CS101", Course("CS101", "Introduction to Programming", 3, "CS"));
        mock_courses_data.emplace("IT202", Course("IT202", "Data Structures and Algorithms", 4, "IT"));
        mock_courses_data.emplace("EE301", Course("EE301", "Circuit Theory", 3, "EE"));
        mock_courses_data.emplace("POT101", Course("POT101", "Basic Potions", 3, "CHEM"));
        mock_courses_data.emplace("TRN101", Course("TRN101", "Intro to Transfiguration", 4, "TRAN"));
        mock_course_data_initialized_flag = true;
    }
}

void MockCourseDao::clearMockData() {
    mock_courses_data.clear();
    mock_course_data_initialized_flag = false;
}


MockCourseDao::MockCourseDao() {
    // Constructor không tự động init data
}

std::expected<Course, Error> MockCourseDao::getById(const std::string& id) const {
    auto it = mock_courses_data.find(id);
    if (it != mock_courses_data.end()) {
        return it->second;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Course with ID " + id + " not found"});
}

std::expected<std::vector<Course>, Error> MockCourseDao::getAll() const {
    std::vector<Course> result;
    for (const auto& pair : mock_courses_data) {
        result.push_back(pair.second);
    }
    return result;
}

std::expected<Course, Error> MockCourseDao::add(const Course& course) {
    if (mock_courses_data.count(course.getId())) {
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Mock Course with ID " + course.getId() + " already exists"});
    }
    // Có thể thêm kiểm tra trùng tên nếu cần
    auto insert_result = mock_courses_data.emplace(course.getId(), course);
    if (!insert_result.second) { 
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to emplace/insert course into mock data, possibly due to internal map state."});
    }
    return course;
}

std::expected<bool, Error> MockCourseDao::update(const Course& course) {
    auto it = mock_courses_data.find(course.getId());
    if (it != mock_courses_data.end()) {
        it->second = course; 
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Course with ID " + course.getId() + " not found for update"});
}

std::expected<bool, Error> MockCourseDao::remove(const std::string& id) {
    if (mock_courses_data.erase(id) > 0) {
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock Course with ID " + id + " not found for removal"});
}

std::expected<bool, Error> MockCourseDao::exists(const std::string& id) const {
    return mock_courses_data.count(id) > 0;
}

std::expected<std::vector<Course>, Error> MockCourseDao::findByFacultyId(const std::string& facultyId) const {
    std::vector<Course> result;
    for (const auto& pair : mock_courses_data) {
        if (pair.second.getFacultyId() == facultyId) {
            result.push_back(pair.second);
        }
    }
    return result;
}
// --- END OF MODIFIED FILE src/core/data_access/mock/MockCourseDao.cpp ---