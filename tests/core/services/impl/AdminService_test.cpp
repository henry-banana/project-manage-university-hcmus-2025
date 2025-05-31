// // --- START OF MODIFIED FILE tests/core/services/impl/AdminService_test.cpp ---
// #include "gtest/gtest.h"
// #include "gmock/gmock.h"
// #include "../../../../src/core/services/impl/AdminService.h"
// #include "../../../../src/core/data_access/mock/MockStudentDao.h" 
// #include "../../../../src/core/data_access/mock/MockTeacherDao.h" 
// #include "../../../../src/core/data_access/mock/MockLoginDao.h"   
// #include "../../../../src/core/data_access/mock/MockFeeRecordDao.h" 
// #include "../../../../src/core/data_access/mock/MockSalaryRecordDao.h" 
// #include "../../../../src/core/data_access/mock/MockFacultyDao.h" 
// #include "../../../../src/core/data_access/mock/MockEnrollmentDao.h" 
// #include "../../../../src/core/data_access/mock/MockCourseResultDao.h" 
// #include "../../../../src/core/validators/impl/GeneralInputValidator.h" 
// #include "../../../../src/core/validators/interface/IValidator.h" 
// #include "../../../../src/core/services/SessionContext.h"
// #include "../../../../src/core/entities/Student.h"
// #include "../../../../src/core/entities/Teacher.h"
// #include "../../../../src/core/entities/FeeRecord.h"
// #include "../../../../src/core/entities/SalaryRecord.h"
// #include "../../../../src/common/ErrorType.h" 
// #include "../../../../src/common/ValidationResult.h" 

// #include <memory>
// #include <expected>
// #include <vector>
// #include <string>

// using ::testing::Return;
// using ::testing::_;
// using ::testing::NiceMock; 
// using ::testing::An;       


// // --- Các lớp Mock bằng Google Mock (GIỮ NGUYÊN NHƯ TRƯỚC) ---
// class MockLoginDaoGmockAdmin : public ILoginDao { 
// public:
//     MOCK_METHOD((std::expected<LoginCredentials, Error>), findCredentialsByUserId, (const std::string& userId), (const, override));
//     MOCK_METHOD((std::expected<bool, Error>), addUserCredentials, (const std::string& userId, const std::string& passwordHash, const std::string& salt, UserRole role, LoginStatus status), (override));
//     MOCK_METHOD((std::expected<bool, Error>), updatePassword, (const std::string& userId, const std::string& newPasswordHash, const std::string& newSalt), (override));
//     MOCK_METHOD((std::expected<bool, Error>), removeUserCredentials, (const std::string& userId), (override));
//     MOCK_METHOD((std::expected<UserRole, Error>), getUserRole, (const std::string& userId), (const, override));
//     MOCK_METHOD((std::expected<LoginStatus, Error>), getUserStatus, (const std::string& userId), (const, override));
//     MOCK_METHOD((std::expected<bool, Error>), updateUserRoleAndStatus, (const std::string& userId, UserRole newRole, LoginStatus newStatus), (override));
//     MOCK_METHOD((std::expected<std::vector<LoginCredentials>, Error>), findByStatus, (LoginStatus status), (const, override));
// };

// class MockStudentDaoGmockAdmin : public IStudentDao {
// public:
//     MOCK_METHOD((std::expected<Student, Error>), getById, (const std::string& id), (const, override));
//     MOCK_METHOD((std::expected<std::vector<Student>, Error>), getAll, (), (const, override));
//     MOCK_METHOD((std::expected<Student, Error>), add, (const Student& student), (override));
//     MOCK_METHOD((std::expected<bool, Error>), update, (const Student& student), (override));
//     MOCK_METHOD((std::expected<bool, Error>), remove, (const std::string& id), (override));
//     MOCK_METHOD((std::expected<bool, Error>), exists, (const std::string& id), (const, override));
//     MOCK_METHOD((std::expected<std::vector<Student>, Error>), findByFacultyId, (const std::string& facultyId), (const, override));
//     MOCK_METHOD((std::expected<Student, Error>), findByEmail, (const std::string& email), (const, override));
//     MOCK_METHOD((std::expected<std::vector<Student>, Error>), findByStatus, (LoginStatus status), (const, override));
//     MOCK_METHOD((std::expected<bool, Error>), updateStatus, (const std::string& studentId, LoginStatus newStatus), (override));
// };

// class MockTeacherDaoGmockAdmin : public ITeacherDao {
// public:
//     MOCK_METHOD((std::expected<Teacher, Error>), getById, (const std::string& id), (const, override));
//     MOCK_METHOD((std::expected<std::vector<Teacher>, Error>), getAll, (), (const, override));
//     MOCK_METHOD((std::expected<Teacher, Error>), add, (const Teacher& teacher), (override));
//     MOCK_METHOD((std::expected<bool, Error>), update, (const Teacher& teacher), (override));
//     MOCK_METHOD((std::expected<bool, Error>), remove, (const std::string& id), (override));
//     MOCK_METHOD((std::expected<bool, Error>), exists, (const std::string& id), (const, override));
//     MOCK_METHOD((std::expected<std::vector<Teacher>, Error>), findByFacultyId, (const std::string& facultyId), (const, override));
//     MOCK_METHOD((std::expected<std::vector<Teacher>, Error>), findByDesignation, (const std::string& designation), (const, override));
//     MOCK_METHOD((std::expected<Teacher, Error>), findByEmail, (const std::string& email), (const, override));
// };

// class MockFacultyDaoGmockAdmin : public IFacultyDao { // (➕) Đổi tên từ MyMockFalcutyDao cho nhất quán
// public:
//     MOCK_METHOD((std::expected<Faculty, Error>), getById, (const std::string& id), (const, override));
//     MOCK_METHOD((std::expected<std::vector<Faculty>, Error>), getAll, (), (const, override));
//     MOCK_METHOD((std::expected<Faculty, Error>), add, (const Faculty& faculty), (override));
//     MOCK_METHOD((std::expected<bool, Error>), update, (const Faculty& faculty), (override));
//     MOCK_METHOD((std::expected<bool, Error>), remove, (const std::string& id), (override));
//     MOCK_METHOD((std::expected<bool, Error>), exists, (const std::string& id), (const, override));
//     MOCK_METHOD((std::expected<Faculty, Error>), findByName, (const std::string& name), (const, override));
// };

// class MockFeeRecordDaoGmockAdmin : public IFeeRecordDao {
// public:
//     MOCK_METHOD((std::expected<FeeRecord, Error>), getById, (const std::string& id), (const, override));
//     MOCK_METHOD((std::expected<std::vector<FeeRecord>, Error>), getAll, (), (const, override));
//     MOCK_METHOD((std::expected<FeeRecord, Error>), add, (const FeeRecord& entity), (override));
//     MOCK_METHOD((std::expected<bool, Error>), update, (const FeeRecord& entity), (override));
//     MOCK_METHOD((std::expected<bool, Error>), remove, (const std::string& id), (override));
//     MOCK_METHOD((std::expected<bool, Error>), exists, (const std::string& id), (const, override));
// };
// class MockSalaryRecordDaoGmockAdmin : public ISalaryRecordDao {
// public:
//     MOCK_METHOD((std::expected<SalaryRecord, Error>), getById, (const std::string& id), (const, override));
//     MOCK_METHOD((std::expected<std::vector<SalaryRecord>, Error>), getAll, (), (const, override));
//     MOCK_METHOD((std::expected<SalaryRecord, Error>), add, (const SalaryRecord& entity), (override));
//     MOCK_METHOD((std::expected<bool, Error>), update, (const SalaryRecord& entity), (override));
//     MOCK_METHOD((std::expected<bool, Error>), remove, (const std::string& id), (override));
//     MOCK_METHOD((std::expected<bool, Error>), exists, (const std::string& id), (const, override));
// };
// class MockEnrollmentDaoGmockAdmin : public IEnrollmentDao {
// public:
//     MOCK_METHOD((std::expected<bool, Error>), addEnrollment, (const std::string&, const std::string&), (override));
//     MOCK_METHOD((std::expected<bool, Error>), removeEnrollment, (const std::string&, const std::string&), (override));
//     MOCK_METHOD((std::expected<bool, Error>), removeEnrollmentsByStudent, (const std::string&), (override));
//     MOCK_METHOD((std::expected<bool, Error>), removeEnrollmentsByCourse, (const std::string&), (override));
//     MOCK_METHOD((std::expected<bool, Error>), isEnrolled, (const std::string&, const std::string&), (const, override));
//     MOCK_METHOD((std::expected<std::vector<std::string>, Error>), findCourseIdsByStudentId, (const std::string&), (const, override));
//     MOCK_METHOD((std::expected<std::vector<std::string>, Error>), findStudentIdsByCourseId, (const std::string&), (const, override));
//     MOCK_METHOD((std::expected<std::vector<EnrollmentRecord>, Error>), getAllEnrollments, (), (const, override));
// };
// class MockCourseResultDaoGmockAdmin : public ICourseResultDao {
// public:
//     MOCK_METHOD((std::expected<CourseResult, Error>), find, (const std::string&, const std::string&), (const, override));
//     MOCK_METHOD((std::expected<std::vector<CourseResult>, Error>), findByStudentId, (const std::string&), (const, override));
//     MOCK_METHOD((std::expected<std::vector<CourseResult>, Error>), findByCourseId, (const std::string&), (const, override));
//     MOCK_METHOD((std::expected<bool, Error>), addOrUpdate, (const CourseResult&), (override));
//     MOCK_METHOD((std::expected<bool, Error>), remove, (const std::string&, const std::string&), (override));
//     MOCK_METHOD((std::expected<bool, Error>), removeAllForStudent, (const std::string&), (override));
//     MOCK_METHOD((std::expected<bool, Error>), removeAllForCourse, (const std::string&), (override));
// };

// class MockGeneralInputValidatorGmockAdmin : public IGeneralInputValidator { 
// public:
//     MOCK_METHOD(ValidationResult, validateEmail, (const std::string&), (const, override));
//     MOCK_METHOD(ValidationResult, validatePhoneNumber, (const std::string&, const std::string&), (const, override));
//     MOCK_METHOD(ValidationResult, validatePasswordComplexity, (const std::string&), (const, override));
//     MOCK_METHOD(ValidationResult, validateIdFormat, (const std::string&, const std::string&, size_t, size_t), (const, override));
//     MOCK_METHOD(ValidationResult, validateRequiredString, (const std::string&, const std::string&, size_t), (const, override));
//     MOCK_METHOD(ValidationResult, validateOptionalString, (const std::string&, const std::string&, size_t), (const, override));
//     MOCK_METHOD(ValidationResult, validateInteger, (long long, const std::string&, long long, long long), (const, override));
//     MOCK_METHOD(ValidationResult, validateDate, (int, int, int, const std::string&), (const, override));
//     MOCK_METHOD(ValidationResult, validateCitizenId, (const std::string&, const std::string&), (const, override));
// };

// class MockSessionContextGmockAdmin : public SessionContext { 
// public:
//     MOCK_CONST_METHOD0(isAuthenticated, bool()); 
//     MOCK_CONST_METHOD0(getCurrentUserId, std::optional<std::string>());
//     MOCK_CONST_METHOD0(getCurrentUserRole, std::optional<UserRole>());
//     MOCK_CONST_METHOD0(getCurrentUser, std::optional<std::shared_ptr<User>>());
// };
// // --- Hết phần Mock Objects ---


// // Test fixture
// class AdminServiceTest : public ::testing::Test {
// protected:
//     std::shared_ptr<NiceMock<MockStudentDaoGmockAdmin>> mockStudentDao;
//     std::shared_ptr<NiceMock<MockTeacherDaoGmockAdmin>> mockTeacherDao;
//     std::shared_ptr<NiceMock<MockFacultyDaoGmockAdmin>> mockFacultyDao; 
//     std::shared_ptr<NiceMock<MockLoginDaoGmockAdmin>> mockLoginDao;
//     std::shared_ptr<NiceMock<MockFeeRecordDaoGmockAdmin>> mockFeeDao;
//     std::shared_ptr<NiceMock<MockSalaryRecordDaoGmockAdmin>> mockSalaryDao;
//     std::shared_ptr<NiceMock<MockEnrollmentDaoGmockAdmin>> mockEnrollmentDao;
//     std::shared_ptr<NiceMock<MockCourseResultDaoGmockAdmin>> mockCourseResultDao;
//     std::shared_ptr<NiceMock<MockGeneralInputValidatorGmockAdmin>> mockValidator;
//     std::shared_ptr<NiceMock<MockSessionContextGmockAdmin>> mockSession; 
    
//     std::unique_ptr<AdminService> service; 

//     void SetUp() override {
//         mockStudentDao = std::make_shared<NiceMock<MockStudentDaoGmockAdmin>>();
//         mockTeacherDao = std::make_shared<NiceMock<MockTeacherDaoGmockAdmin>>();
//         mockFacultyDao = std::make_shared<NiceMock<MockFacultyDaoGmockAdmin>>(); 
//         mockLoginDao = std::make_shared<NiceMock<MockLoginDaoGmockAdmin>>();
//         mockFeeDao = std::make_shared<NiceMock<MockFeeRecordDaoGmockAdmin>>();
//         mockSalaryDao = std::make_shared<NiceMock<MockSalaryRecordDaoGmockAdmin>>();
//         mockEnrollmentDao = std::make_shared<NiceMock<MockEnrollmentDaoGmockAdmin>>();
//         mockCourseResultDao = std::make_shared<NiceMock<MockCourseResultDaoGmockAdmin>>();
//         mockValidator = std::make_shared<NiceMock<MockGeneralInputValidatorGmockAdmin>>();
//         mockSession = std::make_shared<NiceMock<MockSessionContextGmockAdmin>>();
        
//         // (QUAN TRỌNG) Giả sử constructor của AdminService ĐÃ được cập nhật để nhận IFacultyDao
//         service = std::make_unique<AdminService>(
//             mockStudentDao, mockTeacherDao, mockFacultyDao, mockLoginDao, mockFeeDao, 
//             mockSalaryDao, mockEnrollmentDao, mockCourseResultDao,
//             mockValidator, mockSession
//         );
//     }
// };

// // (Các test cases giữ nguyên như Hòa đã có hoặc Măng đã sửa ở trên)
// // approveStudentRegistration
// TEST_F(AdminServiceTest, ApproveStudentRegistration_Success) {
//     EXPECT_CALL(*mockSession, isAuthenticated()).WillRepeatedly(Return(true));
//     EXPECT_CALL(*mockSession, getCurrentUserRole()).WillRepeatedly(Return(UserRole::ADMIN));
//     EXPECT_CALL(*mockValidator, validateIdFormat("S1", "Student ID", _, _)).WillOnce(Return(ValidationResult{true, {}}));

//     Student pendingStudent("S1", "Test", "User", "IT", LoginStatus::PENDING_APPROVAL);
//     pendingStudent.setRole(UserRole::PENDING_STUDENT);
//     EXPECT_CALL(*mockStudentDao, getById("S1")).WillOnce(Return(pendingStudent));
//     EXPECT_CALL(*mockLoginDao, updateUserRoleAndStatus("S1", UserRole::STUDENT, LoginStatus::ACTIVE)).WillOnce(Return(true));
//     EXPECT_CALL(*mockFeeDao, add(An<const FeeRecord&>())).WillOnce(Return(FeeRecord("S1", 5000000, 0))); 

//     auto result = service->approveStudentRegistration("S1");
//     ASSERT_TRUE(result.has_value()) << (result.has_value() ? "" : result.error().message);
//     EXPECT_TRUE(result.value());
// }

// TEST_F(AdminServiceTest, ApproveStudentRegistration_NotPending) {
//     EXPECT_CALL(*mockSession, isAuthenticated()).WillRepeatedly(Return(true));
//     EXPECT_CALL(*mockSession, getCurrentUserRole()).WillRepeatedly(Return(UserRole::ADMIN));
//     EXPECT_CALL(*mockValidator, validateIdFormat("S1", "Student ID", _, _)).WillOnce(Return(ValidationResult{true, {}}));

//     Student activeStudent("S1", "Test", "User", "IT", LoginStatus::ACTIVE); 
//     activeStudent.setRole(UserRole::STUDENT);
//     EXPECT_CALL(*mockStudentDao, getById("S1")).WillOnce(Return(activeStudent));

//     auto result = service->approveStudentRegistration("S1");
//     ASSERT_FALSE(result.has_value());
//     EXPECT_EQ(result.error().code, ErrorCode::VALIDATION_ERROR);
//     EXPECT_NE(result.error().message.find("not pending approval"), std::string::npos);
// }


// TEST_F(AdminServiceTest, ApproveStudentRegistration_NotFound) {
//     EXPECT_CALL(*mockSession, isAuthenticated()).WillRepeatedly(Return(true));
//     EXPECT_CALL(*mockSession, getCurrentUserRole()).WillRepeatedly(Return(UserRole::ADMIN));
//     EXPECT_CALL(*mockValidator, validateIdFormat("S999", "Student ID", _, _)).WillOnce(Return(ValidationResult{true, {}}));
//     EXPECT_CALL(*mockStudentDao, getById("S999")).WillOnce(Return(std::unexpected(Error{ErrorCode::NOT_FOUND, "not found"})));
    
//     auto result = service->approveStudentRegistration("S999");
//     ASSERT_FALSE(result.has_value());
//     ASSERT_EQ(result.error().code, ErrorCode::NOT_FOUND);
// }

// // getStudentsByStatus
// TEST_F(AdminServiceTest, GetStudentsByStatus_Success) {
//     EXPECT_CALL(*mockSession, isAuthenticated()).WillRepeatedly(Return(true));
//     EXPECT_CALL(*mockSession, getCurrentUserRole()).WillRepeatedly(Return(UserRole::ADMIN));
    
//     std::vector<Student> students = {
//         Student("S1", "A", "Nguyen", "F001", LoginStatus::ACTIVE),
//         Student("S2", "B", "Tran", "F002", LoginStatus::ACTIVE)
//     };
//     EXPECT_CALL(*mockStudentDao, findByStatus(LoginStatus::ACTIVE)).WillOnce(Return(students));
    
//     auto result = service->getStudentsByStatus(LoginStatus::ACTIVE);
//     ASSERT_TRUE(result.has_value());
//     ASSERT_EQ(result->size(), 2);
// }


// TEST_F(AdminServiceTest, ResetUserPassword_Success) {
//     EXPECT_CALL(*mockSession, isAuthenticated()).WillRepeatedly(Return(true));
//     EXPECT_CALL(*mockSession, getCurrentUserRole()).WillRepeatedly(Return(UserRole::ADMIN));
//     EXPECT_CALL(*mockValidator, validateIdFormat("U1", "User ID", _, _)).WillOnce(Return(ValidationResult{true, {}}));
//     EXPECT_CALL(*mockValidator, validatePasswordComplexity("newpass")).WillOnce(Return(ValidationResult{true, {}}));
//     EXPECT_CALL(*mockLoginDao, findCredentialsByUserId("U1")).WillOnce(Return(LoginCredentials{"U1", "oldhash", "oldsalt", UserRole::STUDENT, LoginStatus::ACTIVE})); 
//     EXPECT_CALL(*mockLoginDao, updatePassword("U1", An<const std::string&>(), An<const std::string&>())).WillOnce(Return(true));

//     auto result = service->resetUserPassword("U1", "newpass");
//     ASSERT_TRUE(result.has_value()) << (result.has_value() ? "" : result.error().message);
//     ASSERT_TRUE(result.value());
// }

// TEST_F(AdminServiceTest, DisableUserAccount_Success) {
//     EXPECT_CALL(*mockSession, isAuthenticated())
//         .WillRepeatedly(Return(true)); 
//     EXPECT_CALL(*mockSession, getCurrentUserRole())
//         .WillRepeatedly(Return(UserRole::ADMIN));
        
//     EXPECT_CALL(*mockValidator, validateIdFormat("U1", "User ID", _, _))
//         .WillOnce(Return(ValidationResult{true, {}}));

//     EXPECT_CALL(*mockLoginDao, getUserRole("U1")).WillOnce(Return(UserRole::STUDENT));
//     EXPECT_CALL(*mockLoginDao, updateUserRoleAndStatus("U1", UserRole::STUDENT, LoginStatus::DISABLED)).WillOnce(Return(true));
            
//     auto result = service->disableUserAccount("U1");
//     ASSERT_TRUE(result.has_value()) << (result.has_value() ? "" : result.error().message); 
//     EXPECT_TRUE(result.value());
// }

// TEST_F(AdminServiceTest, EnableUserAccount_Success) {
//     EXPECT_CALL(*mockSession, isAuthenticated()).WillRepeatedly(Return(true));
//     EXPECT_CALL(*mockSession, getCurrentUserRole()).WillRepeatedly(Return(UserRole::ADMIN));
//     EXPECT_CALL(*mockValidator, validateIdFormat("U1", "User ID", _, _)).WillOnce(Return(ValidationResult{true, {}}));

//     EXPECT_CALL(*mockLoginDao, getUserRole("U1")).WillOnce(Return(UserRole::STUDENT)); 
//     EXPECT_CALL(*mockLoginDao, updateUserRoleAndStatus("U1", UserRole::STUDENT, LoginStatus::ACTIVE)).WillOnce(Return(true));

//     auto result = service->enableUserAccount("U1");
//     ASSERT_TRUE(result.has_value()) << (result.has_value() ? "" : result.error().message);
//     ASSERT_TRUE(result.value());
// }

// TEST_F(AdminServiceTest, EnableUserAccount_PendingStudent_BecomesStudent) {
//     EXPECT_CALL(*mockSession, isAuthenticated()).WillRepeatedly(Return(true));
//     EXPECT_CALL(*mockSession, getCurrentUserRole()).WillRepeatedly(Return(UserRole::ADMIN));
//     EXPECT_CALL(*mockValidator, validateIdFormat("P01", "User ID", _, _)).WillOnce(Return(ValidationResult{true, {}}));

//     EXPECT_CALL(*mockLoginDao, getUserRole("P01")).WillOnce(Return(UserRole::PENDING_STUDENT));
//     EXPECT_CALL(*mockLoginDao, updateUserRoleAndStatus("P01", UserRole::STUDENT, LoginStatus::ACTIVE)).WillOnce(Return(true));

//     auto result = service->enableUserAccount("P01");
//     ASSERT_TRUE(result.has_value()) << (result.has_value() ? "" : result.error().message);
//     ASSERT_TRUE(result.value());
// }


// TEST_F(AdminServiceTest, IsAdminAuthenticated_True) {
//     EXPECT_CALL(*mockSession, isAuthenticated()).WillOnce(Return(true));
//     EXPECT_CALL(*mockSession, getCurrentUserRole())
//         .WillOnce(Return(UserRole::ADMIN)); // (✅) WillOnce là đủ nếu service đã sửa
//     bool result = service->isAdminAuthenticated();
//     ASSERT_TRUE(result);
// }

// TEST_F(AdminServiceTest, IsAdminAuthenticated_False_NotAuthenticated) {
//     EXPECT_CALL(*mockSession, isAuthenticated()).WillOnce(Return(false));
//     // getCurrentUserRole sẽ không được gọi nếu isAuthenticated là false
//     bool result = service->isAdminAuthenticated();
//     ASSERT_FALSE(result);
// }

// TEST_F(AdminServiceTest, IsAdminAuthenticated_False_NotAdminRole) {
//     EXPECT_CALL(*mockSession, isAuthenticated()).WillOnce(Return(true));
//     EXPECT_CALL(*mockSession, getCurrentUserRole())
//         .WillOnce(Return(UserRole::STUDENT)); // (✅) WillOnce là đủ
//     bool result = service->isAdminAuthenticated();
//     ASSERT_FALSE(result);
// }

// TEST_F(AdminServiceTest, IsAdminAuthenticated_False_NoRole) {
//     EXPECT_CALL(*mockSession, isAuthenticated()).WillOnce(Return(true));
//     EXPECT_CALL(*mockSession, getCurrentUserRole())
//         .WillOnce(Return(std::nullopt)); // (✅) WillOnce là đủ
//     bool result = service->isAdminAuthenticated();
//     ASSERT_FALSE(result);
// }

// // --- END OF MODIFIED FILE tests/core/services/impl/AdminService_test.cpp ---