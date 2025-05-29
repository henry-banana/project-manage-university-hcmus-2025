#include <gtest/gtest.h>
#include "../mock/MockLoginDao.h"
#include "../common/UserRole.h"
#include "../common/LoginStatus.h"

LoginCredentials makeCredentials(
    const std::string& userId,
    const std::string& hash = "hashed_pw",
    const std::string& salt = "s@lt",
    UserRole role = UserRole::STUDENT,
    LoginStatus status = LoginStatus::ACTIVE
) {
    return LoginCredentials{userId, hash, salt, role, status};
}

TEST(MockLoginDaoTest, AddAndFindCredentials) {
    MockLoginDao dao;
    auto addResult = dao.addUserCredentials("user123", "hashed", "salt", UserRole::STUDENT, LoginStatus::ACTIVE);
    EXPECT_TRUE(addResult.has_value());
    EXPECT_TRUE(addResult.value());

    auto findResult = dao.findCredentialsByUserId("user123");
    EXPECT_TRUE(findResult.has_value());
    EXPECT_EQ(findResult->userId, "user123");
    EXPECT_EQ(findResult->passwordHash, "hashed");
    EXPECT_EQ(findResult->salt, "salt");
    EXPECT_EQ(findResult->role, UserRole::STUDENT);
    EXPECT_EQ(findResult->status, LoginStatus::ACTIVE);
}

TEST(MockLoginDaoTest, FindCredentials_NotFound) {
    MockLoginDao dao;
    auto result = dao.findCredentialsByUserId("not_exist");
    EXPECT_FALSE(result.has_value());
}

TEST(MockLoginDaoTest, UpdatePassword) {
    MockLoginDao dao;
    dao.addUserCredentials("user1", "oldHash", "oldSalt", UserRole::ADMIN, LoginStatus::ACTIVE);

    auto updateResult = dao.updatePassword("user1", "newHash", "newSalt");
    EXPECT_TRUE(updateResult.has_value());
    EXPECT_TRUE(updateResult.value());

    auto findResult = dao.findCredentialsByUserId("user1");
    EXPECT_EQ(findResult->passwordHash, "newHash");
    EXPECT_EQ(findResult->salt, "newSalt");
}

TEST(MockLoginDaoTest, RemoveCredentials) {
    MockLoginDao dao;
    dao.addUserCredentials("user1", "hash", "salt", UserRole::ADMIN, LoginStatus::ACTIVE);

    auto removeResult = dao.removeUserCredentials("user1");
    EXPECT_TRUE(removeResult.has_value());
    EXPECT_TRUE(removeResult.value());

    auto findResult = dao.findCredentialsByUserId("user1");
    EXPECT_FALSE(findResult.has_value());
}

TEST(MockLoginDaoTest, GetUserRole) {
    MockLoginDao dao;
    dao.addUserCredentials("teacher1", "hash", "salt", UserRole::TEACHER, LoginStatus::ACTIVE);

    auto roleResult = dao.getUserRole("teacher1");
    EXPECT_TRUE(roleResult.has_value());
    EXPECT_EQ(roleResult.value(), UserRole::TEACHER);
}

TEST(MockLoginDaoTest, GetUserStatus) {
    MockLoginDao dao;
    dao.addUserCredentials("student1", "hash", "salt", UserRole::STUDENT, LoginStatus::DISABLED);

    auto statusResult = dao.getUserStatus("student1");
    EXPECT_TRUE(statusResult.has_value());
    EXPECT_EQ(statusResult.value(), LoginStatus::DISABLED);
}

TEST(MockLoginDaoTest, UpdateRoleAndStatus) {
    MockLoginDao dao;
    dao.addUserCredentials("admin1", "hash", "salt", UserRole::ADMIN, LoginStatus::PENDING_APPROVAL);

    auto updateResult = dao.updateUserRoleAndStatus("admin1", UserRole::TEACHER, LoginStatus::ACTIVE);
    EXPECT_TRUE(updateResult.has_value());
    EXPECT_TRUE(updateResult.value());

    auto cred = dao.findCredentialsByUserId("admin1");
    EXPECT_TRUE(cred.has_value());
    EXPECT_EQ(cred->role, UserRole::TEACHER);
    EXPECT_EQ(cred->status, LoginStatus::ACTIVE);
}

TEST(MockLoginDaoTest, FindByStatus) {
    MockLoginDao dao;
    int numActive = dao.findByStatus(LoginStatus::ACTIVE).value_or(std::vector<LoginCredentials>()).size();
    int numDisabled = dao.findByStatus(LoginStatus::DISABLED).value_or(std::vector<LoginCredentials>()).size();
    dao.addUserCredentials("u1", "h", "s", UserRole::STUDENT, LoginStatus::ACTIVE);
    dao.addUserCredentials("u2", "h", "s", UserRole::TEACHER, LoginStatus::DISABLED);
    dao.addUserCredentials("u3", "h", "s", UserRole::ADMIN, LoginStatus::ACTIVE);

    auto activeUsers = dao.findByStatus(LoginStatus::ACTIVE);
    EXPECT_TRUE(activeUsers.has_value());
    EXPECT_EQ(activeUsers->size(), 2 + numActive);

    auto lockedUsers = dao.findByStatus(LoginStatus::DISABLED);
    EXPECT_TRUE(lockedUsers.has_value());
    EXPECT_EQ(lockedUsers->size(), 1+ numDisabled);
}
