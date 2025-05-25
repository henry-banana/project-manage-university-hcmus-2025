#include "StudentService.h"
#include "../../../utils/Logger.h"
#include "../../../utils/StringUtils.h"

StudentService::StudentService(std::shared_ptr<IStudentDao> studentDao,
                               std::shared_ptr<ITeacherDao> teacherDao,
                               std::shared_ptr<IFacultyDao> facultyDao,
                               std::shared_ptr<IGeneralInputValidator> inputValidator,
                               std::shared_ptr<SessionContext> sessionContext) // (➕)
    : _studentDao(std::move(studentDao)),
      _teacherDao(std::move(teacherDao)),
      _facultyDao(std::move(facultyDao)),
      _inputValidator(std::move(inputValidator)),
      _sessionContext(std::move(sessionContext)) { // (➕)
    if (!_studentDao) throw std::invalid_argument("StudentDao cannot be null for StudentService.");
    if (!_facultyDao) throw std::invalid_argument("FacultyDao cannot be null for StudentService.");
    if (!_inputValidator) throw std::invalid_argument("InputValidator cannot be null for StudentService.");
    if (!_sessionContext) throw std::invalid_argument("SessionContext cannot be null for StudentService."); // (➕)
}

std::expected<Student, Error> StudentService::getStudentDetails(const std::string& studentId) const {
    if (studentId.empty()) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Student ID cannot be empty."});
    }
    // Kiểm tra quyền: User hiện tại có phải là student đó hoặc admin không?
    if (!_sessionContext->isAuthenticated()) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    auto currentUserIdOpt = _sessionContext->getCurrentUserId();
    auto currentUserRoleOpt = _sessionContext->getCurrentUserRole();

    bool canAccess = false;
    if (currentUserRoleOpt.has_value() && currentUserRoleOpt.value() == UserRole::ADMIN) {
        canAccess = true;
    } else if (currentUserIdOpt.has_value() && currentUserIdOpt.value() == studentId) {
        canAccess = true;
    }

    if (!canAccess) {
         return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "You do not have permission to view these student details."});
    }

    return _studentDao->getById(studentId);
}

std::expected<std::vector<Student>, Error> StudentService::getAllStudents() const {
    // Chỉ Admin mới có quyền xem tất cả sinh viên
    if (!_sessionContext->isAuthenticated() || !_sessionContext->getCurrentUserRole().has_value() || _sessionContext->getCurrentUserRole().value() != UserRole::ADMIN) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can view all students."});
    }
    return _studentDao->getAll();
}

std::expected<std::vector<Student>, Error> StudentService::getStudentsByFaculty(const std::string& facultyId) const {
    if (facultyId.empty()) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Faculty ID cannot be empty."});
    }
    // Giả sử ai cũng có thể xem sinh viên theo khoa (nếu cần hạn chế thì thêm kiểm tra quyền)
    return _studentDao->findByFacultyId(facultyId);
}

std::expected<bool, Error> StudentService::updateStudentDetails(const StudentUpdateData& data) {
    if (!_sessionContext->isAuthenticated()) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    auto currentUserIdOpt = _sessionContext->getCurrentUserId();
    auto currentUserRoleOpt = _sessionContext->getCurrentUserRole();

    bool canUpdate = false;
    if (currentUserRoleOpt.has_value() && currentUserRoleOpt.value() == UserRole::ADMIN) {
        canUpdate = true;
    } else if (currentUserIdOpt.has_value() && currentUserIdOpt.value() == data.studentIdToUpdate) {
        // Sinh viên chỉ được update một số trường của chính mình, không được đổi facultyId, role, status.
        // Struct StudentUpdateData đã không cho phép update role, status.
        // Nếu là student tự update, không cho phép update facultyId.
        if (data.facultyId.has_value()) {
             return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Students cannot change their own faculty ID."});
        }
        canUpdate = true;
    }

    if (!canUpdate) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "You do not have permission to update this student's details."});
    }

    auto studentResult = _studentDao->getById(data.studentIdToUpdate);
    if (!studentResult.has_value()) {
        return std::unexpected(studentResult.error());
    }
    Student studentToUpdate = studentResult.value();
    bool changed = false;
    ValidationResult vrTotal;
    auto appendErrors = [&](const ValidationResult& vr) {
        for(const auto& err : vr.errors) vrTotal.addError(err);
    };

    if (data.firstName.has_value()) {
        appendErrors(_inputValidator->validateRequiredString(data.firstName.value(), "First Name", 50));
        if (vrTotal.isValid && studentToUpdate.setFirstName(data.firstName.value())) changed = true;
    }
    if (data.lastName.has_value()) {
        appendErrors(_inputValidator->validateRequiredString(data.lastName.value(), "Last Name", 50));
        if (vrTotal.isValid && studentToUpdate.setLastName(data.lastName.value())) changed = true;
    }
    if (data.birthday.has_value()) {
        appendErrors(_inputValidator->validateDate(data.birthday.value().getDay(), data.birthday.value().getMonth(), data.birthday.value().getYear(), "Birthday"));
        if (vrTotal.isValid && studentToUpdate.setBirthday(data.birthday.value())) changed = true;
    }
    if (data.address.has_value()) {
        appendErrors(_inputValidator->validateOptionalString(data.address.value(), "Address", 200));
        if (vrTotal.isValid && studentToUpdate.setAddress(data.address.value())) changed = true;
    }
    if (data.citizenId.has_value()) {
        if (currentUserRoleOpt.value() != UserRole::ADMIN) { // Chỉ Admin được sửa CitizenID
            return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only Admin can update Citizen ID."});
        }
        if (StringUtils::trim(data.citizenId.value()).empty()){
             vrTotal.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID cannot be set to empty.");
        } else {
            appendErrors(_inputValidator->validateCitizenId(data.citizenId.value()));
        }
        if (vrTotal.isValid && studentToUpdate.setCitizenId(data.citizenId.value())) changed = true;
    }
    if (data.email.has_value()) {
        if (StringUtils::trim(data.email.value()).empty()){
            vrTotal.addError(ErrorCode::VALIDATION_ERROR, "Email cannot be set to empty.");
        } else {
            appendErrors(_inputValidator->validateEmail(data.email.value()));
        }
        if (vrTotal.isValid) {
            auto studentByEmail = _studentDao->findByEmail(data.email.value());
            if (studentByEmail.has_value() && studentByEmail.value().getId() != studentToUpdate.getId()) {
                vrTotal.addError(ErrorCode::ALREADY_EXISTS, "Email " + data.email.value() + " is already in use by another student.");
            } else if (studentByEmail.error().code != ErrorCode::NOT_FOUND) {
                 return std::unexpected(studentByEmail.error());
            }
            // (➕) Kiểm tra email có trùng với Teacher không
            auto teacherByEmail = _teacherDao->findByEmail(data.email.value());
            if(teacherByEmail.has_value()){ // Teacher không cần check ID vì đang update Student
                 vrTotal.addError(ErrorCode::ALREADY_EXISTS, "Email " + data.email.value() + " is already in use by a teacher.");
            } else if(teacherByEmail.error().code != ErrorCode::NOT_FOUND){
                return std::unexpected(teacherByEmail.error());
            }

            if (vrTotal.isValid && studentToUpdate.setEmail(data.email.value())) changed = true;
        }
    }
    if (data.phoneNumber.has_value()) {
        if (!StringUtils::trim(data.phoneNumber.value()).empty()){
            appendErrors(_inputValidator->validatePhoneNumber(data.phoneNumber.value()));
        }
        if (vrTotal.isValid && studentToUpdate.setPhoneNumber(data.phoneNumber.value())) changed = true;
    }
    if (data.facultyId.has_value()) {
        if (currentUserRoleOpt.value() != UserRole::ADMIN) { // Chỉ Admin được sửa FacultyID
            return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only Admin can update Faculty ID."});
        }
        appendErrors(_inputValidator->validateRequiredString(data.facultyId.value(), "Faculty ID", 10));
        if (vrTotal.isValid) {
            auto facultyExists = _facultyDao->exists(data.facultyId.value());
            if (!facultyExists.has_value()) return std::unexpected(facultyExists.error());
            if (!facultyExists.value()) {
                 vrTotal.addError(ErrorCode::NOT_FOUND, "Faculty ID '" + data.facultyId.value() + "' does not exist.");
            }
            if (vrTotal.isValid && studentToUpdate.setFacultyId(data.facultyId.value())) changed = true;
        }
    }

    if (!vrTotal.isValid) {
        return std::unexpected(vrTotal.errors[0]);
    }
    if (!changed) {
        return true;
    }
    
    ValidationResult finalVr = studentToUpdate.validateBasic();
    if (!finalVr.isValid) {
        LOG_ERROR("StudentService: Update failed due to final entity validation: " + finalVr.getErrorMessagesCombined());
        return std::unexpected(finalVr.errors[0]);
    }

    auto updateResult = _studentDao->update(studentToUpdate);
    if (!updateResult.has_value() || !updateResult.value()) {
        return std::unexpected(updateResult.has_value() ? Error{ErrorCode::OPERATION_FAILED, "Failed to update student details in database."} : updateResult.error());
    }

    LOG_INFO("Student details updated successfully for ID: " + data.studentIdToUpdate);
    return true;
}