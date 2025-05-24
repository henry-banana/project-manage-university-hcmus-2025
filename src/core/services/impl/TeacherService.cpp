#include "TeacherService.h"
#include "../../../utils/Logger.h"
#include "../../../utils/StringUtils.h"

TeacherService::TeacherService(std::shared_ptr<ITeacherDao> teacherDao,
                               std::shared_ptr<IStudentDao> studentDao, // (➕)
                               std::shared_ptr<IFacultyDao> facultyDao,
                               std::shared_ptr<IGeneralInputValidator> inputValidator,
                               std::shared_ptr<SessionContext> sessionContext) // (➕)
    : _teacherDao(std::move(teacherDao)),
      _studentDao(std::move(studentDao)), // (➕)
      _facultyDao(std::move(facultyDao)),
      _inputValidator(std::move(inputValidator)),
      _sessionContext(std::move(sessionContext)) { // (➕)
    if (!_teacherDao) throw std::invalid_argument("TeacherDao cannot be null for TeacherService.");
    if (!_studentDao) throw std::invalid_argument("StudentDao cannot be null for TeacherService."); // (➕)
    if (!_facultyDao) throw std::invalid_argument("FacultyDao cannot be null for TeacherService.");
    if (!_inputValidator) throw std::invalid_argument("InputValidator cannot be null for TeacherService.");
    if (!_sessionContext) throw std::invalid_argument("SessionContext cannot be null for TeacherService."); // (➕)
}

std::expected<Teacher, Error> TeacherService::getTeacherDetails(const std::string& teacherId) const {
    if (teacherId.empty()) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Teacher ID cannot be empty."});
    }
    // Mọi người dùng đã xác thực đều có thể xem thông tin giáo viên (trừ thông tin nhạy cảm nếu có)
    if (!_sessionContext->isAuthenticated()) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    return _teacherDao->getById(teacherId);
}

std::expected<std::vector<Teacher>, Error> TeacherService::getAllTeachers() const {
    // Mọi người dùng đã xác thực đều có thể xem danh sách giáo viên
     if (!_sessionContext->isAuthenticated()) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    return _teacherDao->getAll();
}

std::expected<std::vector<Teacher>, Error> TeacherService::getTeachersByFaculty(const std::string& facultyId) const {
     if (facultyId.empty()) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Faculty ID cannot be empty."});
    }
    if (!_sessionContext->isAuthenticated()) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    return _teacherDao->findByFacultyId(facultyId);
}

std::expected<std::vector<Teacher>, Error> TeacherService::getTeachersByDesignation(const std::string& designation) const {
    if (designation.empty()) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Designation cannot be empty."});
    }
     if (!_sessionContext->isAuthenticated()) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    return _teacherDao->findByDesignation(designation);
}

std::expected<bool, Error> TeacherService::updateTeacherDetails(const TeacherUpdateData& data) {
    if (!_sessionContext->isAuthenticated()) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    auto currentUserIdOpt = _sessionContext->getCurrentUserId();
    auto currentUserRoleOpt = _sessionContext->getCurrentUserRole();

    bool canUpdate = false;
    if (currentUserRoleOpt.has_value() && currentUserRoleOpt.value() == UserRole::ADMIN) {
        canUpdate = true;
    } else if (currentUserIdOpt.has_value() && currentUserIdOpt.value() == data.teacherIdToUpdate) {
        // Teacher chỉ được update một số trường của chính mình.
        if (data.facultyId.has_value() || data.designation.has_value() || data.experienceYears.has_value()) { // Ví dụ: không cho GV tự đổi khoa, chức vụ, kinh nghiệm
             return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Teachers cannot change their own faculty, designation, or experience years."});
        }
        canUpdate = true;
    }

    if (!canUpdate) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "You do not have permission to update this teacher's details."});
    }

    auto teacherResult = _teacherDao->getById(data.teacherIdToUpdate);
    if (!teacherResult.has_value()) {
        return std::unexpected(teacherResult.error());
    }
    Teacher teacherToUpdate = teacherResult.value();
    bool changed = false;
    ValidationResult vrTotal;
    auto appendErrors = [&](const ValidationResult& vr) {
        for(const auto& err : vr.errors) vrTotal.addError(err);
    };

    if (data.firstName.has_value()) {
        appendErrors(_inputValidator->validateRequiredString(data.firstName.value(), "First Name", 50));
        if (vrTotal.isValid && teacherToUpdate.setFirstName(data.firstName.value())) changed = true;
    }
    // Tương tự cho các trường khác...
    if (data.lastName.has_value()) {
        appendErrors(_inputValidator->validateRequiredString(data.lastName.value(), "Last Name", 50));
        if (vrTotal.isValid && teacherToUpdate.setLastName(data.lastName.value())) changed = true;
    }
     if (data.birthday.has_value()) {
        appendErrors(_inputValidator->validateDate(data.birthday.value().getDay(), data.birthday.value().getMonth(), data.birthday.value().getYear(), "Birthday"));
        if (vrTotal.isValid && teacherToUpdate.setBirthday(data.birthday.value())) changed = true;
    }
    if (data.address.has_value()) {
        appendErrors(_inputValidator->validateOptionalString(data.address.value(), "Address", 200));
        if (vrTotal.isValid && teacherToUpdate.setAddress(data.address.value())) changed = true;
    }
    if (data.citizenId.has_value()) {
         if (currentUserRoleOpt.value() != UserRole::ADMIN) {
            return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only Admin can update Citizen ID."});
        }
        if (StringUtils::trim(data.citizenId.value()).empty()){
             vrTotal.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID cannot be set to empty.");
        } else {
            appendErrors(_inputValidator->validateCitizenId(data.citizenId.value()));
        }
        if (vrTotal.isValid && teacherToUpdate.setCitizenId(data.citizenId.value())) changed = true;
    }
    if (data.email.has_value()) {
        if (StringUtils::trim(data.email.value()).empty()){
            vrTotal.addError(ErrorCode::VALIDATION_ERROR, "Email cannot be set to empty.");
        } else {
            appendErrors(_inputValidator->validateEmail(data.email.value()));
        }
        if (vrTotal.isValid) {
            auto teacherByEmail = _teacherDao->findByEmail(data.email.value());
            if (teacherByEmail.has_value() && teacherByEmail.value().getId() != teacherToUpdate.getId()) {
                vrTotal.addError(ErrorCode::ALREADY_EXISTS, "Email " + data.email.value() + " is already in use by another teacher.");
            } else if(teacherByEmail.error().code != ErrorCode::NOT_FOUND) {
                return std::unexpected(teacherByEmail.error());
            }
            // (➕) Kiểm tra email có trùng với Student không
            auto studentByEmail = _studentDao->findByEmail(data.email.value());
            if(studentByEmail.has_value()){
                 vrTotal.addError(ErrorCode::ALREADY_EXISTS, "Email " + data.email.value() + " is already in use by a student.");
            } else if(studentByEmail.error().code != ErrorCode::NOT_FOUND){
                return std::unexpected(studentByEmail.error());
            }
            if (vrTotal.isValid && teacherToUpdate.setEmail(data.email.value())) changed = true;
        }
    }
    if (data.phoneNumber.has_value()) {
         if (!StringUtils::trim(data.phoneNumber.value()).empty()){
            appendErrors(_inputValidator->validatePhoneNumber(data.phoneNumber.value()));
        }
        if (vrTotal.isValid && teacherToUpdate.setPhoneNumber(data.phoneNumber.value())) changed = true;
    }
    if (data.facultyId.has_value()) {
        if (currentUserRoleOpt.value() != UserRole::ADMIN) {
            return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only Admin can update Faculty ID."});
        }
        appendErrors(_inputValidator->validateRequiredString(data.facultyId.value(), "Faculty ID", 10));
        if (vrTotal.isValid) {
            auto facultyExists = _facultyDao->exists(data.facultyId.value());
            if (!facultyExists.has_value()) return std::unexpected(facultyExists.error());
            if (!facultyExists.value()) {
                 vrTotal.addError(ErrorCode::NOT_FOUND, "Faculty ID '" + data.facultyId.value() + "' does not exist.");
            }
            if (vrTotal.isValid && teacherToUpdate.setFacultyId(data.facultyId.value())) changed = true;
        }
    }
    if (data.qualification.has_value()) {
        appendErrors(_inputValidator->validateOptionalString(data.qualification.value(), "Qualification", 100));
        if (vrTotal.isValid && teacherToUpdate.setQualification(data.qualification.value())) changed = true;
    }
    if (data.specializationSubjects.has_value()) {
        appendErrors(_inputValidator->validateOptionalString(data.specializationSubjects.value(), "Specialization Subjects", 255));
        if (vrTotal.isValid && teacherToUpdate.setSpecializationSubjects(data.specializationSubjects.value())) changed = true;
    }
    if (data.designation.has_value()) {
         if (currentUserRoleOpt.value() != UserRole::ADMIN) {
            return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only Admin can update Designation."});
        }
        appendErrors(_inputValidator->validateOptionalString(data.designation.value(), "Designation", 50));
        if (vrTotal.isValid && teacherToUpdate.setDesignation(data.designation.value())) changed = true;
    }
    if (data.experienceYears.has_value()) {
        if (currentUserRoleOpt.value() != UserRole::ADMIN) {
            return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only Admin can update Experience Years."});
        }
        appendErrors(_inputValidator->validateInteger(data.experienceYears.value(), "Experience Years", 0, 70));
        if (vrTotal.isValid && teacherToUpdate.setExperienceYears(data.experienceYears.value())) changed = true;
    }
    
    if (!vrTotal.isValid) {
        return std::unexpected(vrTotal.errors[0]);
    }
    if (!changed) {
        return true;
    }

    ValidationResult finalVr = teacherToUpdate.validateBasic();
    if (!finalVr.isValid) {
        LOG_ERROR("TeacherService: Update failed due to final entity validation: " + finalVr.getErrorMessagesCombined());
        return std::unexpected(finalVr.errors[0]);
    }

    auto updateResult = _teacherDao->update(teacherToUpdate);
    if (!updateResult.has_value() || !updateResult.value()) {
        return std::unexpected(updateResult.has_value() ? Error{ErrorCode::OPERATION_FAILED, "Failed to update teacher details."} : updateResult.error());
    }
    LOG_INFO("Teacher details updated successfully for ID: " + data.teacherIdToUpdate);
    return true;
}