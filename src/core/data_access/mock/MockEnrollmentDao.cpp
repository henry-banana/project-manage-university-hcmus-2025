#include "MockEnrollmentDao.h"
#include "../../../common/ErrorType.h"
#include <vector>
#include <string>
#include <algorithm>
#include <expected>

namespace { // Copy lại
    std::vector<EnrollmentRecord> mock_enrollments_data;
    bool mock_enrollment_data_initialized = false;

    void initializeMockEnrollmentDataIfNeeded() {
        if (!mock_enrollment_data_initialized) {
            mock_enrollments_data.push_back({"S001", "CS101"});
            mock_enrollments_data.push_back({"S001", "IT202"});
            mock_enrollments_data.push_back({"S002", "CS101"});
            mock_enrollments_data.push_back({"S003", "IT202"});
            mock_enrollments_data.push_back({"S003", "EE301"});
            mock_enrollments_data.push_back({"S001", "POT101"});
            mock_enrollments_data.push_back({"S002", "TRN101"});

            mock_enrollment_data_initialized = true;
        }
    }
}

MockEnrollmentDao::MockEnrollmentDao() {
    initializeMockEnrollmentDataIfNeeded();
}

std::expected<bool, Error> MockEnrollmentDao::addEnrollment(const std::string& studentId, const std::string& courseId) {
    auto enrolledCheck = isEnrolled(studentId, courseId);
    if (enrolledCheck.has_value() && enrolledCheck.value()) {
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Student " + studentId + " already enrolled in course " + courseId});
    }
    // Giả sử isEnrolled trả về error nếu có vấn đề khác
    if (!enrolledCheck.has_value() && enrolledCheck.error().code != ErrorCode::NOT_FOUND) { // NOT_FOUND là ok để add
         return std::unexpected(enrolledCheck.error());
    }

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
    return std::unexpected(Error{ErrorCode::NOT_FOUND, "Enrollment not found for Student " + studentId + " in Course " + courseId});
}

std::expected<bool, Error> MockEnrollmentDao::removeEnrollmentsByStudent(const std::string& studentId) {
    auto initial_size = mock_enrollments_data.size();
    mock_enrollments_data.erase(
        std::remove_if(mock_enrollments_data.begin(), mock_enrollments_data.end(),
                       [&](const EnrollmentRecord& er){ return er.studentId == studentId; }),
        mock_enrollments_data.end()
    );
    // Trong mock, việc này luôn thành công, kể cả khi không có gì để xóa
    return true; // Hoặc trả về true nếu size thay đổi
}

std::expected<bool, Error> MockEnrollmentDao::removeEnrollmentsByCourse(const std::string& courseId) {
    mock_enrollments_data.erase(
        std::remove_if(mock_enrollments_data.begin(), mock_enrollments_data.end(),
                       [&](const EnrollmentRecord& er){ return er.courseId == courseId; }),
        mock_enrollments_data.end()
    );
    return true;
}

std::expected<bool, Error> MockEnrollmentDao::isEnrolled(const std::string& studentId, const std::string& courseId) const {
    bool found = std::any_of(mock_enrollments_data.begin(), mock_enrollments_data.end(),
                       [&](const EnrollmentRecord& er){
                           return er.studentId == studentId && er.courseId == courseId;
                       });
    if (found) return true;
    // Trả về lỗi NOT_FOUND nếu không tìm thấy, thay vì false trực tiếp, để phân biệt với lỗi khác
    // Tuy nhiên, isEnrolled thường được mong đợi trả về bool (true/false) hơn là error khi không tìm thấy.
    // Để nhất quán, nếu không tìm thấy, ta trả về `false` và không có error.
    return false; 
    // Nếu muốn phân biệt rõ "không tìm thấy" và "lỗi thực sự", có thể phức tạp hơn:
    // return std::unexpected(Error{ErrorCode::NOT_FOUND, "Enrollment record not found."}); // nếu muốn coi "không tìm thấy" là một dạng lỗi kiểm tra được
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