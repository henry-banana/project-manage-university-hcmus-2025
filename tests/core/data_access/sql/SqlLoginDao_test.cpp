#include "gtest/gtest.h"
#include "../../../../src/core/data_access/sql/SqlLoginDao.h"
#include "../../../../src/core/database_adapter/sql/SQLiteAdapter.h"
#include "../../../../src/core/parsing/impl_sql_parser/LoginCredentialsSqlParser.h"
#include "../../../../src/core/data_access/interface/ILoginDao.h"
#include "../../../../src/common/ErrorType.h"
#include <memory>

class SqlLoginDaoTest : public ::testing::Test {
protected:
    std::shared_ptr<IDatabaseAdapter> dbAdapter;
    std::shared_ptr<IEntityParser<LoginCredentials, DbQueryResultRow>> parser;
    std::unique_ptr<SqlLoginDao> loginDao;

    void SetUp() override {
        dbAdapter = std::make_shared<SQLiteAdapter>();
        auto result = dbAdapter->connect(":memory:");
        ASSERT_TRUE(result.has_value() && result.value()) << "DB connect failed";

        auto schemaResult = std::static_pointer_cast<SQLiteAdapter>(dbAdapter)->ensureTablesExist();
        ASSERT_TRUE(schemaResult.has_value() && schemaResult.value()) << "Ensure tables failed";

        parser = std::make_shared<LoginCredentialsSqlParser>();
        loginDao = std::make_unique<SqlLoginDao>(dbAdapter, parser);
    }

    void TearDown() override {
        if (dbAdapter && dbAdapter->isConnected()) {
            dbAdapter->disconnect();
        }
    }

    void insertCredentialsDirectly(const std::string& id, const std::string& hash, const std::string& salt, UserRole role, LoginStatus status) {
        auto query = "INSERT INTO Logins (userId, passwordHash, salt) VALUES (?, ?, ?);";
        auto result = dbAdapter->executeUpdate(query, {id, hash, salt});
        ASSERT_TRUE(result.has_value()) << result.error().message;
    }
};

// --- TESTS ---

TEST_F(SqlLoginDaoTest, AddUserCredentials_Success) {
    auto result = loginDao->addUserCredentials("user1", "hashed", "salt123", UserRole::ADMIN, LoginStatus::ACTIVE);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(SqlLoginDaoTest, AddUserCredentials_DuplicateId_ReturnsError) {
    insertCredentialsDirectly("user2", "h1", "s1", UserRole::TEACHER, LoginStatus::ACTIVE);
    auto result = loginDao->addUserCredentials("user2", "h2", "s2", UserRole::TEACHER, LoginStatus::ACTIVE);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::ALREADY_EXISTS);
}

TEST_F(SqlLoginDaoTest, FindCredentialsByUserId_Exists) {
    insertCredentialsDirectly("user3", "hash3", "salt3", UserRole::TEACHER, LoginStatus::ACTIVE);
    auto result = loginDao->findCredentialsByUserId("user3");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().userId, "user3");
    EXPECT_EQ(result.value().role, UserRole::TEACHER);
}

TEST_F(SqlLoginDaoTest, FindCredentialsByUserId_NotFound) {
    auto result = loginDao->findCredentialsByUserId("no_user");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlLoginDaoTest, UpdatePassword_Success) {
    insertCredentialsDirectly("user4", "old", "oldsalt", UserRole::STUDENT, LoginStatus::ACTIVE);
    auto result = loginDao->updatePassword("user4", "newHash", "newSalt");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(SqlLoginDaoTest, UpdatePassword_UserNotFound) {
    auto result = loginDao->updatePassword("ghost", "x", "y");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlLoginDaoTest, RemoveUserCredentials_Success) {
    insertCredentialsDirectly("user5", "h", "s", UserRole::STUDENT, LoginStatus::DISABLED);
    auto result = loginDao->removeUserCredentials("user5");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());
}

TEST_F(SqlLoginDaoTest, RemoveUserCredentials_NotFound) {
    auto result = loginDao->removeUserCredentials("not_exist");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlLoginDaoTest, GetUserRole_Exists) {
    insertCredentialsDirectly("user6", "h", "s", UserRole::ADMIN, LoginStatus::ACTIVE);
    auto result = loginDao->getUserRole("user6");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), UserRole::ADMIN);
}

TEST_F(SqlLoginDaoTest, GetUserRole_NotFound) {
    auto result = loginDao->getUserRole("ghost2");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlLoginDaoTest, GetUserStatus_Exists) {
    insertCredentialsDirectly("user7", "h", "s", UserRole::STUDENT, LoginStatus::DISABLED);
    auto result = loginDao->getUserStatus("user7");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), LoginStatus::DISABLED);
}

TEST_F(SqlLoginDaoTest, GetUserStatus_NotFound) {
    auto result = loginDao->getUserStatus("ghost3");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlLoginDaoTest, UpdateUserRoleAndStatus_Success) {
    insertCredentialsDirectly("user8", "h", "s", UserRole::STUDENT, LoginStatus::ACTIVE);
    auto result = loginDao->updateUserRoleAndStatus("user8", UserRole::ADMIN, LoginStatus::DISABLED);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result.value());

    auto role = loginDao->getUserRole("user8");
    auto status = loginDao->getUserStatus("user8");
    EXPECT_EQ(role.value(), UserRole::ADMIN);
    EXPECT_EQ(status.value(), LoginStatus::DISABLED);
}

TEST_F(SqlLoginDaoTest, UpdateUserRoleAndStatus_UserNotFound) {
    auto result = loginDao->updateUserRoleAndStatus("ghost4", UserRole::ADMIN, LoginStatus::DISABLED);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::NOT_FOUND);
}

TEST_F(SqlLoginDaoTest, FindByStatus_ReturnsCorrectUsers) {
    insertCredentialsDirectly("u1", "h", "s", UserRole::TEACHER, LoginStatus::DISABLED);
    insertCredentialsDirectly("u2", "h", "s", UserRole::TEACHER, LoginStatus::DISABLED);
    insertCredentialsDirectly("u3", "h", "s", UserRole::TEACHER, LoginStatus::ACTIVE);
    auto result = loginDao->findByStatus(LoginStatus::DISABLED);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value().size(), 2);
}
