// --- START OF MODIFIED FILE src/core/data_access/mock/MockEnrollmentDao.cpp ---
#include "MockEnrollmentDao.h"
#include "../../../common/ErrorType.h"
#include <vector>
#include <string>
#include <algorithm>
#include <expected>

namespace { 
    std::vector<EnrollmentRecord> mock_enrollments_data;
    bool mock_enrollment_data_initialized_flag = false;
}

void MockEnrollmentDao::initializeDefaultMockData() {
    if (!mock_enrollment_data_initialized_flag) {
        mock_enrollments_data.clear();
        mock_enrollments_data.push_back({"S001", "CS101"});
        mock_enrollments_data.push_back({"S001", "IT202"});
        mock_enrollments_data.push_back({"S002", "CS101"});
        mock_enrollments_data.push_back({"S003", "IT202"});
        mock_enrollments_data.push_back({"S003", "EE301"});
        mock_enrollments_data.push_back({"S001", "POT101"});
        mock_enrollments_data.push_back({"S002", "TRN101"});
        mock_enrollment_data_initialized_flag = true;
    }
}

void MockEnrollmentDao::clearMockData() {
    mock_enrollments_data.clear();
    mock_enrollment_data_initialized_flag = false;
}

MockEnrollmentDao::MockEnrollmentDao() {
    // Constructor không tự động init data
}

std::expected<bool, Error> MockEnrollmentDao::addEnrollment(const std::string& studentId, const std::string& courseId) {
    auto enrolledCheck = isEnrolled(studentId, courseId); // isEnrolled đã trả về std::expected<bool, Error>

    if (!enrolledCheck.has_value()) { // Lỗi khi kiểm tra isEnrolled
        return std::unexpected(enrolledCheck.error());
    }

    if (enrolledCheck.value()) { // Nếu value là true -> đã enrolled
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Student " + studentId + " already enrolled in course " + courseId});
    }
    
    // Nếu value là false -> chưa enrolled, tiến hành add
    mock_enrollments_data.push_back({studentId, courseId});
    return true;
}

std::expected<bool, Error> MockEnrollmentDao::removeEnrollment(const std::string& studentId, const std::string& courseId) {
    auto it = std::remove_if(mock_enrollments_data.begin(), mock_enrollments_data.end(),
                             [&](const EnrollmentRecord& er){
                                 return er.studentId == studentId && er.courseId == courseId;
                             });
    if (it != mock_enrollments_data.end()) {
        mock_enrollments_data.erase(it, mock_enrollments_data.end());
        return true;
    }
    // Nếu không tìm thấy để xóa, trả về lỗi NOT_FOUND
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Enrollment not found for Student " + studentId + " in Course " + courseId + " to remove."});
}

std::expected<bool, Error> MockEnrollmentDao::removeEnrollmentsByStudent(const std::string& studentId) {
    // auto initial_size = mock_enrollments_data.size(); // Không cần thiết nữa
    mock_enrollments_data.erase(
        std::remove_if(mock_enrollments_data.begin(), mock_enrollments_data.end(),
                       [&](const EnrollmentRecord& er){ return er.studentId == studentId; }),
        mock_enrollments_data.end()
    );
    return true; // Luôn thành công, kể cả khi không xóa gì
}

std::expected<bool, Error> MockEnrollmentDao::removeEnrollmentsByCourse(const std::string& courseId) {
    mock_enrollments_data.erase(
        std::remove_if(mock_enrollments_data.begin(), mock_enrollments_data.end(),
                       [&](const EnrollmentRecord& er){ return er.courseId == courseId; }),
        mock_enrollments_data.end()
    );
    return true; // Luôn thành công
}

std::expected<bool, Error> MockEnrollmentDao::isEnrolled(const std::string& studentId, const std::string& courseId) const {
    bool found = std::any_of(mock_enrollments_data.begin(), mock_enrollments_data.end(),
                       [&](const EnrollmentRecord& er){
                           return er.studentId == studentId && er.courseId == courseId;
                       });
    return found; 
}

std::expected<std::vector<std::string>, Error> MockEnrollmentDao::findCourseIdsByStudentId(const std::string& studentId) const {
    std::vector<std::string> courseIds;
    for (const auto& er : mock_enrollments_data) {
        if (er.studentId == studentId) {
            courseIds.push_back(er.courseId);
        }
    }
    return courseIds;
}

std::expected<std::vector<std::string>, Error> MockEnrollmentDao::findStudentIdsByCourseId(const std::string& courseId) const {
    std::vector<std::string> studentIds;
    for (const auto& er : mock_enrollments_data) {
        if (er.courseId == courseId) {
            studentIds.push_back(er.studentId);
        }
    }
    return studentIds;
}

std::expected<std::vector<EnrollmentRecord>, Error> MockEnrollmentDao::getAllEnrollments() const {
    return mock_enrollments_data;
}
// --- END OF MODIFIED FILE src/core/data_access/mock/MockEnrollmentDao.cpp ---