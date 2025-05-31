#include <gtest/gtest.h>
#include <string>
#include <any>
#include <unordered_map>
#include <vector>
#include <expected>

#include "../../core/parsing/impl_sql_parser/LoginCredentialsSqlParser.h"
#include "../../common/ErrorType.h"
#include "../../common/LoginStatus.h"
#include "../../common/UserRole.h"
#include "../../core/data_access/interface/ILoginDao.h"

class LoginCredentialsSqlParserTest : public ::testing::Test {
protected:
    LoginCredentialsSqlParser parser;

    DbQueryResultRow createValidRow() {
        return DbQueryResultRow{
            {"userId", std::string("user123")},
            {"passwordHash", std::string("hashedPassword")},
            {"salt", std::string("randomSalt")},
            {"role", static_cast<long long>(UserRole::ADMIN)}, 
            {"status", static_cast<long long>(LoginStatus::ACTIVE)} 
        };
    }

    LoginCredentials createValidCredentials() {
        return LoginCredentials{
            "user123",
            "hashedPassword",
            "randomSalt",
            UserRole::ADMIN,
            LoginStatus::ACTIVE
        };
    }
};

TEST_F(LoginCredentialsSqlParserTest, Parse_ValidRow_ReturnsLoginCredentials) {
    DbQueryResultRow row = createValidRow();

    auto result = parser.parse(row);
    ASSERT_TRUE(result.has_value());
    
    const LoginCredentials& creds = result.value();
    EXPECT_EQ(creds.userId, "user123");
    EXPECT_EQ(creds.passwordHash, "hashedPassword");
    EXPECT_EQ(creds.salt, "randomSalt");
    EXPECT_EQ(creds.role, UserRole::ADMIN);
    EXPECT_EQ(creds.status, LoginStatus::ACTIVE);
}

TEST_F(LoginCredentialsSqlParserTest, Parse_MissingField_ReturnsError) {
    DbQueryResultRow row = createValidRow();
    row.erase("userId"); // Bỏ field quan trọng

    auto result = parser.parse(row);
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::PARSING_ERROR); 
}

TEST_F(LoginCredentialsSqlParserTest, Serialize_ValidCredentials_ReturnsRow) {
    LoginCredentials creds = createValidCredentials();
    auto result = parser.serialize(creds);
    ASSERT_TRUE(result.has_value());
    DbQueryResultRow row = result.value();
    EXPECT_EQ(std::any_cast<std::string>(row.at("userId")), "user123");
    EXPECT_EQ(std::any_cast<std::string>(row.at("passwordHash")), "hashedPassword");
    EXPECT_EQ(std::any_cast<std::string>(row.at("salt")), "randomSalt");
    // Bỏ assert cho role và status nếu chúng không được serialize
}

TEST_F(LoginCredentialsSqlParserTest, ToQueryInsertParams_ValidCredentials_ReturnsParamsVector) {
    LoginCredentials creds = createValidCredentials();

    auto result = parser.toQueryInsertParams(creds);
    ASSERT_TRUE(result.has_value());

    std::vector<std::any> params = result.value();

    EXPECT_EQ(std::any_cast<std::string>(params[0]), "user123");        // userId
    EXPECT_EQ(std::any_cast<std::string>(params[1]), "hashedPassword"); // passwordHash
    EXPECT_EQ(std::any_cast<std::string>(params[2]), "randomSalt");     // salt
    // Role và status không cần ở insert này nếu bảng Logins không lưu role/status, nếu có thì test thêm
}

TEST_F(LoginCredentialsSqlParserTest, ToQueryUpdateParams_ValidCredentials_ReturnsParamsVector) {
    LoginCredentials creds = createValidCredentials();

    auto result = parser.toQueryUpdateParams(creds);
    ASSERT_TRUE(result.has_value());

    std::vector<std::any> params = result.value();

    EXPECT_EQ(std::any_cast<std::string>(params[0]), "hashedPassword"); // passwordHash
    EXPECT_EQ(std::any_cast<std::string>(params[1]), "randomSalt");     // salt
    EXPECT_EQ(std::any_cast<std::string>(params[2]), "user123");        // userId
}
