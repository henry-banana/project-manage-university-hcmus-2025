#include <gtest/gtest.h>
#include <any>
#include <string>
#include <map>
#include "../src/core/parsing/impl_sql_parser/StudentSqlParser.h"
#include "../src/common/ErrorType.h"

// Giả định kiểu DbQueryResultRow là map<string, any>
using DbQueryResultRow = std::map<std::string, std::any>;

class StudentSqlParserTest : public ::testing::Test {
protected:
    StudentSqlParser parser;

    DbQueryResultRow makeValidRow() {
        return DbQueryResultRow{
            {"id", std::string("S123")},
            {"firstName", std::string("John")},
            {"lastName", std::string("Doe")},
            {"birthDay", 15},
            {"birthMonth", 4},
            {"birthYear", 2000},
            {"address", std::string("123 Main St")},
            {"citizenId", std::string("123456789")},
            {"email", std::string("john.doe@example.com")},
            {"phoneNumber", std::string("0123456789")},
            {"role", static_cast<int>(UserRole::STUDENT)},
            {"status", static_cast<int>(LoginStatus::ACTIVE)},
            {"facultyId", std::string("F001")}
        };
    }
};

TEST_F(StudentSqlParserTest, Parse_ValidRow_ReturnsStudent) {
    auto row = makeValidRow();

    auto result = parser.parse(row);
    ASSERT_TRUE(result.has_value());

    const Student& student = result.value();
    EXPECT_EQ(student.getId(), "S123");
    EXPECT_EQ(student.getFirstName(), "John");
    EXPECT_EQ(student.getLastName(), "Doe");
    EXPECT_EQ(student.getFacultyId(), "F001");
    EXPECT_EQ(student.getStatus(), LoginStatus::ACTIVE);
    // ... các assert khác nếu có getter cho các trường User
}

TEST_F(StudentSqlParserTest, Parse_MissingField_ReturnsError) {
    auto row = makeValidRow();
    row.erase("id");  // Bỏ trường bắt buộc

    auto result = parser.parse(row);
    EXPECT_FALSE(result.has_value());
    ASSERT_TRUE(result.error().code == ErrorCode::PARSING_ERROR);
}

TEST_F(StudentSqlParserTest, ToQueryInsertParams_ValidStudent_ReturnsParams) {
    Student student("S123", "John", "Doe", "F001", LoginStatus::ACTIVE);
    // Giả sử có setter cho các trường kế thừa hoặc khởi tạo đầy đủ

    auto result = parser.toQueryInsertParams(student);
    ASSERT_TRUE(result.has_value());

    auto params = result.value();
    EXPECT_EQ(params.size(), 13);
    EXPECT_EQ(std::any_cast<std::string>(params[0]), "S123");
    EXPECT_EQ(std::any_cast<std::string>(params[1]), "John");
    EXPECT_EQ(std::any_cast<std::string>(params[12]), "F001");
}

TEST_F(StudentSqlParserTest, ToQueryUpdateParams_ValidStudent_ReturnsParams) {
    Student student("S123", "John", "Doe", "F001", LoginStatus::ACTIVE);

    auto result = parser.toQueryUpdateParams(student);
    ASSERT_TRUE(result.has_value());

    auto params = result.value();
    EXPECT_EQ(params.size(), 13);
    EXPECT_EQ(std::any_cast<std::string>(params.back()), "S123");  // ID cuối cùng
}

TEST_F(StudentSqlParserTest, Serialize_ValidStudent_ReturnsRow) {
    Student student("S123", "John", "Doe", "F001", LoginStatus::ACTIVE);
    // Giả sử có setter cho các trường kế thừa hoặc khởi tạo đầy đủ

    auto result = parser.serialize(student);
    ASSERT_TRUE(result.has_value());

    const DbQueryResultRow& row = result.value();
    EXPECT_EQ(std::any_cast<std::string>(row.at("id")), "S123");
    EXPECT_EQ(std::any_cast<std::string>(row.at("firstName")), "John");
    EXPECT_EQ(std::any_cast<std::string>(row.at("lastName")), "Doe");
    EXPECT_EQ(std::any_cast<std::string>(row.at("facultyId")), "F001");
}

