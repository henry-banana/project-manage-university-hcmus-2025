#include "MockCourseResultDao.h"
#include "../../entities/CourseResult.h"
#include "../../../common/ErrorType.h"
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <expected> // Đã include trong header, nhưng để rõ ràng

// Namespace ẩn danh và initializeMockCourseResultDataIfNeeded giữ nguyên như trước,
// chỉ cần đảm bảo CourseResult được tạo đúng cách.
namespace {
    std::map<std::string, CourseResult> mock_course_results_data;
    bool mock_course_result_data_initialized = false;

    std::string makeCourseResultKey(const std::string& studentId, const std::string& courseId) {
        return studentId + "_" + courseId;
    }

    void initializeMockCourseResultDataIfNeeded() {
        if (!mock_course_result_data_initialized) {
            // SỬA Ở ĐÂY:
            mock_course_results_data.emplace(makeCourseResultKey("S001", "CS101"), CourseResult("S001", "CS101", 85));
            mock_course_results_data.emplace(makeCourseResultKey("S001", "IT202"), CourseResult("S001", "IT202", 70));
            mock_course_results_data.emplace(makeCourseResultKey("S002", "CS101"), CourseResult("S002", "CS101", 92));
            mock_course_results_data.emplace(makeCourseResultKey("S003", "IT202"), CourseResult("S003", "IT202", 60));
            mock_course_results_data.emplace(makeCourseResultKey("S003", "EE301"), CourseResult("S003", "EE301", 75));
            mock_course_results_data.emplace(makeCourseResultKey("S001", "POT101"), CourseResult("S001", "POT101", 95));
            mock_course_results_data.emplace(makeCourseResultKey("S002", "TRN101"), CourseResult("S002", "TRN101", 88));
            mock_course_results_data.emplace(makeCourseResultKey("S001", "EE301"), CourseResult("S001", "EE301", -1)); // Chưa có điểm        
            mock_course_result_data_initialized = true;
        }
    }
}
MockCourseResultDao::MockCourseResultDao() {
    initializeMockCourseResultDataIfNeeded();
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
    // Việc không tìm thấy kết quả nào cho sinh viên không nhất thiết là lỗi,
    // có thể sinh viên đó chưa có điểm nào. Trả về vector rỗng.
    return results;
}

std::expected<std::vector<CourseResult>, Error> MockCourseResultDao::findByCourseId(const std::string& courseId) const {
    std::vector<CourseResult> results;
    for (const auto& pair : mock_course_results_data) {
        if (pair.second.getCourseId() == courseId) {
            results.push_back(pair.second);
        }
    }
    return results; // Tương tự, trả về vector rỗng nếu không có
}

std::expected<bool, Error> MockCourseResultDao::addOrUpdate(const CourseResult& result) {
    ValidationResult vr = result.validate(); 
    if (!vr.isValid) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid CourseResult data: " + vr.getErrorMessagesCombined()});
    }
    auto key = makeCourseResultKey(result.getStudentId(), result.getCourseId());

    // SỬA Ở ĐÂY:
    auto it = mock_course_results_data.find(key);
    if (it != mock_course_results_data.end()) {
        // Key đã tồn tại, thực hiện update (ghi đè)
        it->second = result; 
    } else {
        // Key chưa tồn tại, thực hiện emplace (thêm mới)
        mock_course_results_data.emplace(key, result);
    }
    // Hoặc một cách ngắn gọn hơn cho logic "thêm hoặc cập nhật" nếu bạn dùng C++17 trở lên:
    // mock_course_results_data.insert_or_assign(key, result);

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
     //bool removed_at_least_one = false; // Không cần thiết cho mock đơn giản này
     for (auto it = mock_course_results_data.begin(); it != mock_course_results_data.end(); ) {
        if (it->second.getStudentId() == studentId) {
            it = mock_course_results_data.erase(it);
            // removed_at_least_one = true;
        } else {
            ++it;
        }
    }
    return true; // Trong mock, việc không tìm thấy gì để xóa vẫn coi là "hoàn thành yêu cầu"
}

std::expected<bool, Error> MockCourseResultDao::removeAllForCourse(const std::string& courseId) {
    // bool removed_at_least_one = false;
     for (auto it = mock_course_results_data.begin(); it != mock_course_results_data.end(); ) {
        if (it->second.getCourseId() == courseId) {
            it = mock_course_results_data.erase(it);
            // removed_at_least_one = true;
        } else {
            ++it;
        }
    }
    return true;
}