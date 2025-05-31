// --- START OF MODIFIED FILE src/core/data_access/mock/MockCourseResultDao.cpp ---
#include "MockCourseResultDao.h"
#include "../../entities/CourseResult.h"
#include "../../../common/ErrorType.h"
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <expected> 

namespace {
    std::map<std::string, CourseResult> mock_course_results_data;
    bool mock_course_result_data_initialized_flag = false;

    std::string makeCourseResultKey(const std::string& studentId, const std::string& courseId) {
        return studentId + "_" + courseId;
    }
}

void MockCourseResultDao::initializeDefaultMockData() {
    if (!mock_course_result_data_initialized_flag) {
        mock_course_results_data.clear();
        mock_course_results_data.emplace(makeCourseResultKey("S001", "CS101"), CourseResult("S001", "CS101", 85));
        mock_course_results_data.emplace(makeCourseResultKey("S001", "IT202"), CourseResult("S001", "IT202", 70));
        mock_course_results_data.emplace(makeCourseResultKey("S002", "CS101"), CourseResult("S002", "CS101", 92));
        mock_course_results_data.emplace(makeCourseResultKey("S003", "IT202"), CourseResult("S003", "IT202", 60));
        // Hòa có thể thêm các record khác ở đây
        mock_course_result_data_initialized_flag = true;
    }
}

void MockCourseResultDao::clearMockData() {
    mock_course_results_data.clear();
    mock_course_result_data_initialized_flag = false;
}

MockCourseResultDao::MockCourseResultDao() {
    // Constructor không tự động init data
}

std::expected<CourseResult, Error> MockCourseResultDao::find(const std::string& studentId, const std::string& courseId) const {
    auto key = makeCourseResultKey(studentId, courseId);
    auto it = mock_course_results_data.find(key);
    if (it != mock_course_results_data.end()) {
        return it->second;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock CourseResult: Record not found for Student " + studentId + ", Course " + courseId});
}

std::expected<std::vector<CourseResult>, Error> MockCourseResultDao::findByStudentId(const std::string& studentId) const {
    std::vector<CourseResult> results;
    for (const auto& pair : mock_course_results_data) {
        if (pair.second.getStudentId() == studentId) {
            results.push_back(pair.second);
        }
    }
    return results;
}

std::expected<std::vector<CourseResult>, Error> MockCourseResultDao::findByCourseId(const std::string& courseId) const {
    std::vector<CourseResult> results;
    for (const auto& pair : mock_course_results_data) {
        if (pair.second.getCourseId() == courseId) {
            results.push_back(pair.second);
        }
    }
    return results;
}

std::expected<bool, Error> MockCourseResultDao::addOrUpdate(const CourseResult& result) {
    ValidationResult vr = result.validate(); 
    if (!vr.isValid) {
        // Trả về lỗi đầu tiên tìm thấy từ validate()
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid CourseResult data: " + vr.errors[0].message});
    }
    auto key = makeCourseResultKey(result.getStudentId(), result.getCourseId());
    mock_course_results_data.insert_or_assign(key, result); // Thêm hoặc ghi đè
    return true;
}

std::expected<bool, Error> MockCourseResultDao::remove(const std::string& studentId, const std::string& courseId) {
    auto key = makeCourseResultKey(studentId, courseId);
    if (mock_course_results_data.erase(key) > 0) {
        return true;
    }
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Mock CourseResult: Record not found for Student " + studentId + ", Course " + courseId + " to remove."});
}

std::expected<bool, Error> MockCourseResultDao::removeAllForStudent(const std::string& studentId) {
     for (auto it = mock_course_results_data.begin(); it != mock_course_results_data.end(); ) {
        if (it->second.getStudentId() == studentId) {
            it = mock_course_results_data.erase(it);
        } else {
            ++it;
        }
    }
    return true; 
}

std::expected<bool, Error> MockCourseResultDao::removeAllForCourse(const std::string& courseId) {
     for (auto it = mock_course_results_data.begin(); it != mock_course_results_data.end(); ) {
        if (it->second.getCourseId() == courseId) {
            it = mock_course_results_data.erase(it);
        } else {
            ++it;
        }
    }
    return true;
}
// --- END OF MODIFIED FILE src/core/data_access/mock/MockCourseResultDao.cpp ---