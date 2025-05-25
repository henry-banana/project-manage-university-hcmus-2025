#include "EnrollmentService.h"
#include "../../../utils/Logger.h"

EnrollmentService::EnrollmentService(
    std::shared_ptr<IEnrollmentDao> enrollmentDao,
    std::shared_ptr<IStudentDao> studentDao,
    std::shared_ptr<ICourseDao> courseDao,
    std::shared_ptr<IGeneralInputValidator> inputValidator,
    std::shared_ptr<SessionContext> sessionContext)
    : _enrollmentDao(std::move(enrollmentDao)),
      _studentDao(std::move(studentDao)),
      _courseDao(std::move(courseDao)),
      _inputValidator(std::move(inputValidator)),
      _sessionContext(std::move(sessionContext)) {
    if (!_enrollmentDao) throw std::invalid_argument("EnrollmentDao cannot be null.");
    if (!_studentDao) throw std::invalid_argument("StudentDao cannot be null.");
    if (!_courseDao) throw std::invalid_argument("CourseDao cannot be null.");
    if (!_inputValidator) throw std::invalid_argument("InputValidator cannot be null.");
    if (!_sessionContext) throw std::invalid_argument("SessionContext cannot be null.");
}

std::expected<bool, Error> EnrollmentService::enrollStudentInCourse(const std::string& studentId, const std::string& courseId) {
    if (!_sessionContext->isAuthenticated()) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    auto currentUserRoleOpt = _sessionContext->getCurrentUserRole();
    auto currentUserIdOpt = _sessionContext->getCurrentUserId();

    // Chỉ Student mới được tự đăng ký, hoặc Admin đăng ký cho Student
    if (!currentUserRoleOpt.has_value() || 
        (currentUserRoleOpt.value() != UserRole::ADMIN && 
         (currentUserRoleOpt.value() != UserRole::STUDENT || !currentUserIdOpt.has_value() || currentUserIdOpt.value() != studentId)
        )
       ) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Permission denied to enroll in course."});
    }
    
    ValidationResult studentIdVr = _inputValidator->validateIdFormat(studentId, "Student ID");
    if (!studentIdVr.isValid) return std::unexpected(studentIdVr.errors[0]);
    ValidationResult courseIdVr = _inputValidator->validateIdFormat(courseId, "Course ID");
    if (!courseIdVr.isValid) return std::unexpected(courseIdVr.errors[0]);

    // Kiểm tra Student tồn tại và ACTIVE
    auto studentResult = _studentDao->getById(studentId);
    if (!studentResult.has_value()) {
        return std::unexpected(studentResult.error());
    }
    if (studentResult.value().getStatus() != LoginStatus::ACTIVE) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Student account is not active. Cannot enroll in courses."});
    }

    // Kiểm tra Course tồn tại
    auto courseExists = _courseDao->exists(courseId);
    if (!courseExists.has_value()) return std::unexpected(courseExists.error());
    if (!courseExists.value()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Course with ID '" + courseId + "' not found."});
    }

    // Kiểm tra đã enroll chưa
    auto enrolledCheck = _enrollmentDao->isEnrolled(studentId, courseId);
    if (!enrolledCheck.has_value()) return std::unexpected(enrolledCheck.error());
    if (enrolledCheck.value()) {
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Student is already enrolled in this course."});
    }

    auto enrollResult = _enrollmentDao->addEnrollment(studentId, courseId);
    if (enrollResult.has_value() && enrollResult.value()) {
        LOG_INFO("Student " + studentId + " enrolled in course " + courseId);
    }
    return enrollResult;
}

std::expected<bool, Error> EnrollmentService::dropCourseForStudent(const std::string& studentId, const std::string& courseId) {
     if (!_sessionContext->isAuthenticated()) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    auto currentUserRoleOpt = _sessionContext->getCurrentUserRole();
    auto currentUserIdOpt = _sessionContext->getCurrentUserId();

    if (!currentUserRoleOpt.has_value() || 
        (currentUserRoleOpt.value() != UserRole::ADMIN && 
         (currentUserRoleOpt.value() != UserRole::STUDENT || !currentUserIdOpt.has_value() || currentUserIdOpt.value() != studentId)
        )
       ) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Permission denied to drop course."});
    }

    ValidationResult studentIdVr = _inputValidator->validateIdFormat(studentId, "Student ID");
    if (!studentIdVr.isValid) return std::unexpected(studentIdVr.errors[0]);
    ValidationResult courseIdVr = _inputValidator->validateIdFormat(courseId, "Course ID");
    if (!courseIdVr.isValid) return std::unexpected(courseIdVr.errors[0]);

    // Không cần check student/course tồn tại vì nếu enrollment không có thì DAO sẽ báo lỗi NOT_FOUND.
    auto dropResult = _enrollmentDao->removeEnrollment(studentId, courseId);
     if (dropResult.has_value() && dropResult.value()) {
        LOG_INFO("Student " + studentId + " dropped course " + courseId);
    }
    return dropResult;
}

std::expected<std::vector<Course>, Error> EnrollmentService::getEnrolledCoursesByStudent(const std::string& studentId) const {
    if (!_sessionContext->isAuthenticated()) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    auto currentUserIdOpt = _sessionContext->getCurrentUserId();
    auto currentUserRoleOpt = _sessionContext->getCurrentUserRole();
    if (!currentUserRoleOpt.has_value() || 
        (currentUserRoleOpt.value() != UserRole::ADMIN && 
         currentUserRoleOpt.value() != UserRole::TEACHER && // GV có thể xem SV đăng ký môn của mình (logic phức tạp hơn)
         (currentUserRoleOpt.value() != UserRole::STUDENT || !currentUserIdOpt.has_value() || currentUserIdOpt.value() != studentId)
        )
       ) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Permission denied to view enrolled courses."});
    }

    ValidationResult studentIdVr = _inputValidator->validateIdFormat(studentId, "Student ID");
    if (!studentIdVr.isValid) return std::unexpected(studentIdVr.errors[0]);

    auto courseIdsResult = _enrollmentDao->findCourseIdsByStudentId(studentId);
    if (!courseIdsResult.has_value()) {
        return std::unexpected(courseIdsResult.error());
    }

    std::vector<Course> courses;
    for (const auto& cId : courseIdsResult.value()) {
        auto courseResult = _courseDao->getById(cId);
        if (courseResult.has_value()) {
            courses.push_back(courseResult.value());
        } else {
            LOG_WARN("Could not retrieve details for enrolled course ID: " + cId + " for student " + studentId);
            // Có thể bỏ qua hoặc trả về lỗi tùy theo yêu cầu
        }
    }
    return courses;
}

std::expected<std::vector<Student>, Error> EnrollmentService::getEnrolledStudentsByCourse(const std::string& courseId) const {
    if (!_sessionContext->isAuthenticated()) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
     auto currentUserRoleOpt = _sessionContext->getCurrentUserRole();
    // Chỉ Admin hoặc Teacher của khoa đó/môn đó mới được xem (logic phức tạp hơn cho Teacher)
    if (!currentUserRoleOpt.has_value() || (currentUserRoleOpt.value() != UserRole::ADMIN && currentUserRoleOpt.value() != UserRole::TEACHER)) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Permission denied to view enrolled students."});
    }

    ValidationResult courseIdVr = _inputValidator->validateIdFormat(courseId, "Course ID");
    if (!courseIdVr.isValid) return std::unexpected(courseIdVr.errors[0]);

    auto studentIdsResult = _enrollmentDao->findStudentIdsByCourseId(courseId);
    if (!studentIdsResult.has_value()) {
        return std::unexpected(studentIdsResult.error());
    }

    std::vector<Student> students;
    for (const auto& sId : studentIdsResult.value()) {
        auto studentResult = _studentDao->getById(sId);
        if (studentResult.has_value()) {
            students.push_back(studentResult.value());
        } else {
            LOG_WARN("Could not retrieve details for enrolled student ID: " + sId + " for course " + courseId);
        }
    }
    return students;
}

std::expected<bool, Error> EnrollmentService::isStudentEnrolled(const std::string& studentId, const std::string& courseId) const {
    if (!_sessionContext->isAuthenticated()) { // Có thể cho phép public check? Hoặc chỉ user liên quan?
        // return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    ValidationResult studentIdVr = _inputValidator->validateIdFormat(studentId, "Student ID");
    if (!studentIdVr.isValid) return std::unexpected(studentIdVr.errors[0]);
    ValidationResult courseIdVr = _inputValidator->validateIdFormat(courseId, "Course ID");
    if (!courseIdVr.isValid) return std::unexpected(courseIdVr.errors[0]);
    
    return _enrollmentDao->isEnrolled(studentId, courseId);
}