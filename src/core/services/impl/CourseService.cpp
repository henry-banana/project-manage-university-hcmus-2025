#include "CourseService.h"
#include "../../../utils/Logger.h"

/**
 * @brief Khởi tạo đối tượng CourseService
 * 
 * @param courseDao Đối tượng truy cập dữ liệu khóa học
 * @param facultyDao Đối tượng truy cập dữ liệu khoa
 * @param enrollmentDao Đối tượng truy cập dữ liệu đăng ký khóa học
 * @param courseResultDao Đối tượng truy cập dữ liệu kết quả học tập
 * @param inputValidator Đối tượng kiểm tra đầu vào
 * @param sessionContext Đối tượng quản lý phiên đăng nhập
 * @throw std::invalid_argument Nếu bất kỳ đối số nào là nullptr
 */
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

/**
 * @brief Lấy thông tin chi tiết của một khóa học
 * 
 * Phương thức này trả về thông tin chi tiết của một khóa học dựa trên ID.
 * Bất kỳ người dùng nào cũng có thể xem thông tin khóa học.
 * 
 * @param courseId ID của khóa học cần lấy thông tin
 * @return std::expected<Course, Error> Đối tượng khóa học nếu thành công, hoặc lỗi nếu thất bại
 */
std::expected<Course, Error> CourseService::getCourseById(const std::string& courseId) const {
    if (courseId.empty()) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Course ID cannot be empty."});
    }
    // Ai cũng có thể xem thông tin khóa học
    return _courseDao->getById(courseId);
}

/**
 * @brief Lấy danh sách tất cả khóa học
 * 
 * Phương thức này trả về danh sách tất cả khóa học trong hệ thống.
 * Bất kỳ người dùng nào cũng có thể xem danh sách khóa học.
 * 
 * @return std::expected<std::vector<Course>, Error> Danh sách khóa học nếu thành công, hoặc lỗi nếu thất bại
 */
std::expected<std::vector<Course>, Error> CourseService::getAllCourses() const {
    // Ai cũng có thể xem
    return _courseDao->getAll();
}

/**
 * @brief Lấy danh sách khóa học theo khoa
 * 
 * Phương thức này trả về danh sách khóa học thuộc một khoa cụ thể.
 * Bất kỳ người dùng nào cũng có thể xem danh sách khóa học theo khoa.
 * 
 * @param facultyId ID của khoa cần lấy danh sách khóa học
 * @return std::expected<std::vector<Course>, Error> Danh sách khóa học nếu thành công, hoặc lỗi nếu thất bại
 */
std::expected<std::vector<Course>, Error> CourseService::getCoursesByFaculty(const std::string& facultyId) const {
     if (facultyId.empty()) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Faculty ID cannot be empty."});
    }
    return _courseDao->findByFacultyId(facultyId);
}

/**
 * @brief Thêm khóa học mới vào hệ thống
 * 
 * Phương thức này thêm một khóa học mới vào hệ thống với các thông tin cơ bản.
 * Yêu cầu quyền truy cập: chỉ admin mới có quyền thêm khóa học.
 * 
 * @param id ID của khóa học mới
 * @param name Tên của khóa học
 * @param credits Số tín chỉ của khóa học
 * @param facultyId ID của khoa mà khóa học thuộc về
 * @return std::expected<Course, Error> Đối tượng khóa học mới nếu thành công, hoặc lỗi nếu thất bại
 */
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

/**
 * @brief Cập nhật thông tin khóa học
 * 
 * Phương thức này cập nhật thông tin của một khóa học hiện có trong hệ thống.
 * Yêu cầu quyền truy cập: chỉ admin mới có quyền cập nhật khóa học.
 * 
 * @param courseId ID của khóa học cần cập nhật
 * @param newName Tên mới của khóa học
 * @param newCredits Số tín chỉ mới của khóa học
 * @param newFacultyId ID mới của khoa mà khóa học thuộc về
 * @return std::expected<bool, Error> true nếu cập nhật thành công, hoặc lỗi nếu thất bại
 */
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

/**
 * @brief Xóa khóa học khỏi hệ thống
 * 
 * Phương thức này xóa một khóa học khỏi hệ thống, kiểm tra xem có sinh viên nào đang đăng ký hoặc có kết quả học tập nào không.
 * Yêu cầu quyền truy cập: chỉ admin mới có quyền xóa khóa học.
 * 
 * @param courseId ID của khóa học cần xóa
 * @return std::expected<bool, Error> true nếu xóa thành công, hoặc lỗi nếu thất bại
 */
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