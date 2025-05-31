#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../src/core/services/impl/AdminService.h"
#include "../../../src/core/data_access/mock/MockStudentDao.h"
#include "../../../src/core/data_access/mock/MockTeacherDao.h"
#include "../../../src/core/data_access/mock/MockLoginDao.h"
#include "../../../src/core/data_access/mock/MockFeeRecordDao.h"
#include "../../../src/core/data_access/mock/MockSalaryRecordDao.h"
#include "../../../../src/core/data_access/mock/MockFacultyDao.h"
#include "../../../src/core/data_access/mock/MockEnrollmentDao.h"
#include "../../../src/core/data_access/mock/MockCourseResultDao.h"
#include "../../../src/core/validators/impl/GeneralInputValidator.h"
#include "../../../src/core/services/SessionContext.h"
#include "../../../src/core/entities/Student.h"
#include "../../../src/core/entities/Teacher.h"
#include "../../../src/core/entities/FeeRecord.h"
#include "../../../src/core/entities/SalaryRecord.h"
#include "../../../src/core/entities/CourseResult.h"
#include "../src/common/ValidationResult.h"

#include <memory>
#include <expected>
#include <vector>
#include <string>

using ::testing::Return;
using ::testing::_;

// Mock validator
class MockGeneralInputValidator : public IGeneralInputValidator {
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

// Mock session context
class MockSessionContext : public SessionContext {
public:
    MOCK_METHOD(bool, isAuthenticated, (), (const, override));
    MOCK_METHOD(std::optional<std::string>, getCurrentUserId, (), (const, override));
    MOCK_METHOD(std::optional<UserRole>, getCurrentUserRole, (), (const, override));
    MOCK_METHOD(std::optional<std::shared_ptr<User>>, getCurrentUser, (), (const, override));
};

// MyMock cho từng DAO, kế thừa từ Mock*Dao trong folder mock
class MyMockStudentDao : public MockStudentDao {
public:
    std::function<std::expected<Student, Error>(const std::string&)> getByIdFunc;
    std::function<std::expected<std::vector<Student>, Error>(LoginStatus)> getByStatusFunc;
    std::function<std::expected<Student, Error>(const NewStudentDataByAdmin&)> addByAdminFunc;
    std::function<std::expected<bool, Error>(const std::string&)> removeFunc;
    std::expected<Student, Error> getById(const std::string& id) const override {
        return getByIdFunc ? getByIdFunc(id) : std::unexpected(Error{ErrorCode::NOT_FOUND, "not found"});
    }
    std::expected<std::vector<Student>, Error> getByStatus(LoginStatus status) const {
        return getByStatusFunc ? getByStatusFunc(status) : std::vector<Student>{};
    }
    std::expected<Student, Error> addByAdmin(const NewStudentDataByAdmin& data) {
        return addByAdminFunc ? addByAdminFunc(data) : std::unexpected(Error{ErrorCode::OPERATION_FAILED, "add failed"});
    }
    std::expected<bool, Error> remove(const std::string& id) override {
        return removeFunc ? removeFunc(id) : std::unexpected(Error{ErrorCode::NOT_FOUND, "remove failed"});
    }
};

class MyMockTeacherDao : public MockTeacherDao {
public:
    std::function<std::expected<Teacher, Error>(const std::string&)> getByIdFunc;
    std::function<std::expected<Teacher, Error>(const NewTeacherDataByAdmin&)> addByAdminFunc;
    std::function<std::expected<bool, Error>(const std::string&)> removeFunc;
    std::expected<Teacher, Error> getById(const std::string& id) const override {
        return getByIdFunc ? getByIdFunc(id) : std::unexpected(Error{ErrorCode::NOT_FOUND, "not found"});
    }
    std::expected<Teacher, Error> addByAdmin(const NewTeacherDataByAdmin& data) {
        return addByAdminFunc ? addByAdminFunc(data) : std::unexpected(Error{ErrorCode::OPERATION_FAILED, "add failed"});
    }
    std::expected<bool, Error> remove(const std::string& id) override {
        return removeFunc ? removeFunc(id) : std::unexpected(Error{ErrorCode::NOT_FOUND, "remove failed"});
    }
};

class MyMockLoginDao : public MockLoginDao {
public:
    std::function<std::expected<bool, Error>(const std::string&, const std::string&)> resetPasswordFunc;
    std::function<std::expected<bool, Error>(const std::string&)> disableFunc;
    std::function<std::expected<bool, Error>(const std::string&)> enableFunc;
    std::expected<bool, Error> resetPassword(const std::string& userId, const std::string& newPassword) {
        return resetPasswordFunc ? resetPasswordFunc(userId, newPassword) : std::unexpected(Error{ErrorCode::OPERATION_FAILED, "reset failed"});
    }
    std::expected<bool, Error> disable(const std::string& userId) {
        return disableFunc ? disableFunc(userId) : std::unexpected(Error{ErrorCode::OPERATION_FAILED, "disable failed"});
    }
    std::expected<bool, Error> enable(const std::string& userId) {
        return enableFunc ? enableFunc(userId) : std::unexpected(Error{ErrorCode::OPERATION_FAILED, "enable failed"});
    }
};

class MyMockFalcutyDao : public MockFacultyDao {};

class MyMockFeeRecordDao : public MockFeeRecordDao {};
class MyMockSalaryRecordDao : public MockSalaryRecordDao {};
class MyMockEnrollmentDao : public MockEnrollmentDao {};
class MyMockCourseResultDao : public MockCourseResultDao {};

// Test fixture
class AdminServiceTest : public ::testing::Test {
protected:
    std::shared_ptr<MyMockStudentDao> mockStudentDao = std::make_shared<MyMockStudentDao>();
    std::shared_ptr<MyMockTeacherDao> mockTeacherDao = std::make_shared<MyMockTeacherDao>();
    std::shared_ptr<MyMockFalcutyDao> mockFalcutyDao = std::make_shared<MyMockFalcutyDao>();
    std::shared_ptr<MyMockLoginDao> mockLoginDao = std::make_shared<MyMockLoginDao>();
    std::shared_ptr<MyMockFeeRecordDao> mockFeeDao = std::make_shared<MyMockFeeRecordDao>();
    std::shared_ptr<MyMockSalaryRecordDao> mockSalaryDao = std::make_shared<MyMockSalaryRecordDao>();
    std::shared_ptr<MyMockEnrollmentDao> mockEnrollmentDao = std::make_shared<MyMockEnrollmentDao>();
    std::shared_ptr<MyMockCourseResultDao> mockCourseResultDao = std::make_shared<MyMockCourseResultDao>();
    std::shared_ptr<MockGeneralInputValidator> mockValidator = std::make_shared<MockGeneralInputValidator>();
    std::shared_ptr<MockSessionContext> mockSession = std::make_shared<MockSessionContext>();
    AdminService service{
        mockStudentDao, mockTeacherDao, mockFalcutyDao, mockLoginDao, mockFeeDao,
        mockSalaryDao, mockEnrollmentDao, mockCourseResultDao,
        mockValidator, mockSession
    };
};

// approveStudentRegistration
TEST_F(AdminServiceTest, ApproveStudentRegistration_Success) {
    mockStudentDao->getByIdFunc = [](const std::string&) { return Student("S1", "Test", "Nguyen", "F001", LoginStatus::ACTIVE); };
    auto result = service.approveStudentRegistration("S1");
    ASSERT_TRUE(result.has_value());
}

TEST_F(AdminServiceTest, ApproveStudentRegistration_NotFound) {
    mockStudentDao->getByIdFunc = [](const std::string&) { return std::unexpected(Error{ErrorCode::NOT_FOUND, "not found"}); };
    auto result = service.approveStudentRegistration("S999");
    ASSERT_FALSE(result.has_value());
    ASSERT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

// getStudentsByStatus
TEST_F(AdminServiceTest, GetStudentsByStatus_Success) {
    mockStudentDao->getByStatusFunc = [](LoginStatus) {
        return std::vector<Student>{
            Student("S1", "A", "Nguyen", "F001", LoginStatus::ACTIVE),
            Student("S2", "B", "Tran", "F002", LoginStatus::ACTIVE)
        };
    };
    auto result = service.getStudentsByStatus(LoginStatus::ACTIVE);
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 2);
}

// addStudentByAdmin
TEST_F(AdminServiceTest, AddStudentByAdmin_Success) {
    mockStudentDao->addByAdminFunc = [](const NewStudentDataByAdmin&) {
        // Adjust the arguments to match the Student constructor signature
        return Student("S3", "New", "Nguyen", "F003", LoginStatus::ACTIVE);
    };
    auto result = service.addStudentByAdmin(NewStudentDataByAdmin{"S3"});
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getId(), "S3");
}

// removeStudentAccount
TEST_F(AdminServiceTest, RemoveStudentAccount_Success) {
    mockStudentDao->removeFunc = [](const std::string&) { return true; };
    auto result = service.removeStudentAccount("S1");
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result.value());
}

// addTeacherByAdmin
TEST_F(AdminServiceTest, AddTeacherByAdmin_Success) {
    mockTeacherDao->addByAdminFunc = [](const NewTeacherDataByAdmin&) {
        return Teacher("T1", "Nguyen", "Van A", "F001", LoginStatus::ACTIVE);
    };
    auto result = service.addTeacherByAdmin(NewTeacherDataByAdmin{"T1"});
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getId(), "T1");
}

// removeTeacherAccount
TEST_F(AdminServiceTest, RemoveTeacherAccount_Success) {
    mockTeacherDao->removeFunc = [](const std::string&) { return true; };
    auto result = service.removeTeacherAccount("T1");
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result.value());
}

// resetUserPassword
TEST_F(AdminServiceTest, ResetUserPassword_Success) {
    mockLoginDao->resetPasswordFunc = [](const std::string&, const std::string&) { return true; };
    auto result = service.resetUserPassword("U1", "newpass");
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result.value());
}

// disableUserAccount
TEST_F(AdminServiceTest, DisableUserAccount_Success) {
    mockLoginDao->disableFunc = [](const std::string&) { return true; };
    auto result = service.disableUserAccount("U1");
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result.value());
}

// enableUserAccount
TEST_F(AdminServiceTest, EnableUserAccount_Success) {
    mockLoginDao->enableFunc = [](const std::string&) { return true; };
    auto result = service.enableUserAccount("U1");
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result.value());
}

// isAdminAuthenticated
TEST_F(AdminServiceTest, IsAdminAuthenticated_True) {
    EXPECT_CALL(*mockSession, isAuthenticated()).WillOnce(Return(true));
    bool result = service.isAdminAuthenticated();
    ASSERT_TRUE(result);
}

TEST_F(AdminServiceTest, IsAdminAuthenticated_False) {
    EXPECT_CALL(*mockSession, isAuthenticated()).WillOnce(Return(false));
    bool result = service.isAdminAuthenticated();
    ASSERT_FALSE(result);
}