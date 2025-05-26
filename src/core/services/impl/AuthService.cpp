/**
 * @file AuthService.cpp
 * @brief Triển khai chi tiết của các phương thức trong lớp AuthService
 * 
 * File này chứa mã nguồn triển khai cho các chức năng xác thực người dùng
 * bao gồm đăng nhập, đăng xuất, đăng ký, và quản lý thông tin phiên làm việc.
 */
#include "AuthService.h"
#include "../../../utils/StringUtils.h" // (➕) Cần cho trim
#include "../../../utils/PasswordInput.h" // Add this include for PasswordUtils functions
#include <random>   // (➕) Cần cho tạo studentId
#include <algorithm> // For std::all_of
#include <stdexcept> // For std::invalid_argument
#include <expected> // Add at the top of the file
#include "../../entities/AdminUser.h" // For UserRole enum

/**
 * @brief Constructor khởi tạo dịch vụ xác thực với các dependency cần thiết
 * 
 * @param loginDao DAO truy vấn thông tin đăng nhập
 * @param studentDao DAO truy vấn thông tin sinh viên
 * @param teacherDao DAO truy vấn thông tin giảng viên
 * @param inputValidator Bộ xác thực đầu vào
 * @param sessionContext Context lưu trữ thông tin phiên
 * 
 * @throws std::invalid_argument Nếu bất kỳ tham số nào là nullptr
 */
AuthService::AuthService(std::shared_ptr<ILoginDao> loginDao,
                         std::shared_ptr<IStudentDao> studentDao,
                         std::shared_ptr<ITeacherDao> teacherDao,
                         std::shared_ptr<IGeneralInputValidator> inputValidator,
                         std::shared_ptr<SessionContext> sessionContext)
    : _loginDao(std::move(loginDao)),
      _studentDao(std::move(studentDao)),
      _teacherDao(std::move(teacherDao)), // (➕)
      _inputValidator(std::move(inputValidator)),
      _sessionContext(std::move(sessionContext)) {
    if (!_loginDao) throw std::invalid_argument("LoginDao cannot be null for AuthService.");
    if (!_studentDao) throw std::invalid_argument("StudentDao cannot be null for AuthService.");
    if (!_teacherDao) throw std::invalid_argument("TeacherDao cannot be null for AuthService."); // (➕)
    if (!_inputValidator) throw std::invalid_argument("InputValidator cannot be null for AuthService.");
    if (!_sessionContext) throw std::invalid_argument("SessionContext cannot be null for AuthService.");
}

/**
 * @brief Xử lý đăng nhập bằng ID hoặc email cùng với mật khẩu
 * 
 * Phương thức này thực hiện các bước:
 * 1. Kiểm tra thông tin đầu vào không rỗng
 * 2. Tìm kiếm thông tin đăng nhập bằng ID
 * 3. Nếu không tìm thấy bằng ID, thử tìm kiếm bằng email (cho cả sinh viên và giảng viên)
 * 4. Xác thực mật khẩu nếu tìm thấy thông tin đăng nhập
 * 5. Nếu thành công, lấy chi tiết người dùng và lưu thông tin phiên
 * 
 * @param userIdOrEmail ID người dùng hoặc địa chỉ email
 * @param password Mật khẩu cần xác thực
 * @return Đối tượng User nếu thành công hoặc Error nếu thất bại
 */
std::expected<std::shared_ptr<User>, Error> AuthService::login(const std::string& userIdOrEmail, const std::string& password) {
    if (userIdOrEmail.empty() || password.empty()) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "User ID/Email and password cannot be empty."});
    }

    std::expected<LoginCredentials, Error> credsResult = std::unexpected(Error{ErrorCode::NOT_FOUND, "User not found."}); // Khởi tạo với lỗi mặc định
    std::shared_ptr<User> userDetails = nullptr;

    // Thử tìm bằng User ID trước
    auto credsById = _loginDao->findCredentialsByUserId(userIdOrEmail);

    if (credsById.has_value()) {
        credsResult = credsById;
    } else if (credsById.error().code == ErrorCode::NOT_FOUND) { // Chỉ thử tìm bằng email nếu ID không thấy
        // Kiểm tra xem userIdOrEmail có phải là email không
        if (userIdOrEmail.find('@') != std::string::npos) {
            auto studentByEmail = _studentDao->findByEmail(userIdOrEmail);
            if (studentByEmail.has_value()) {
                credsResult = _loginDao->findCredentialsByUserId(studentByEmail.value().getId());
                if (credsResult.has_value()) userDetails = std::make_shared<Student>(studentByEmail.value());
            } else if (studentByEmail.error().code == ErrorCode::NOT_FOUND) {
                auto teacherByEmail = _teacherDao->findByEmail(userIdOrEmail);
                if (teacherByEmail.has_value()) {
                    credsResult = _loginDao->findCredentialsByUserId(teacherByEmail.value().getId());
                    if (credsResult.has_value()) userDetails = std::make_shared<Teacher>(teacherByEmail.value());
                } else if (teacherByEmail.error().code != ErrorCode::NOT_FOUND) {
                     return std::unexpected(teacherByEmail.error()); // Lỗi khi tìm teacher bằng email
                }
            } else {
                 return std::unexpected(studentByEmail.error()); // Lỗi khi tìm student bằng email
            }
        }
    } else { // Lỗi khác NOT_FOUND khi tìm bằng ID
        return std::unexpected(credsById.error());
    }
    
    // Nếu vẫn không tìm thấy credentials sau khi thử cả ID và Email
    if (!credsResult.has_value()) {
        LOG_WARN("Login attempt failed for: " + userIdOrEmail + ". Reason: " + credsResult.error().message);
        // Trả về lỗi chung để không tiết lộ user/email có tồn tại hay không
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "Invalid user ID/email or password."});
    }

    const LoginCredentials& creds = credsResult.value();

    // Nếu userDetails chưa được tạo (ví dụ creds tìm thấy bằng ID nhưng chưa lấy chi tiết)
    if (!userDetails) {
        if (creds.role == UserRole::STUDENT) {
            auto studentRes = _studentDao->getById(creds.userId);
            if (studentRes.has_value()) userDetails = std::make_shared<Student>(studentRes.value());
            else return std::unexpected(studentRes.error()); 
        } 
        else if (creds.role == UserRole::PENDING_STUDENT) {
            auto pendingStudentRes = _studentDao->getById(creds.userId);
            if (pendingStudentRes.has_value()) userDetails = std::make_shared<Student>(pendingStudentRes.value());
            else return std::unexpected(pendingStudentRes.error());
        } else if (creds.role == UserRole::TEACHER) {
            auto teacherRes = _teacherDao->getById(creds.userId);
            if (teacherRes.has_value()) userDetails = std::make_shared<Teacher>(teacherRes.value());
            else return std::unexpected(teacherRes.error());

        } else if (creds.role == UserRole::ADMIN) {
            // (SỬA Ở ĐÂY)
            // Dòng cũ: userDetails = std::make_shared<User>(creds.userId, "Admin", "User", UserRole::ADMIN, LoginStatus::ACTIVE);
            // Giả sử Admin có thể không có họ riêng, hoặc bạn sẽ lấy từ DB nếu có bảng Admin riêng
            std::string adminFirstName = "Admin"; // Hoặc lấy từ DB nếu có
            std::string adminLastName = "";    // Hoặc lấy từ DB
            // Nếu thông tin tên admin được lưu trong bảng Users khi tạo tài khoản admin:
            // Bạn cần một cách để lấy firstName, lastName của admin từ DB, có thể là một hàm getById chung trong UserDao.
            // Tạm thời, chúng ta có thể không cần lấy chi tiết tên từ DB cho AdminUser ở đây nếu nó không quan trọng
            // bằng việc có một đối tượng cụ thể.
            // Nếu `ILoginDao::findCredentialsByUserId` đã join và lấy được first/last name từ bảng Users:
            // firstName = SqlParserUtils::getOptional<std::string>(row_của_admin, "firstName");
            // lastName = SqlParserUtils::getOptional<std::string>(row_của_admin, "lastName");

            userDetails = std::make_shared<AdminUser>(creds.userId, adminFirstName, adminLastName, creds.status);
            // AdminUser sẽ tự set Role là ADMIN trong constructor của nó.
            // Có thể cần set thêm email nếu AdminUser có trường email riêng và bạn lấy được từ LoginCredentials
            if(userDetails){ // Kiểm tra userDetails được tạo thành công
                auto adminAsUser = std::static_pointer_cast<User>(userDetails); // Để gọi setter của User
                adminAsUser->setEmail(userIdOrEmail); // Giả sử admin login bằng username là userId hoặc email
            }
        
        } else {
            return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "Unsupported user role for login details."});
        }
    }
    // Kiểm tra lại userDetails sau khi fetch
     if (!userDetails) {
        LOG_ERROR("Login successful for credentials of " + creds.userId + " but failed to retrieve full user details.");
        return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Login succeeded but failed to retrieve user profile."});
    }


    if (creds.status == LoginStatus::DISABLED) {
        LOG_WARN("Login attempt failed for: " + userIdOrEmail + ". Reason: Account disabled.");
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "Account is disabled."});
    }
    if (creds.status == LoginStatus::PENDING_APPROVAL) {
         LOG_WARN("Login attempt failed for: " + userIdOrEmail + ". Reason: Account pending approval.");
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "Account is pending approval."});
    }

    if (!PasswordUtils::verifyPassword(password, creds.passwordHash, creds.salt)) {
        LOG_WARN("Login attempt failed for: " + userIdOrEmail + ". Reason: Invalid password.");
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "Invalid user ID/email or password."});
    }
        
    _sessionContext->setCurrentUser(userDetails);
    LOG_INFO("User " + creds.userId + " logged in successfully.");
    return userDetails;
}

void AuthService::logout() {
    if (_sessionContext->isAuthenticated()) {
        LOG_INFO("User " + _sessionContext->getCurrentUserId().value_or("Unknown") + " logged out.");
        _sessionContext->clearCurrentUser();
    }
}

bool AuthService::isAuthenticated() const {
    return _sessionContext->isAuthenticated();
}

std::optional<std::string> AuthService::getCurrentUserId() const {
    return _sessionContext->getCurrentUserId();
}

std::optional<UserRole> AuthService::getCurrentUserRole() const {
    return _sessionContext->getCurrentUserRole();
}

std::optional<std::shared_ptr<User>> AuthService::getCurrentUser() const {
    return _sessionContext->getCurrentUser();
}

std::expected<bool, Error> AuthService::registerStudent(const StudentRegistrationData& data, const std::string& plainPassword) {
    ValidationResult vrTotal;
    auto appendErrors = [&](const ValidationResult& vr) {
        for(const auto& err : vr.errors) vrTotal.addError(err);
    };

    appendErrors(_inputValidator->validateEmail(data.email));
    appendErrors(_inputValidator->validatePasswordComplexity(plainPassword));
    appendErrors(_inputValidator->validateRequiredString(data.firstName, "First Name", 50));
    appendErrors(_inputValidator->validateRequiredString(data.lastName, "Last Name", 50));
    appendErrors(_inputValidator->validateRequiredString(data.facultyId, "Faculty ID", 10));
    appendErrors(_inputValidator->validateDate(data.birthDay, data.birthMonth, data.birthYear, "Birthday"));
    
    if (StringUtils::trim(data.citizenId).empty()){ // citizenId bắt buộc
        vrTotal.addError(ErrorCode::VALIDATION_ERROR, "Citizen ID is required.");
    } else {
        appendErrors(_inputValidator->validateCitizenId(data.citizenId));
    }
    if (!data.phoneNumber.empty()){ // phone không bắt buộc, nhưng nếu có thì validate
        appendErrors(_inputValidator->validatePhoneNumber(data.phoneNumber));
    }
    if (!data.address.empty()){ // address không bắt buộc
        appendErrors(_inputValidator->validateOptionalString(data.address, "Address", 200));
    }

    if(!vrTotal.isValid) return std::unexpected(vrTotal.errors[0]);

    // Kiểm tra email đã tồn tại trong Student hoặc Teacher
    auto studentByEmail = _studentDao->findByEmail(data.email);
    if (studentByEmail.has_value()) {
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Email '" + data.email + "' already registered by a student."});
    }
    if (studentByEmail.error().code != ErrorCode::NOT_FOUND) {
        return std::unexpected(studentByEmail.error());
    }
    auto teacherByEmail = _teacherDao->findByEmail(data.email);
     if (teacherByEmail.has_value()) {
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Email '" + data.email + "' already registered by a teacher."});
    }
    if (teacherByEmail.error().code != ErrorCode::NOT_FOUND) {
        return std::unexpected(teacherByEmail.error());
    }
    
    // Logic tạo Student ID (cần cải thiện cho production)
    std::string studentId;
    std::string cIdSuffix = data.citizenId.length() > 3 ? data.citizenId.substr(data.citizenId.length() - 3) : data.citizenId;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(100, 999); // Tăng phạm vi để giảm trùng
    int retryCount = 0;
    bool idOk = false;
    do {
        studentId = "S" + cIdSuffix + std::to_string(distrib(gen));
        auto studentExists = _studentDao->exists(studentId); 
        if (!studentExists.has_value()) return std::unexpected(studentExists.error());
        if (!studentExists.value()) {
            idOk = true;
            break; 
        }
        retryCount++;
    } while (retryCount < 20); // Tăng số lần thử

    if (!idOk) {
         return std::unexpected(Error{ErrorCode::OPERATION_FAILED, "Could not generate unique student ID after multiple retries."});
    }
    
    Student newStudent(studentId, data.firstName, data.lastName, data.facultyId, LoginStatus::PENDING_APPROVAL);
    newStudent.setBirthday(data.birthDay, data.birthMonth, data.birthYear);
    newStudent.setAddress(data.address);
    newStudent.setCitizenId(data.citizenId);
    newStudent.setEmail(data.email);
    newStudent.setPhoneNumber(data.phoneNumber);
    newStudent.setRole(UserRole::PENDING_STUDENT);

    // Thêm Student (bao gồm cả User record)
    // SqlStudentDao::add nên xử lý transaction nội bộ cho việc thêm vào Users và Students.
    auto addStudentResult = _studentDao->add(newStudent);
    if (!addStudentResult.has_value()) {
        LOG_ERROR("Student registration failed during studentDao->add: " + addStudentResult.error().message);
        return std::unexpected(addStudentResult.error());
    }

    std::string salt = PasswordUtils::generateSalt();
    std::string hashedPassword = PasswordUtils::hashPassword(plainPassword, salt);
    
    // Thêm LoginCredentials. addUserCredentials chỉ thêm vào bảng Logins.
    auto addCredsResult = _loginDao->addUserCredentials(studentId, hashedPassword, salt, UserRole::PENDING_STUDENT, LoginStatus::PENDING_APPROVAL);
    if (!addCredsResult.has_value() || !addCredsResult.value()) {
        LOG_ERROR("Student registration failed at addUserCredentials for " + studentId + ": " + (addCredsResult.has_value() ? "Op failed" : addCredsResult.error().message));
        // Nếu thất bại, cần rollback việc thêm student.
        // DAO::remove nên xử lý transaction để xóa cả Users và Students.
        auto removeRollback = _studentDao->remove(studentId); 
        if (!removeRollback.has_value()) {
            LOG_CRITICAL("Failed to rollback student creation for " + studentId + " after credential add failure: " + removeRollback.error().message);
        }
        return std::unexpected(addCredsResult.has_value() ? Error{ErrorCode::OPERATION_FAILED, "Failed to set login credentials."} : addCredsResult.error());
    }

    LOG_INFO("Student " + studentId + " registered successfully. Status: PENDING_APPROVAL.");
    return true;
}


std::expected<bool, Error> AuthService::changePassword(const std::string& currentUserId, const std::string& oldPassword, const std::string& newPassword) {
    if (!isAuthenticated()) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    auto currentIdOpt = getCurrentUserId();
    if (!currentIdOpt.has_value() || currentIdOpt.value() != currentUserId) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Permission denied to change password for this user."});
    }

    auto credsResult = _loginDao->findCredentialsByUserId(currentUserId);
    if (!credsResult.has_value()) {
        return std::unexpected(Error{ErrorCode::NOT_FOUND, "User credentials not found."});
    }

    const LoginCredentials& creds = credsResult.value();
    if (!PasswordUtils::verifyPassword(oldPassword, creds.passwordHash, creds.salt)) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "Incorrect old password."});
    }

    ValidationResult newPassVr = _inputValidator->validatePasswordComplexity(newPassword);
    if (!newPassVr.isValid) {
        return std::unexpected(newPassVr.errors[0]);
    }
     if (newPassword == oldPassword) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "New password cannot be the same as the old password."});
    }

    std::string newSalt = PasswordUtils::generateSalt();
    std::string newHashedPassword = PasswordUtils::hashPassword(newPassword, newSalt);

    auto updateResult = _loginDao->updatePassword(currentUserId, newHashedPassword, newSalt);
    if (!updateResult.has_value() || !updateResult.value()) {
        LOG_ERROR("Password change failed for user " + currentUserId + ": " + (updateResult.has_value() ? "Update failed" : updateResult.error().message));
        return std::unexpected(updateResult.has_value() ? Error{ErrorCode::OPERATION_FAILED, "Failed to update password."} : updateResult.error());
    }

    LOG_INFO("Password changed successfully for user " + currentUserId);
    return true;
}