#include <gtest/gtest.h>
#include <any>
#include <string>
#include <map>
#include <vector>
#include "../src/core/parsing/impl_sql_parser/TeacherSqlParser.h"
#include "../src/common/ErrorType.h"

// Alias cho dòng kết quả truy vấn SQL
using DbQueryResultRow = std::map<std::string, std::any>;

class TeacherSqlParserTest : public ::testing::Test {
protected:
    TeacherSqlParser parser;

    // Tạo 1 DbQueryResultRow hợp lệ làm mẫu test
    DbQueryResultRow makeValidRow() {
        return DbQueryResultRow{
            {"id", std::string("T001")},
            {"firstName", std::string("Alice")},
            {"lastName", std::string("Smith")},
            {"birthDay", 10},
            {"birthMonth", 8},
            {"birthYear", 1980},
            {"address", std::string("456 College St")},
            {"citizenId", std::string("987654321")},
            {"email", std::string("alice.smith@uni.edu")},
            {"phoneNumber", std::string("0987654321")},
            {"role", static_cast<int>(UserRole::TEACHER)},
            {"status", static_cast<int>(LoginStatus::ACTIVE)},
            {"departmentId", std::string("D002")}
        };
    }
};

TEST_F(TeacherSqlParserTest, Parse_ValidRow_ReturnsTeacher) {
    auto row = makeValidRow();

    auto result = parser.parse(row);
    ASSERT_TRUE(result.has_value()) << "Parsing valid row should succeed";

    const Teacher& teacher = result.value();
    EXPECT_EQ(teacher.getId(), "T001");
    EXPECT_EQ(teacher.getFirstName(), "Alice");
    EXPECT_EQ(teacher.getLastName(), "Smith");
    EXPECT_EQ(teacher.getStatus(), LoginStatus::ACTIVE);
    EXPECT_EQ(teacher.getRole(), UserRole::TEACHER);
    EXPECT_EQ(teacher.getFacultyId(), "D002");
}

TEST_F(TeacherSqlParserTest, Parse_MissingId_ReturnsError) {
    auto row = makeValidRow();
    row.erase("id");

    auto result = parser.parse(row);
    EXPECT_FALSE(result.has_value());
    ASSERT_EQ(result.error().code, ErrorCode::PARSING_ERROR);
}

TEST_F(TeacherSqlParserTest, Serialize_ValidTeacher_ReturnsRow) {
    Teacher teacher("T001", "Alice", "Smith", "D002", LoginStatus::ACTIVE);
    // Giả định Teacher có constructor phù hợp hoặc đã set đầy đủ thuộc tính

    auto result = parser.serialize(teacher);
    ASSERT_TRUE(result.has_value());

    const auto& row = result.value();
    EXPECT_EQ(std::any_cast<std::string>(row.at("id")), "T001");
    EXPECT_EQ(std::any_cast<std::string>(row.at("departmentId")), "D002");
}

TEST_F(TeacherSqlParserTest, ToQueryInsertParams_ReturnsCorrectParams) {
    Teacher teacher("T001", "Alice", "Smith", "D002", LoginStatus::ACTIVE);

    auto result = parser.toQueryInsertParams(teacher);
    ASSERT_TRUE(result.has_value());

    auto params = result.value();
    EXPECT_GE(params.size(), 1);
    EXPECT_EQ(std::any_cast<std::string>(params[0]), "T001");  // ID đầu tiên
}

TEST_F(TeacherSqlParserTest, ToQueryUpdateParams_ReturnsCorrectParams) {
    Teacher teacher("T001", "Alice", "Smith", "D002", LoginStatus::ACTIVE);

    auto result = parser.toQueryUpdateParams(teacher);
    ASSERT_TRUE(result.has_value());

    auto params = result.value();
    EXPECT_GE(params.size(), 1);
    EXPECT_EQ(std::any_cast<std::string>(params.back()), "T001");  // ID cuối cùng
}
