// --- START OF MODIFIED FILE tests/core/data_access/mock/MockLoginDao_test.cpp ---
#include <gtest/gtest.h>
#include "../../../../src/core/data_access/mock/MockLoginDao.h" // (➕)
#include "../../../../src/common/UserRole.h"        // (➕)
#include "../../../../src/common/LoginStatus.h"     // (➕)
#include "../../../../src/common/ErrorType.h"       // (➕)

// LoginCredentials đã được define trong ILoginDao.h

class MockLoginDaoTest : public ::testing::Test { // (➕)
protected:
    MockLoginDao dao;

    void SetUp() override {
        MockLoginDao::clearMockData();
        MockLoginDao::initializeDefaultMockData(); // Load admin, S001, T001, S002_Pending, T003_Disabled
    }
    void TearDown() override {
        MockLoginDao::clearMockData();
    }
};


TEST_F(MockLoginDaoTest, AddAndFindCredentials) {
    // Dao đã có data từ SetUp, add user mới
    auto addResult = dao.addUserCredentials("user123_test", "hashed_test", "salt_test", UserRole::STUDENT, LoginStatus::ACTIVE);
    ASSERT_TRUE(addResult.has_value()) << addResult.error().message;
    EXPECT_TRUE(addResult.value());

    auto findResult = dao.findCredentialsByUserId("user123_test");
    ASSERT_TRUE(findResult.has_value());
    EXPECT_EQ(findResult->userId, "user123_test");
    EXPECT_EQ(findResult->passwordHash, "hashed_test");
}

TEST_F(MockLoginDaoTest, FindCredentials_Existing_DefaultData) {
    auto findResult = dao.findCredentialsByUserId("admin"); // "admin" có trong default data
    ASSERT_TRUE(findResult.has_value());
    EXPECT_EQ(findResult->userId, "admin");
    EXPECT_EQ(findResult->role, UserRole::ADMIN);
}


TEST_F(MockLoginDaoTest, FindCredentials_NotFound) {
    auto result = dao.findCredentialsByUserId("not_exist_user");
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(MockLoginDaoTest, UpdatePassword_Success) {
    // "S001" có trong default data
    auto updateResult = dao.updatePassword("S001", "newHashS001", "newSaltS001");
    ASSERT_TRUE(updateResult.has_value()) << updateResult.error().message;
    EXPECT_TRUE(updateResult.value());

    auto findResult = dao.findCredentialsByUserId("S001");
    ASSERT_TRUE(findResult.has_value());
    EXPECT_EQ(findResult->passwordHash, "newHashS001");
    EXPECT_EQ(findResult->salt, "newSaltS001");
}

TEST_F(MockLoginDaoTest, UpdatePassword_NotFound_ShouldFail) {
    auto updateResult = dao.updatePassword("NON_EXIST_UPD_PASS", "newHash", "newSalt");
    ASSERT_FALSE(updateResult.has_value());
    EXPECT_EQ(updateResult.error().code, ErrorCode::NOT_FOUND);
}


TEST_F(MockLoginDaoTest, RemoveCredentials_Success) {
    // "T001" có trong default data
    auto removeResult = dao.removeUserCredentials("T001");
    ASSERT_TRUE(removeResult.has_value()) << removeResult.error().message;
    EXPECT_TRUE(removeResult.value());

    auto findResult = dao.findCredentialsByUserId("T001");
    EXPECT_FALSE(findResult.has_value());
}

TEST_F(MockLoginDaoTest, RemoveCredentials_NotFound_ShouldFail) {
    auto removeResult = dao.removeUserCredentials("NON_EXIST_REM_CRED");
    ASSERT_FALSE(removeResult.has_value());
    EXPECT_EQ(removeResult.error().code, ErrorCode::NOT_FOUND);
}


TEST_F(MockLoginDaoTest, GetUserRole_Success) {
    // "T001" có trong default data, role TEACHER
    auto roleResult = dao.getUserRole("T001");
    ASSERT_TRUE(roleResult.has_value()) << roleResult.error().message;
    EXPECT_EQ(roleResult.value(), UserRole::TEACHER);
}

TEST_F(MockLoginDaoTest, GetUserRole_NotFound_ShouldFail) {
    auto roleResult = dao.getUserRole("NON_EXIST_ROLE");
    ASSERT_FALSE(roleResult.has_value());
    EXPECT_EQ(roleResult.error().code, ErrorCode::NOT_FOUND);
}


TEST_F(MockLoginDaoTest, GetUserStatus_Success) {
    // "T003_Disabled" có trong default data, status DISABLED
    auto statusResult = dao.getUserStatus("T003_Disabled");
    ASSERT_TRUE(statusResult.has_value()) << statusResult.error().message;
    EXPECT_EQ(statusResult.value(), LoginStatus::DISABLED);
}

TEST_F(MockLoginDaoTest, GetUserStatus_NotFound_ShouldFail) {
    auto statusResult = dao.getUserStatus("NON_EXIST_STATUS");
    ASSERT_FALSE(statusResult.has_value());
    EXPECT_EQ(statusResult.error().code, ErrorCode::NOT_FOUND);
}


TEST_F(MockLoginDaoTest, UpdateRoleAndStatus_Success) {
    // "S002_Pending" có trong default data
    auto updateResult = dao.updateUserRoleAndStatus("S002_Pending", UserRole::STUDENT, LoginStatus::ACTIVE);
    ASSERT_TRUE(updateResult.has_value()) << updateResult.error().message;
    EXPECT_TRUE(updateResult.value());

    auto cred = dao.findCredentialsByUserId("S002_Pending");
    ASSERT_TRUE(cred.has_value());
    EXPECT_EQ(cred->role, UserRole::STUDENT);
    EXPECT_EQ(cred->status, LoginStatus::ACTIVE);
}

TEST_F(MockLoginDaoTest, UpdateRoleAndStatus_NotFound_ShouldFail) {
    auto updateResult = dao.updateUserRoleAndStatus("NON_EXIST_UPD_ROLE", UserRole::STUDENT, LoginStatus::ACTIVE);
    ASSERT_FALSE(updateResult.has_value());
    EXPECT_EQ(updateResult.error().code, ErrorCode::NOT_FOUND);
}


TEST_F(MockLoginDaoTest, FindByStatus) {
    // initializeDefaultMockData() thêm:
    // ACTIVE: admin, S001, T001 (3 users)
    // PENDING_APPROVAL: S002_Pending (1 user)
    // DISABLED: T003_Disabled (1 user)

    auto activeUsers = dao.findByStatus(LoginStatus::ACTIVE);
    ASSERT_TRUE(activeUsers.has_value());
    EXPECT_EQ(activeUsers->size(), 3);

    auto pendingUsers = dao.findByStatus(LoginStatus::PENDING_APPROVAL);
    ASSERT_TRUE(pendingUsers.has_value());
    EXPECT_EQ(pendingUsers->size(), 1);
    
    auto disabledUsers = dao.findByStatus(LoginStatus::DISABLED);
    ASSERT_TRUE(disabledUsers.has_value());
    EXPECT_EQ(disabledUsers->size(), 1);

    // Thêm một user ACTIVE mới để kiểm tra
    dao.addUserCredentials("u_active_test", "h", "s", UserRole::STUDENT, LoginStatus::ACTIVE);
    activeUsers = dao.findByStatus(LoginStatus::ACTIVE);
    ASSERT_TRUE(activeUsers.has_value());
    EXPECT_EQ(activeUsers->size(), 3 + 1);
}
// --- END OF MODIFIED FILE tests/core/data_access/mock/MockLoginDao_test.cpp ---