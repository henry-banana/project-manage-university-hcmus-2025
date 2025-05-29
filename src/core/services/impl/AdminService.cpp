#include "AdminService.h"
#include "../../../utils/Logger.h"
#include "../../../utils/StringUtils.h"
#include <random>

/**
 * @brief Khởi tạo đối tượng AdminService
 * 
 * @param studentDao Đối tượng truy cập dữ liệu sinh viên
 * @param teacherDao Đối tượng truy cập dữ liệu giảng viên
 * @param loginDao Đối tượng truy cập dữ liệu đăng nhập
 * @param feeDao Đối tượng truy cập dữ liệu học phí
 * @param salaryDao Đối tượng truy cập dữ liệu lương
 * @param enrollmentDao Đối tượng truy cập dữ liệu đăng ký khóa học
 * @param courseResultDao Đối tượng truy cập dữ liệu kết quả học tập
 * @param inputValidator Đối tượng kiểm tra đầu vào
 * @param sessionContext Đối tượng quản lý phiên đăng nhập
 * @throw std::invalid_argument Nếu bất kỳ đối số nào là nullptr
 */
AdminService::AdminService(
    std::shared_ptr<IStudentDao> studentDao,
    std::shared_ptr<ITeacherDao> teacherDao,
    std::shared_ptr<IFacultyDao> facultyDao,
    std::shared_ptr<ILoginDao> loginDao,
    std::shared_ptr<IFeeRecordDao> feeDao,
    std::shared_ptr<ISalaryRecordDao> salaryDao,
    std::shared_ptr<IEnrollmentDao> enrollmentDao,
    std::shared_ptr<ICourseResultDao> courseResultDao,
    std::shared_ptr<IGeneralInputValidator> inputValidator,
    std::shared_ptr<SessionContext> sessionContext)
    : _studentDao(std::move(studentDao)),
      _teacherDao(std::move(teacherDao)),
      _facultyDao(std::move(facultyDao)),
      _loginDao(std::move(loginDao)),
      _feeDao(std::move(feeDao)),
      _salaryDao(std::move(salaryDao)),
      _enrollmentDao(std::move(enrollmentDao)),
      _courseResultDao(std::move(courseResultDao)),
      _inputValidator(std::move(inputValidator)),
      _sessionContext(std::move(sessionContext)) {
    // Kiểm tra null cho tất cả dependencies
    if (!_studentDao || !_teacherDao || !_loginDao || !_feeDao || !_salaryDao || 
        !_enrollmentDao || !_courseResultDao || !_inputValidator || !_sessionContext) {
        throw std::invalid_argument("One or more DAO/Validator/SessionContext is null for AdminService.");
    }
}

/**
 * @brief Kiểm tra xem người dùng hiện tại có phải là admin đã đăng nhập hay không
 * 
 * Phương thức này kiểm tra ba điều kiện:
 * 1. Người dùng đã đăng nhập
 * 2. Người dùng có vai trò
 * 3. Vai trò của người dùng là ADMIN
 * 
 * @return true Nếu người dùng hiện tại là admin đã đăng nhập
 * @return false Nếu người dùng chưa đăng nhập hoặc không phải admin
 */
bool AdminService::isAdminAuthenticated() const {
    return _sessionContext->isAuthenticated() && 
           _sessionContext->getCurrentUserRole().has_value() &&
           _sessionContext->getCurrentUserRole().value() == UserRole::ADMIN;
}

/**
 * @brief Phê duyệt đăng ký sinh viên
 * 
 * Phương thức này chuyển trạng thái sinh viên từ PENDING_APPROVAL sang ACTIVE
 * và vai trò từ PENDING_STUDENT sang STUDENT. Đồng thời tạo bản ghi học phí 
 * ban đầu cho sinh viên được phê duyệt.
 * Yêu cầu quyền truy cập: chỉ admin mới có quyền phê duyệt đăng ký sinh viên.
 * 
 * @param studentIdToApprove ID của sinh viên cần phê duyệt
 * @return std::expected<bool, Error> true nếu phê duyệt thành công, hoặc lỗi nếu thất bại
 */
std::expected<bool, Error> AdminService::approveStudentRegistration(const std::string& studentIdToApprove) {
    if (!isAdminAuthenticated()) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can approve student registrations."});
    }
    ValidationResult idVr = _inputValidator->validateIdFormat(studentIdToApprove, "Student ID");
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);

    auto studentExp = _studentDao->getById(studentIdToApprove);
    if (!studentExp.has_value()){
        return std::unexpected(studentExp.error());
    }
    Student student = studentExp.value();

    if (student.getStatus() != LoginStatus::PENDING_APPROVAL || student.getRole() != UserRole::PENDING_STUDENT) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Student is not pending approval."});
    }

    // Cập nhật trong cả StudentDao (bảng Users) và LoginDao (bảng Users)
    // Các DAO nên có hàm updateStatusAndRole hoặc LoginDao có hàm updateUserRoleAndStatus
    auto updateStatusRoleRes = _loginDao->updateUserRoleAndStatus(studentIdToApprove, UserRole::STUDENT, LoginStatus::ACTIVE);
    if(!updateStatusRoleRes.has_value() || !updateStatusRoleRes.value()){
         return std::unexpected(updateStatusRoleRes.has_value() ? 
            Error{ErrorCode::OPERATION_FAILED, "Failed to update student role and status."} :
            updateStatusRoleRes.error());
    }
    
    // Tạo FeeRecord ban đầu cho sinh viên (ví dụ học phí cố định hoặc 0)
    // Điều này nên được cấu hình hoặc nhập liệu
    long initialFee = 5000000; // Ví dụ
    auto feeRecordResult = _feeDao->add(FeeRecord(studentIdToApprove, initialFee, 0));
    if (!feeRecordResult.has_value()) {
        LOG_WARN("Student " + studentIdToApprove + " approved, but failed to create initial fee record: " + feeRecordResult.error().message);
        // Không coi đây là lỗi chặn việc approve, nhưng cần log lại. Admin có thể tạo sau.
    }


    LOG_INFO("Student registration approved for ID: " + studentIdToApprove);
    return true;
}

/**
 * @brief Lấy danh sách sinh viên theo trạng thái
 * 
 * Phương thức này trả về danh sách sinh viên có trạng thái đăng nhập cụ thể
 * (ví dụ: ACTIVE, PENDING_APPROVAL, DISABLED).
 * Yêu cầu quyền truy cập: chỉ admin mới có quyền xem danh sách sinh viên theo trạng thái.
 * 
 * @param status Trạng thái đăng nhập cần lọc sinh viên
 * @return std::expected<std::vector<Student>, Error> Danh sách sinh viên nếu thành công, hoặc lỗi nếu thất bại
 */
std::expected<std::vector<Student>, Error> AdminService::getStudentsByStatus(LoginStatus status) const {
    if (!isAdminAuthenticated()) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can view students by status."});
    }
    return _studentDao->findByStatus(status);
}

/**
 * @brief Thêm sinh viên mới bởi admin
 * 
 * Phương thức này tạo một tài khoản sinh viên mới với trạng thái ACTIVE, 
 * đặt mật khẩu ban đầu và tạo bản ghi học phí. Mã sinh viên được tạo tự động
 * dựa trên CMND/CCCD và số ngẫu nhiên.
 * Yêu cầu quyền truy cập: chỉ admin mới có quyền thêm sinh viên.
 * 
 * @param data Dữ liệu sinh viên mới và mật khẩu ban đầu
 * @return std::expected<Student, Error> Đối tượng sinh viên mới nếu thành công, hoặc lỗi nếu thất bại
 */
std::expected<Student, Error> AdminService::addStudentByAdmin(const NewStudentDataByAdmin& data) {
    if (!isAdminAuthenticated()) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can add students."});
    }
    // Validate tương tự AuthService::registerStudent, nhưng có thể bỏ qua một số bước nếu admin được tin cậy hơn
    // hoặc set status là ACTIVE ngay.
    // Tạm thời, quy trình giống register nhưng set ACTIVE.

    ValidationResult vrTotal;
    auto appendErrors = [&](const ValidationResult& vr) {
        for(const auto& err : vr.errors) vrTotal.addError(err);
    };
    const auto& studentInfo = data.studentInfo;
    appendErrors(_inputValidator->validateEmail(studentInfo.email));
    appendErrors(_inputValidator->validatePasswordComplexity(data.initialPassword)); // Validate password admin đặt
    // ... (các validate khác cho studentInfo như trong AuthService::registerStudent) ...
    if (StringUtils::trim(studentInfo.citizenId).empty()){
        vrTotal.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID is required.");
    } else {
        appendErrors(_inputValidator->validateCitizenId(studentInfo.citizenId));
    }
     if (!vrTotal.isValid) return std::unexpected(vrTotal.errors[0]);


    auto studentByEmail = _studentDao->findByEmail(studentInfo.email);
    if (studentByEmail.has_value()) return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Email already registered."});
    if (studentByEmail.error().code != ErrorCode::NOT_FOUND) return std::unexpected(studentByEmail.error());

    std::string studentId;
    // Lấy năm hiện tại
    auto nowChrono = std::chrono::system_clock::now(); // Đổi tên biến để tránh trùng
    auto now_c = std::chrono::system_clock::to_time_t(nowChrono);
    std::tm now_tm = {};
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&now_tm, &now_c);
    #else
        localtime_r(&now_c, &now_tm);
    #endif
    std::string yearPrefix = std::to_string((now_tm.tm_year + 1900) % 100);

    auto facultyDetails = _facultyDao->getById(data.studentInfo.facultyId);
    if (!facultyDetails.has_value()) { /* Đã check ở trên, nhưng để an toàn */
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "Faculty details not found for ID: " + data.studentInfo.facultyId});
    }

    std::string idPrefix = yearPrefix + data.studentInfo.facultyId; 
    bool idOk = false;
    int maxRetriesForIdGeneration = 1000; // Tăng số lần thử để tìm số thứ tự
    for (int sequence = 1; sequence <= 9999; ++sequence) {
        std::ostringstream oss;
        oss << idPrefix << std::setfill('0') << std::setw(4) << sequence;
        studentId = oss.str();
        // Kiểm tra xem studentId đã tồn tại trong bảng Users hay chưa
        auto userExists = _loginDao->findCredentialsByUserId(studentId); 
        if (!userExists.has_value()) {
            if (userExists.error().code == ErrorCode::NOT_FOUND) {
                idOk = true; 
                break;
            } else {
                LOG_ERROR("Error checking student ID existence: " + userExists.error().message);
                return std::unexpected(userExists.error()); // Lỗi DB
            }
        }
        if (sequence >= maxRetriesForIdGeneration && !idOk) { // Giới hạn số lần thử
             LOG_WARN("Student ID generation: Max retries reached for prefix " + idPrefix);
             break; // Dừng nếu thử quá nhiều
        }
    }

    if (!idOk) {
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Could not generate unique student ID for prefix " + idPrefix + ". Sequence may be exhausted or DB error occurred."});
    }
    Student newStudent(studentId, studentInfo.firstName, studentInfo.lastName, studentInfo.facultyId, LoginStatus::ACTIVE); // ACTIVE ngay
    newStudent.setBirthday(studentInfo.birthDay, studentInfo.birthMonth, studentInfo.birthYear);
    newStudent.setAddress(studentInfo.address);
    newStudent.setCitizenId(studentInfo.citizenId);
    newStudent.setEmail(studentInfo.email);
    newStudent.setPhoneNumber(studentInfo.phoneNumber);
    newStudent.setRole(UserRole::STUDENT); // Role STUDENT ngay

    auto addStudentResult = _studentDao->add(newStudent);
    if (!addStudentResult.has_value()) return std::unexpected(addStudentResult.error());

    std::string salt = PasswordUtils::generateSalt();
    std::string hashedPassword = PasswordUtils::hashPassword(data.initialPassword, salt);
    auto addCredsResult = _loginDao->addUserCredentials(studentId, hashedPassword, salt, UserRole::STUDENT, LoginStatus::ACTIVE);
    if (!addCredsResult.has_value() || !addCredsResult.value()) {
        _studentDao->remove(studentId); // Rollback
        return std::unexpected(addCredsResult.has_value() ? Error{ErrorCode::OPERATION_FAILED, "Failed to set credentials."} : addCredsResult.error());
    }

    // Tạo FeeRecord ban đầu
    long initialFee = 5000000; // Ví dụ
    auto feeRecordResult = _feeDao->add(FeeRecord(studentId, initialFee, 0));
     if (!feeRecordResult.has_value()) {
        LOG_WARN("Student " + studentId + " added by admin, but failed to create initial fee record: " + feeRecordResult.error().message);
        // Không rollback student, admin có thể tạo fee sau
    }

    LOG_INFO("Student " + studentId + " added by admin successfully.");
    return addStudentResult.value();
}

/**
 * @brief Xóa tài khoản sinh viên
 * 
 * Phương thức này xóa tài khoản sinh viên và tất cả dữ liệu liên quan
 * (thông tin đăng nhập, đăng ký khóa học, kết quả học tập, học phí).
 * Yêu cầu quyền truy cập: chỉ admin mới có quyền xóa tài khoản sinh viên.
 * 
 * @param studentId ID của sinh viên cần xóa
 * @return std::expected<bool, Error> true nếu xóa thành công, hoặc lỗi nếu thất bại
 */
std::expected<bool, Error> AdminService::removeStudentAccount(const std::string& studentId) {
    if (!isAdminAuthenticated()) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can remove student accounts."});
    }
    ValidationResult idVr = _inputValidator->validateIdFormat(studentId, "Student ID");
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);

    // DAO Student::remove sẽ xóa User, và DB schema sẽ cascade xóa Students, Logins, Enrollments, CourseResults, FeeRecords
    // Nên chỉ cần gọi _studentDao->remove(studentId)
    auto removeResult = _studentDao->remove(studentId);
    if (removeResult.has_value() && removeResult.value()) {
        LOG_INFO("Student account removed: " + studentId);
    }
    return removeResult;
}


/**
 * @brief Thêm giảng viên mới bởi admin
 * 
 * Phương thức này tạo một tài khoản giảng viên mới với trạng thái ACTIVE,
 * đặt mật khẩu ban đầu và tạo bản ghi lương. Mức lương ban đầu được tính
 * dựa vào số năm kinh nghiệm.
 * Yêu cầu quyền truy cập: chỉ admin mới có quyền thêm giảng viên.
 * 
 * @param data Dữ liệu giảng viên mới và mật khẩu ban đầu
 * @return std::expected<Teacher, Error> Đối tượng giảng viên mới nếu thành công, hoặc lỗi nếu thất bại
 */
std::expected<Teacher, Error> AdminService::addTeacherByAdmin(const NewTeacherDataByAdmin& data) {
    if (!isAdminAuthenticated()) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can add teachers."});
    }
     ValidationResult vrTotal;
    auto appendErrors = [&](const ValidationResult& vr) {
        for(const auto& err : vr.errors) vrTotal.addError(err);
    };

    appendErrors(_inputValidator->validateIdFormat(data.id, "Teacher ID", 3, 20));
    appendErrors(_inputValidator->validateEmail(data.email));
    appendErrors(_inputValidator->validatePasswordComplexity(data.initialPassword));
    // ... (validate các trường khác của NewTeacherDataByAdmin) ...
    if (StringUtils::trim(data.citizenId).empty()){
        vrTotal.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID is required for teacher.");
    } else {
        appendErrors(_inputValidator->validateCitizenId(data.citizenId));
    }
    if (!vrTotal.isValid) return std::unexpected(vrTotal.errors[0]);

    auto teacherById = _teacherDao->exists(data.id);
    if(!teacherById.has_value()) return std::unexpected(teacherById.error());
    if(teacherById.value()) return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Teacher ID '" + data.id + "' already exists."});
    
    auto teacherByEmail = _teacherDao->findByEmail(data.email);
    if (teacherByEmail.has_value()) return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Email '" + data.email + "' already registered for a teacher."});
    if (teacherByEmail.error().code != ErrorCode::NOT_FOUND) return std::unexpected(teacherByEmail.error());

    auto studentByEmail = _studentDao->findByEmail(data.email);
    if (studentByEmail.has_value()) return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Email '" + data.email + "' already registered for a student."});
    if (studentByEmail.error().code != ErrorCode::NOT_FOUND) return std::unexpected(studentByEmail.error());


    Teacher newTeacher(data.id, data.firstName, data.lastName, data.facultyId, LoginStatus::ACTIVE);
    newTeacher.setBirthday(data.birthDay, data.birthMonth, data.birthYear);
    newTeacher.setAddress(data.address);
    newTeacher.setCitizenId(data.citizenId);
    newTeacher.setEmail(data.email);
    newTeacher.setPhoneNumber(data.phoneNumber);
    newTeacher.setQualification(data.qualification);
    newTeacher.setSpecializationSubjects(data.specializationSubjects);
    newTeacher.setDesignation(data.designation);
    newTeacher.setExperienceYears(data.experienceYears);
    newTeacher.setRole(UserRole::TEACHER);

    auto addTeacherResult = _teacherDao->add(newTeacher); // Sẽ thêm vào Users và Teachers
    if (!addTeacherResult.has_value()) return std::unexpected(addTeacherResult.error());

    std::string salt = PasswordUtils::generateSalt();
    std::string hashedPassword = PasswordUtils::hashPassword(data.initialPassword, salt);
    auto addCredsResult = _loginDao->addUserCredentials(data.id, hashedPassword, salt, UserRole::TEACHER, LoginStatus::ACTIVE);
    if (!addCredsResult.has_value() || !addCredsResult.value()) {
        _teacherDao->remove(data.id); // Rollback
        return std::unexpected(addCredsResult.has_value() ? Error{ErrorCode::OPERATION_FAILED, "Failed to set credentials for teacher."} : addCredsResult.error());
    }
    
    // Tạo SalaryRecord ban đầu
    long initialSalary = data.experienceYears > 5 ? 10000000 : 7000000; // Ví dụ
    auto salaryRecordResult = _salaryDao->add(SalaryRecord(data.id, initialSalary));
    if (!salaryRecordResult.has_value()) {
        LOG_WARN("Teacher " + data.id + " added by admin, but failed to create initial salary record: " + salaryRecordResult.error().message);
    }


    LOG_INFO("Teacher " + data.id + " added by admin successfully.");
    return addTeacherResult.value();
}

/**
 * @brief Xóa tài khoản giảng viên
 * 
 * Phương thức này xóa tài khoản giảng viên và tất cả dữ liệu liên quan
 * (thông tin đăng nhập, bản ghi lương).
 * Yêu cầu quyền truy cập: chỉ admin mới có quyền xóa tài khoản giảng viên.
 * 
 * @param teacherId ID của giảng viên cần xóa
 * @return std::expected<bool, Error> true nếu xóa thành công, hoặc lỗi nếu thất bại
 */
std::expected<bool, Error> AdminService::removeTeacherAccount(const std::string& teacherId) {
    if (!isAdminAuthenticated()) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can remove teacher accounts."});
    }
    ValidationResult idVr = _inputValidator->validateIdFormat(teacherId, "Teacher ID");
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);

    // DAO Teacher::remove sẽ xóa User, và DB schema sẽ cascade xóa Teachers, Logins, SalaryRecords
    auto removeResult = _teacherDao->remove(teacherId);
     if (removeResult.has_value() && removeResult.value()) {
        LOG_INFO("Teacher account removed: " + teacherId);
    }
    return removeResult;
}


/**
 * @brief Đặt lại mật khẩu cho người dùng
 * 
 * Phương thức này cho phép admin đặt lại mật khẩu cho bất kỳ người dùng nào
 * (sinh viên, giảng viên, admin khác) khi họ quên mật khẩu hoặc cần hỗ trợ.
 * Mật khẩu mới sẽ được mã hóa với một salt mới.
 * Yêu cầu quyền truy cập: chỉ admin mới có quyền đặt lại mật khẩu người dùng.
 * 
 * @param userId ID của người dùng cần đặt lại mật khẩu
 * @param newPassword Mật khẩu mới cho người dùng
 * @return std::expected<bool, Error> true nếu đặt lại mật khẩu thành công, hoặc lỗi nếu thất bại
 */
std::expected<bool, Error> AdminService::resetUserPassword(const std::string& userId, const std::string& newPassword) {
    if (!isAdminAuthenticated()) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can reset user passwords."});
    }
    ValidationResult idVr = _inputValidator->validateIdFormat(userId, "User ID");
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);

    ValidationResult passVr = _inputValidator->validatePasswordComplexity(newPassword);
    if (!passVr.isValid) return std::unexpected(passVr.errors[0]);

    // Kiểm tra User tồn tại (bất kể role)
    auto userCreds = _loginDao->findCredentialsByUserId(userId); // Check bảng Logins trước
    if(!userCreds.has_value() && userCreds.error().code == ErrorCode::NOT_FOUND){
        // Nếu không có trong Logins, có thể user đó chưa từng có pass (hiếm) hoặc ID sai
        // Kiểm tra trong Users để chắc chắn user có tồn tại.
        auto roleRes = _loginDao->getUserRole(userId); // getUserRole truy vấn bảng Users
        if(!roleRes.has_value()){
            return std::unexpected(Error{ErrorCode::NOT_FOUND, "User ID " + userId + " not found."});
        }
    } else if (!userCreds.has_value()){
        return std::unexpected(userCreds.error()); // Lỗi khác khi tìm creds
    }


    std::string newSalt = PasswordUtils::generateSalt();
    std::string newHashedPassword = PasswordUtils::hashPassword(newPassword, newSalt);

    auto updateResult = _loginDao->updatePassword(userId, newHashedPassword, newSalt);
     if (!updateResult.has_value() || !updateResult.value()) {
        // Nếu updatePassword báo NOT_FOUND, có thể user đó chưa có record trong Logins
        // Trong trường hợp này, ta cần add credentials mới thay vì update.
        if(updateResult.error().code == ErrorCode::NOT_FOUND){
            // Lấy role và status từ Users table để gọi addUserCredentials
            auto roleExp = _loginDao->getUserRole(userId);
            if(!roleExp.has_value()) return std::unexpected(roleExp.error());
            auto statusExp = _loginDao->getUserStatus(userId);
            if(!statusExp.has_value()) return std::unexpected(statusExp.error());

            auto addCredsRes = _loginDao->addUserCredentials(userId, newHashedPassword, newSalt, roleExp.value(), statusExp.value());
            if(!addCredsRes.has_value() || !addCredsRes.value()){
                 return std::unexpected(addCredsRes.has_value() ? Error{ErrorCode::OPERATION_FAILED, "Failed to set new password for user."} : addCredsRes.error());
            }
        } else {
             return std::unexpected(updateResult.has_value() ? Error{ErrorCode::OPERATION_FAILED, "Failed to reset password."} : updateResult.error());
        }
    }
    LOG_INFO("Password reset successfully for user " + userId);
    return true;
}

/**
 * @brief Vô hiệu hóa tài khoản người dùng
 * 
 * Phương thức này đặt trạng thái của tài khoản người dùng thành DISABLED,
 * ngăn người dùng đăng nhập vào hệ thống. Tài khoản bị vô hiệu hóa có thể
 * được kích hoạt lại sau bằng phương thức enableUserAccount.
 * Yêu cầu quyền truy cập: chỉ admin mới có quyền vô hiệu hóa tài khoản người dùng.
 * 
 * @param userId ID của người dùng cần vô hiệu hóa
 * @return std::expected<bool, Error> true nếu vô hiệu hóa thành công, hoặc lỗi nếu thất bại
 */
std::expected<bool, Error> AdminService::disableUserAccount(const std::string& userId) {
    if (!isAdminAuthenticated()) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can disable user accounts."});
    }
    ValidationResult idVr = _inputValidator->validateIdFormat(userId, "User ID");
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);

    // Lấy role hiện tại để truyền vào updateUserRoleAndStatus
    auto roleExp = _loginDao->getUserRole(userId);
    if (!roleExp.has_value()) return std::unexpected(roleExp.error());

    auto updateResult = _loginDao->updateUserRoleAndStatus(userId, roleExp.value(), LoginStatus::DISABLED);
    if (updateResult.has_value() && updateResult.value()) {
        LOG_INFO("User account disabled: " + userId);
    }
    return updateResult;
}

/**
 * @brief Kích hoạt tài khoản người dùng
 * 
 * Phương thức này đặt trạng thái của tài khoản người dùng thành ACTIVE,
 * cho phép người dùng đăng nhập vào hệ thống. Nếu người dùng có vai trò
 * PENDING_STUDENT, phương thức này sẽ đồng thời nâng cấp vai trò thành STUDENT.
 * Yêu cầu quyền truy cập: chỉ admin mới có quyền kích hoạt tài khoản người dùng.
 * 
 * @param userId ID của người dùng cần kích hoạt
 * @return std::expected<bool, Error> true nếu kích hoạt thành công, hoặc lỗi nếu thất bại
 */
std::expected<bool, Error> AdminService::enableUserAccount(const std::string& userId) {
    if (!isAdminAuthenticated()) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can enable user accounts."});
    }
    ValidationResult idVr = _inputValidator->validateIdFormat(userId, "User ID");
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);

    auto roleExp = _loginDao->getUserRole(userId);
    if (!roleExp.has_value()) return std::unexpected(roleExp.error());
    
    // Nếu user là PENDING_STUDENT, khi enable thì chuyển thành STUDENT + ACTIVE
    UserRole targetRole = roleExp.value();
    if (targetRole == UserRole::PENDING_STUDENT) {
        targetRole = UserRole::STUDENT;
    }

    auto updateResult = _loginDao->updateUserRoleAndStatus(userId, targetRole, LoginStatus::ACTIVE);
     if (updateResult.has_value() && updateResult.value()) {
        LOG_INFO("User account enabled: " + userId);
    }
    return updateResult;
}