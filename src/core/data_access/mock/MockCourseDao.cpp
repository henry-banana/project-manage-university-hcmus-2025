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
    bool mock_course_data_initialized = false;

    void initializeMockCourseDataIfNeeded() {
        if (!mock_course_data_initialized) {
            // SỬA Ở ĐÂY:
            mock_courses_data.emplace("CS101", Course("CS101", "Introduction to Programming", 3, "CS"));
            mock_courses_data.emplace("IT202", Course("IT202", "Data Structures and Algorithms", 4, "IT"));
            // ... các emplace khác ...
            mock_course_data_initialized = true;
        }
    }
}

MockCourseDao::MockCourseDao() {
    initializeMockCourseDataIfNeeded();
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
    auto insert_result = mock_courses_data.emplace(course.getId(), course); // Cách 1: Dùng emplace
    // Hoặc cách 2: Dùng insert (tạo std::pair trước)
    // auto insert_result = mock_courses_data.insert(std::make_pair(course.getId(), course));

    if (!insert_result.second) { // Kiểm tra xem việc chèn có thực sự diễn ra không (dù đã check count ở trên)
        // Trường hợp này không nên xảy ra nếu logic count ở trên đúng
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Failed to emplace/insert course into mock data, possibly due to internal map state."});
    }
    return course;
}

std::expected<bool, Error> MockCourseDao::update(const Course& course) {
    auto it = mock_courses_data.find(course.getId());
    if (it != mock_courses_data.end()) {
        it->second = course; // Gán trực tiếp khi đã tìm thấy là OK
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