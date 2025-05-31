// --- START OF MODIFIED FILE tests/core/services/impl/AuthService_test.cpp ---
#include "gtest/gtest.h"
#include "gmock/gmock.h" 
#include "../../../../src/core/services/impl/AuthService.h"
#include "../../../../src/core/data_access/interface/ILoginDao.h" // Để dùng LoginCredentials
#include "../../../../src/core/data_access/interface/IStudentDao.h"
#include "../../../../src/core/data_access/interface/ITeacherDao.h"
#include "../../../../src/core/data_access/interface/IFacultyDao.h"
#include "../../../../src/core/validators/interface/IValidator.h"
#include "../../../../src/core/services/SessionContext.h" 
#include "../../../../src/core/entities/Student.h"
#include "../../../../src/core/entities/Teacher.h"
#include "../../../../src/core/entities/Faculty.h" // Thêm nếu FacultyDao được dùng trực tiếp
#include "../../../../src/common/ErrorType.h"
#include "../../../../src/utils/PasswordInput.h" 

using ::testing::Return;
using ::testing::_; 
using ::testing::NiceMock;
using ::testing::An; // Thêm An<>() cho any type

// --- Các lớp Mock bằng Google Mock ---
class MockLoginDaoGmock : public ILoginDao {
public:
    MOCK_METHOD((std::expected<LoginCredentials, Error>), findCredentialsByUserId, (const std::string& userId), (const, override));
    MOCK_METHOD((std::expected<bool, Error>), addUserCredentials, (const std::string& userId, const std::string& passwordHash, const std::string& salt, UserRole role, LoginStatus status), (override));
    MOCK_METHOD((std::expected<bool, Error>), updatePassword, (const std::string& userId, const std::string& newPasswordHash, const std::string& newSalt), (override));
    MOCK_METHOD((std::expected<bool, Error>), removeUserCredentials, (const std::string& userId), (override));
    MOCK_METHOD((std::expected<UserRole, Error>), getUserRole, (const std::string& userId), (const, override));
    MOCK_METHOD((std::expected<LoginStatus, Error>), getUserStatus, (const std::string& userId), (const, override));
    MOCK_METHOD((std::expected<bool, Error>), updateUserRoleAndStatus, (const std::string& userId, UserRole newRole, LoginStatus newStatus), (override));
    MOCK_METHOD((std::expected<std::vector<LoginCredentials>, Error>), findByStatus, (LoginStatus status), (const, override));
};

class MockStudentDaoGmock : public IStudentDao {
public:
    MOCK_METHOD((std::expected<Student, Error>), getById, (const std::string& id), (const, override));
    MOCK_METHOD((std::expected<std::vector<Student>, Error>), getAll, (), (const, override));
    MOCK_METHOD((std::expected<Student, Error>), add, (const Student& student), (override));
    MOCK_METHOD((std::expected<bool, Error>), update, (const Student& student), (override));
    MOCK_METHOD((std::expected<bool, Error>), remove, (const std::string& id), (override));
    MOCK_METHOD((std::expected<bool, Error>), exists, (const std::string& id), (const, override));
    MOCK_METHOD((std::expected<std::vector<Student>, Error>), findByFacultyId, (const std::string& facultyId), (const, override));
    MOCK_METHOD((std::expected<Student, Error>), findByEmail, (const std::string& email), (const, override));
    MOCK_METHOD((std::expected<std::vector<Student>, Error>), findByStatus, (LoginStatus status), (const, override));
    MOCK_METHOD((std::expected<bool, Error>), updateStatus, (const std::string& studentId, LoginStatus newStatus), (override));
};

class MockTeacherDaoGmock : public ITeacherDao {
public:
    MOCK_METHOD((std::expected<Teacher, Error>), getById, (const std::string& id), (const, override));
    MOCK_METHOD((std::expected<std::vector<Teacher>, Error>), getAll, (), (const, override));
    MOCK_METHOD((std::expected<Teacher, Error>), add, (const Teacher& teacher), (override));
    MOCK_METHOD((std::expected<bool, Error>), update, (const Teacher& teacher), (override));
    MOCK_METHOD((std::expected<bool, Error>), remove, (const std::string& id), (override));
    MOCK_METHOD((std::expected<bool, Error>), exists, (const std::string& id), (const, override));
    MOCK_METHOD((std::expected<std::vector<Teacher>, Error>), findByFacultyId, (const std::string& facultyId), (const, override));
    MOCK_METHOD((std::expected<std::vector<Teacher>, Error>), findByDesignation, (const std::string& designation), (const, override));
    MOCK_METHOD((std::expected<Teacher, Error>), findByEmail, (const std::string& email), (const, override));
};

class MockFacultyDaoGmock : public IFacultyDao {
public:
    MOCK_METHOD((std::expected<Faculty, Error>), getById, (const std::string& id), (const, override));
    MOCK_METHOD((std::expected<std::vector<Faculty>, Error>), getAll, (), (const, override));
    MOCK_METHOD((std::expected<Faculty, Error>), add, (const Faculty& faculty), (override));
    MOCK_METHOD((std::expected<bool, Error>), update, (const Faculty& faculty), (override));
    MOCK_METHOD((std::expected<bool, Error>), remove, (const std::string& id), (override));
    MOCK_METHOD((std::expected<bool, Error>), exists, (const std::string& id), (const, override));
    MOCK_METHOD((std::expected<Faculty, Error>), findByName, (const std::string& name), (const, override));
};


class MockGeneralInputValidatorGmock : public IGeneralInputValidator {
public:
    MOCK_METHOD(ValidationResult, validateEmail, (const std::string&), (const, override));
    MOCK_METHOD(ValidationResult, validatePhoneNumber, (const std::string&, const std::string&), (const, override));
    MOCK_METHOD(ValidationResult, validatePasswordComplexity, (const std::string&), (const, override));
    MOCK_METHOD(ValidationResult, validateIdFormat, (const std::string&, const std::string&, size_t, size_t), (const, override));
    MOCK_METHOD(ValidationResult, validateRequiredString, (const std::string&, const std::string&, size_t), (const, override));
    MOCK_METHOD(ValidationResult, validateOptionalString, (const std::string&, const std::string&, size_t), (const, override));
    MOCK_METHOD(ValidationResult, validateInteger, (long long, const std::string&, long long, long long), (const, override));
    MOCK_METHOD(ValidationResult, validateDate, (int, int, int, const std::string&), (const, override));
    MOCK_METHOD(ValidationResult, validateCitizenId, (const std::string&, const std::string&), (const, override));
};


class AuthServiceTest : public ::testing::Test {
protected:
    std::shared_ptr<NiceMock<MockLoginDaoGmock>> mockLoginDao;
    std::shared_ptr<NiceMock<MockStudentDaoGmock>> mockStudentDao;
    std::shared_ptr<NiceMock<MockFacultyDaoGmock>> mockFacultyDao; 
    std::shared_ptr<NiceMock<MockTeacherDaoGmock>> mockTeacherDao; 
    std::shared_ptr<NiceMock<MockGeneralInputValidatorGmock>> mockValidator;
    std::shared_ptr<SessionContext> sessionContext; 
    std::unique_ptr<AuthService> authService;

    void SetUp() override {
        mockLoginDao = std::make_shared<NiceMock<MockLoginDaoGmock>>();
        mockStudentDao = std::make_shared<NiceMock<MockStudentDaoGmock>>();
        mockFacultyDao = std::make_shared<NiceMock<MockFacultyDaoGmock>>(); 
        mockTeacherDao = std::make_shared<NiceMock<MockTeacherDaoGmock>>(); 
        mockValidator = std::make_shared<NiceMock<MockGeneralInputValidatorGmock>>();
        sessionContext = std::make_shared<SessionContext>();
        authService = std::make_unique<AuthService>(mockLoginDao, mockStudentDao, mockFacultyDao, mockTeacherDao, mockValidator, sessionContext);
    }
};

TEST_F(AuthServiceTest, Login_Successful_Student) {
    std::string userId = "student1";
    std::string password = "password123";
    std::string salt = PasswordUtils::generateSalt();
    std::string hashedPassword = PasswordUtils::hashPassword(password, salt);

    LoginCredentials creds = {userId, hashedPassword, salt, UserRole::STUDENT, LoginStatus::ACTIVE};
    Student studentDetails(userId, "Test", "Student", "CS", LoginStatus::ACTIVE);

    EXPECT_CALL(*mockLoginDao, findCredentialsByUserId(userId))
        .WillOnce(Return(creds));
    EXPECT_CALL(*mockStudentDao, getById(userId))
        .WillOnce(Return(studentDetails));
    
    auto result = authService->login(userId, password);

    ASSERT_TRUE(result.has_value());
    ASSERT_NE(result.value(), nullptr);
    EXPECT_EQ(result.value()->getId(), userId);
    EXPECT_EQ(result.value()->getRole(), UserRole::STUDENT);
    EXPECT_TRUE(authService->isAuthenticated());
    EXPECT_EQ(authService->getCurrentUserId().value(), userId);
}

TEST_F(AuthServiceTest, Login_Failed_WrongPassword) {
    std::string userId = "student1";
    std::string correctPassword = "password123";
    std::string wrongPassword = "wrongpassword";
    std::string salt = PasswordUtils::generateSalt();
    std::string hashedPassword = PasswordUtils::hashPassword(correctPassword, salt);

    LoginCredentials creds = {userId, hashedPassword, salt, UserRole::STUDENT, LoginStatus::ACTIVE};
    
    EXPECT_CALL(*mockLoginDao, findCredentialsByUserId(userId))
        .WillOnce(Return(creds));
    Student studentDetails(userId, "Test", "Student", "CS", LoginStatus::ACTIVE);
    EXPECT_CALL(*mockStudentDao, getById(userId))
        .WillOnce(Return(studentDetails));


    auto result = authService->login(userId, wrongPassword);

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::AUTHENTICATION_FAILED);
    EXPECT_FALSE(authService->isAuthenticated());
}

TEST_F(AuthServiceTest, Login_UserNotFound) {
    std::string userId = "unknownUser";
    EXPECT_CALL(*mockLoginDao, findCredentialsByUserId(userId))
        .WillOnce(Return(std::unexpected(Error{ErrorCode::NOT_FOUND, "User not found"})));
    EXPECT_CALL(*mockStudentDao, findByEmail(_))
        .WillRepeatedly(Return(std::unexpected(Error{ErrorCode::NOT_FOUND, "Student email not found"})));
    EXPECT_CALL(*mockTeacherDao, findByEmail(_))
        .WillRepeatedly(Return(std::unexpected(Error{ErrorCode::NOT_FOUND, "Teacher email not found"})));


    auto result = authService->login(userId, "anypassword");

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::AUTHENTICATION_FAILED); 
}

TEST_F(AuthServiceTest, Login_AccountDisabled) {
    std::string userId = "disabledUser";
    LoginCredentials creds = {userId, "hash", "salt", UserRole::STUDENT, LoginStatus::DISABLED};
    EXPECT_CALL(*mockLoginDao, findCredentialsByUserId(userId))
        .WillOnce(Return(creds));
    Student studentDetails(userId, "Disabled", "User", "CS", LoginStatus::DISABLED);
     EXPECT_CALL(*mockStudentDao, getById(userId))
        .WillOnce(Return(studentDetails));


    auto result = authService->login(userId, "anypassword");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::AUTHENTICATION_FAILED);
    EXPECT_NE(result.error().message.find("disabled"), std::string::npos);
}

TEST_F(AuthServiceTest, Logout) {
    std::string userId = "student1";
    std::string password = "password123";
    std::string salt = PasswordUtils::generateSalt();
    std::string hashedPassword = PasswordUtils::hashPassword(password, salt);
    LoginCredentials creds = {userId, hashedPassword, salt, UserRole::STUDENT, LoginStatus::ACTIVE};
    Student studentDetails(userId, "Test", "Student", "CS", LoginStatus::ACTIVE);
    EXPECT_CALL(*mockLoginDao, findCredentialsByUserId(userId)).WillOnce(Return(creds));
    EXPECT_CALL(*mockStudentDao, getById(userId)).WillOnce(Return(studentDetails));
    authService->login(userId, password);
    ASSERT_TRUE(authService->isAuthenticated());

    authService->logout();
    ASSERT_FALSE(authService->isAuthenticated());
    EXPECT_FALSE(authService->getCurrentUserId().has_value());
    EXPECT_FALSE(authService->getCurrentUserRole().has_value());
}

TEST_F(AuthServiceTest, RegisterStudent_Success) {
    StudentRegistrationData data;
    data.email = "newstudent@example.com";
    data.firstName = "New";
    data.lastName = "Student";
    data.facultyId = "IT"; 
    data.birthDay = 1; data.birthMonth = 1; data.birthYear = 2000;
    data.citizenId = "000000001";

    EXPECT_CALL(*mockValidator, validateEmail(_)).WillOnce(Return(ValidationResult{}));
    EXPECT_CALL(*mockValidator, validatePasswordComplexity(_)).WillOnce(Return(ValidationResult{}));
    EXPECT_CALL(*mockValidator, validateRequiredString(_,_,_)).WillRepeatedly(Return(ValidationResult{}));
    EXPECT_CALL(*mockValidator, validateDate(_,_,_,_)).WillOnce(Return(ValidationResult{}));
    EXPECT_CALL(*mockValidator, validateCitizenId(_,_)).WillOnce(Return(ValidationResult{}));
    
    EXPECT_CALL(*mockFacultyDao, exists(data.facultyId)).WillOnce(Return(true));
    EXPECT_CALL(*mockStudentDao, findByEmail(data.email)).WillOnce(Return(std::unexpected(Error{ErrorCode::NOT_FOUND, ""})));
    EXPECT_CALL(*mockTeacherDao, findByEmail(data.email)).WillOnce(Return(std::unexpected(Error{ErrorCode::NOT_FOUND, ""})));
    
    // Sử dụng An<const std::string&>() cho các tham số string nếu không muốn chỉ định giá trị cụ thể
    EXPECT_CALL(*mockLoginDao, findCredentialsByUserId(An<const std::string&>())).WillRepeatedly(Return(std::unexpected(Error{ErrorCode::NOT_FOUND, ""})));
    
    EXPECT_CALL(*mockStudentDao, add(An<const Student&>())).WillOnce(Return(Student("23120001", data.firstName, data.lastName, data.facultyId, LoginStatus::PENDING_APPROVAL))); 
    EXPECT_CALL(*mockLoginDao, addUserCredentials(An<const std::string&>(), An<const std::string&>(), An<const std::string&>(), UserRole::PENDING_STUDENT, LoginStatus::PENDING_APPROVAL)).WillOnce(Return(true));

    auto result = authService->registerStudent(data, "ValidPass123");
    ASSERT_TRUE(result.has_value()) << (result.has_value() ? "" : result.error().message);
    EXPECT_TRUE(result.value());
}
// --- END OF MODIFIED FILE tests/core/services/impl/AuthService_test.cpp ---