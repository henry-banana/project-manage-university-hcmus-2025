#include "CourseService.h"
#include "../../../utils/Logger.h"

CourseService::CourseService(std::shared_ptr<ICourseDao> courseDao,
                             std::shared_ptr<IFacultyDao> facultyDao,
                             std::shared_ptr<IEnrollmentDao> enrollmentDao,
                             std::shared_ptr<ICourseResultDao> courseResultDao,
                             std::shared_ptr<IGeneralInputValidator> inputValidator,
                             std::shared_ptr<SessionContext> sessionContext)
    : _courseDao(std::move(courseDao)),
      _facultyDao(std::move(facultyDao)),
      _enrollmentDao(std::move(enrollmentDao)),
      _courseResultDao(std::move(courseResultDao)),
      _inputValidator(std::move(inputValidator)),
      _sessionContext(std::move(sessionContext)) {
    if (!_courseDao) throw std::invalid_argument("CourseDao cannot be null.");
    if (!_facultyDao) throw std::invalid_argument("FacultyDao cannot be null.");
    if (!_enrollmentDao) throw std::invalid_argument("EnrollmentDao cannot be null.");
    if (!_courseResultDao) throw std::invalid_argument("CourseResultDao cannot be null.");
    if (!_inputValidator) throw std::invalid_argument("InputValidator cannot be null.");
    if (!_sessionContext) throw std::invalid_argument("SessionContext cannot be null.");
}

std::expected<Course, Error> CourseService::getCourseById(const std::string& courseId) const {
    if (courseId.empty()) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Course ID cannot be empty."});
    }
    // Ai cũng có thể xem thông tin khóa học
    return _courseDao->getById(courseId);
}

std::expected<std::vector<Course>, Error> CourseService::getAllCourses() const {
    // Ai cũng có thể xem
    return _courseDao->getAll();
}

std::expected<std::vector<Course>, Error> CourseService::getCoursesByFaculty(const std::string& facultyId) const {
     if (facultyId.empty()) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Faculty ID cannot be empty."});
    }
    return _courseDao->findByFacultyId(facultyId);
}

std::expected<Course, Error> CourseService::addCourse(const std::string& id, const std::string& name, int credits, const std::string& facultyId) {
    if (!_sessionContext->isAuthenticated() || !_sessionContext->getCurrentUserRole().has_value() || _sessionContext->getCurrentUserRole().value() != UserRole::ADMIN) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can add courses."});
    }
    ValidationResult vrTotal;
    auto appendErrors = [&](const ValidationResult& vr) {
        for(const auto& err : vr.errors) vrTotal.addError(err);
    };

    appendErrors(_inputValidator->validateIdFormat(id, "Course ID", 3, 20));
    appendErrors(_inputValidator->validateRequiredString(name, "Course Name", 150));
    appendErrors(_inputValidator->validateInteger(credits, "Credits", 1, 10));
    appendErrors(_inputValidator->validateRequiredString(facultyId, "Faculty ID for Course", 10));

    if(!vrTotal.isValid) return std::unexpected(vrTotal.errors[0]);

    auto facultyExists = _facultyDao->exists(facultyId);
    if (!facultyExists.has_value()) return std::unexpected(facultyExists.error());
    if (!facultyExists.value()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Faculty ID '" + facultyId + "' does not exist for course creation."});
    }

    Course newCourse(id, name, credits, facultyId);
    auto addResult = _courseDao->add(newCourse); // DAO sẽ check trùng ID
    if (addResult.has_value()) {
        LOG_INFO("Course added: ID=" + id + ", Name=" + name);
    }
    return addResult;
}

std::expected<bool, Error> CourseService::updateCourse(const std::string& courseId, const std::string& newName, int newCredits, const std::string& newFacultyId) {
    if (!_sessionContext->isAuthenticated() || !_sessionContext->getCurrentUserRole().has_value() || _sessionContext->getCurrentUserRole().value() != UserRole::ADMIN) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can update courses."});
    }
    ValidationResult vrTotal;
    auto appendErrors = [&](const ValidationResult& vr) {
        for(const auto& err : vr.errors) vrTotal.addError(err);
    };

    appendErrors(_inputValidator->validateIdFormat(courseId, "Course ID", 1, 20));
    appendErrors(_inputValidator->validateRequiredString(newName, "New Course Name", 150));
    appendErrors(_inputValidator->validateInteger(newCredits, "New Credits", 1, 10));
    appendErrors(_inputValidator->validateRequiredString(newFacultyId, "New Faculty ID", 10));

    if(!vrTotal.isValid) return std::unexpected(vrTotal.errors[0]);

    auto courseResult = _courseDao->getById(courseId);
    if (!courseResult.has_value()) {
        return std::unexpected(courseResult.error());
    }
    Course courseToUpdate = courseResult.value();

    auto facultyExists = _facultyDao->exists(newFacultyId);
    if (!facultyExists.has_value()) return std::unexpected(facultyExists.error());
    if (!facultyExists.value()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "New Faculty ID '" + newFacultyId + "' does not exist for course update."});
    }
    
    bool changed = false;
    if(courseToUpdate.getName() != newName) { courseToUpdate.setName(newName); changed = true;}
    if(courseToUpdate.getCredits() != newCredits) { courseToUpdate.setCredits(newCredits); changed = true;}
    if(courseToUpdate.getFacultyId() != newFacultyId) { courseToUpdate.setFacultyId(newFacultyId); changed = true;}

    if(!changed) return true;

    auto updateResult = _courseDao->update(courseToUpdate);
     if (updateResult.has_value() && updateResult.value()){
        LOG_INFO("Course updated: ID=" + courseId);
    }
    return updateResult;
}

std::expected<bool, Error> CourseService::removeCourse(const std::string& courseId) {
    if (!_sessionContext->isAuthenticated() || !_sessionContext->getCurrentUserRole().has_value() || _sessionContext->getCurrentUserRole().value() != UserRole::ADMIN) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can remove courses."});
    }
    ValidationResult idVr = _inputValidator->validateIdFormat(courseId, "Course ID", 1, 20);
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);

    // Schema DB: Enrollments và CourseResults có ON DELETE CASCADE Courses(id).
    // Nếu muốn kiểm tra trước, có thể làm:
    auto enrollments = _enrollmentDao->findStudentIdsByCourseId(courseId);
    if(!enrollments.has_value()) return std::unexpected(enrollments.error());
    if(!enrollments.value().empty()){
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Cannot remove course. Students are still enrolled in it."});
    }
    auto results = _courseResultDao->findByCourseId(courseId);
    if(!results.has_value()) return std::unexpected(results.error());
    if(!results.value().empty()){
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Cannot remove course. It has existing student results."});
    }
    // Nếu các kiểm tra trên pass (hoặc bạn chấp nhận ON DELETE CASCADE), thì tiến hành xóa.

    auto removeResult = _courseDao->remove(courseId);
    if (removeResult.has_value() && removeResult.value()){
        LOG_INFO("Course removed: ID=" + courseId);
    }
    return removeResult;
}