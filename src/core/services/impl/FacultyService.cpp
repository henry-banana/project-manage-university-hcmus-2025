#include "FacultyService.h"
#include "../../../utils/Logger.h"

FacultyService::FacultyService(std::shared_ptr<IFacultyDao> facultyDao,
                               std::shared_ptr<IStudentDao> studentDao,
                               std::shared_ptr<ITeacherDao> teacherDao,
                               std::shared_ptr<ICourseDao> courseDao,
                               std::shared_ptr<IGeneralInputValidator> inputValidator,
                               std::shared_ptr<SessionContext> sessionContext)
    : _facultyDao(std::move(facultyDao)),
      _studentDao(std::move(studentDao)),
      _teacherDao(std::move(teacherDao)),
      _courseDao(std::move(courseDao)),
      _inputValidator(std::move(inputValidator)),
      _sessionContext(std::move(sessionContext)) {
    if (!_facultyDao) throw std::invalid_argument("FacultyDao cannot be null.");
    if (!_studentDao) throw std::invalid_argument("StudentDao cannot be null.");
    if (!_teacherDao) throw std::invalid_argument("TeacherDao cannot be null.");
    if (!_courseDao) throw std::invalid_argument("CourseDao cannot be null.");
    if (!_inputValidator) throw std::invalid_argument("InputValidator cannot be null.");
    if (!_sessionContext) throw std::invalid_argument("SessionContext cannot be null.");
}

std::expected<Faculty, Error> FacultyService::getFacultyById(const std::string& facultyId) const {
    if (facultyId.empty()) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Faculty ID cannot be empty."});
    }
    // Ai cũng có thể xem thông tin khoa
    return _facultyDao->getById(facultyId);
}

std::expected<Faculty, Error> FacultyService::getFacultyByName(const std::string& name) const {
    if (name.empty()) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Faculty name cannot be empty."});
    }
    return _facultyDao->findByName(name);
}

std::expected<std::vector<Faculty>, Error> FacultyService::getAllFaculties() const {
    return _facultyDao->getAll();
}

std::expected<Faculty, Error> FacultyService::addFaculty(const std::string& id, const std::string& name) {
    if (!_sessionContext->isAuthenticated() || !_sessionContext->getCurrentUserRole().has_value() || _sessionContext->getCurrentUserRole().value() != UserRole::ADMIN) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can add faculties."});
    }

    ValidationResult idVr = _inputValidator->validateIdFormat(id, "Faculty ID", 1, 10);
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);

    ValidationResult nameVr = _inputValidator->validateRequiredString(name, "Faculty Name", 100);
    if (!nameVr.isValid) return std::unexpected(nameVr.errors[0]);

    // DAO sẽ kiểm tra trùng ID và tên trong hàm add của nó
    Faculty newFaculty(id, name);
    auto addResult = _facultyDao->add(newFaculty);
    if (addResult.has_value()) {
        LOG_INFO("Faculty added: ID=" + id + ", Name=" + name);
    }
    return addResult;
}

std::expected<bool, Error> FacultyService::updateFaculty(const std::string& facultyId, const std::string& newName) {
    if (!_sessionContext->isAuthenticated() || !_sessionContext->getCurrentUserRole().has_value() || _sessionContext->getCurrentUserRole().value() != UserRole::ADMIN) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can update faculties."});
    }

    ValidationResult idVr = _inputValidator->validateIdFormat(facultyId, "Faculty ID", 1, 10);
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);
    
    ValidationResult nameVr = _inputValidator->validateRequiredString(newName, "New Faculty Name", 100);
    if (!nameVr.isValid) return std::unexpected(nameVr.errors[0]);

    auto facultyResult = _facultyDao->getById(facultyId);
    if (!facultyResult.has_value()) {
        return std::unexpected(facultyResult.error());
    }
    Faculty facultyToUpdate = facultyResult.value();
    
    if (!facultyToUpdate.setName(newName)) { // setName có thể có validation riêng
         return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Invalid new name format for faculty."});
    }
    // DAO::update sẽ kiểm tra tên mới có trùng với faculty khác không.
    auto updateResult = _facultyDao->update(facultyToUpdate);
    if (updateResult.has_value() && updateResult.value()){
        LOG_INFO("Faculty updated: ID=" + facultyId + ", New Name=" + newName);
    }
    return updateResult;
}

std::expected<bool, Error> FacultyService::removeFaculty(const std::string& facultyId) {
     if (!_sessionContext->isAuthenticated() || !_sessionContext->getCurrentUserRole().has_value() || _sessionContext->getCurrentUserRole().value() != UserRole::ADMIN) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can remove faculties."});
    }
    ValidationResult idVr = _inputValidator->validateIdFormat(facultyId, "Faculty ID", 1, 10);
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);

    // Kiểm tra ràng buộc trước khi xóa
    // Schema hiện tại dùng ON DELETE SET NULL cho Courses, Students, Teachers khi Faculty bị xóa.
    // Nếu muốn hành vi RESTRICT, cần kiểm tra ở đây.
    // Ví dụ:
    auto studentsInFaculty = _studentDao->findByFacultyId(facultyId);
    if (!studentsInFaculty.has_value()) return std::unexpected(studentsInFaculty.error());
    if (!studentsInFaculty.value().empty()) {
       return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Cannot remove faculty. Students are still assigned to it. Please reassign them first."});
    }

    auto teachersInFaculty = _teacherDao->findByFacultyId(facultyId);
    if (!teachersInFaculty.has_value()) return std::unexpected(teachersInFaculty.error());
    if (!teachersInFaculty.value().empty()) {
       return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Cannot remove faculty. Teachers are still assigned to it. Please reassign them first."});
    }
    
    auto coursesInFaculty = _courseDao->findByFacultyId(facultyId);
    if (!coursesInFaculty.has_value()) return std::unexpected(coursesInFaculty.error());
    if (!coursesInFaculty.value().empty()) {
       return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Cannot remove faculty. Courses are still assigned to it. Please reassign or delete them first."});
    }
    // Nếu các kiểm tra trên pass, có thể xóa. Hoặc nếu schema là SET NULL, thì DAO remove sẽ tự xử lý.
    // Để an toàn, nếu bạn muốn đảm bảo không có gì tham chiếu đến, hãy thực hiện kiểm tra như trên.

    auto removeResult = _facultyDao->remove(facultyId);
    if (removeResult.has_value() && removeResult.value()){
        LOG_INFO("Faculty removed: ID=" + facultyId);
    }
    return removeResult;
}