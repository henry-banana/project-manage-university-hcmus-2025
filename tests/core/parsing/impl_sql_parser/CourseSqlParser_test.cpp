#include <gtest/gtest.h>
#include <any>
#include <string>
#include <map>
#include <vector>
#include "../src/core/parsing/impl_sql_parser/CourseSqlParser.h"
#include "ErrorType.h"

// Alias cho dòng kết quả query SQL
using DbQueryResultRow = std::map<std::string, std::any>;

class CourseSqlParserTest : public ::testing::Test {
protected:
    CourseSqlParser parser;

    // Tạo 1 dòng dữ liệu hợp lệ đại diện cho Course
    DbQueryResultRow makeValidRow() {
    return DbQueryResultRow{
        {"id", std::string("CS101")},
        {"name", std::string("Introduction to Programming")},
        {"credits", static_cast<long long>(3)}, // (<<<< Lưu ý kiểu ở đây)
        {"facultyId", std::string("F001")}
    };
}
};

TEST_F(CourseSqlParserTest, Parse_ValidRow_ReturnsCourse) {
    auto row = makeValidRow();

    auto result = parser.parse(row);
    ASSERT_TRUE(result.has_value()) << "Parsing valid row should succeed";

    const Course& course = result.value();
    EXPECT_EQ(course.getId(), "CS101");
    EXPECT_EQ(course.getName(), "Introduction to Programming");
    EXPECT_EQ(course.getCredits(), 3);
    EXPECT_EQ(course.getFacultyId(), "F001");
}

TEST_F(CourseSqlParserTest, Parse_MissingId_ReturnsError) {
    auto row = makeValidRow();
    row.erase("id");

    auto result = parser.parse(row);
    EXPECT_FALSE(result.has_value());
    ASSERT_EQ(result.error().code, ErrorCode::PARSING_ERROR);
}

TEST_F(CourseSqlParserTest, Serialize_ValidCourse_ReturnsRow) {
    Course course("CS101", "Introduction to Programming", 3, "F001");

    auto result = parser.serialize(course);
    ASSERT_TRUE(result.has_value());

    const auto& row = result.value();
    EXPECT_EQ(std::any_cast<std::string>(row.at("id")), "CS101");
    EXPECT_EQ(std::any_cast<std::string>(row.at("name")), "Introduction to Programming");
    EXPECT_EQ(std::any_cast<int>(row.at("credits")), 3);
    EXPECT_EQ(std::any_cast<std::string>(row.at("facultyId")), "F001");
}

TEST_F(CourseSqlParserTest, ToQueryInsertParams_ReturnsCorrectParams) {
    Course course("CS101", "Introduction to Programming", 3, "F001");

    auto result = parser.toQueryInsertParams(course);
    ASSERT_TRUE(result.has_value());

    auto params = result.value();
    EXPECT_GE(params.size(), 1);
    EXPECT_EQ(std::any_cast<std::string>(params[0]), "CS101");  // ID đầu tiên
    // Có thể kiểm tra thêm name, credits, facultyId nếu muốn
}

TEST_F(CourseSqlParserTest, ToQueryUpdateParams_ReturnsCorrectParams) {
    Course course("CS101", "Introduction to Programming", 3, "F001");

    auto result = parser.toQueryUpdateParams(course);
    ASSERT_TRUE(result.has_value());

    auto params = result.value();
    EXPECT_GE(params.size(), 1);
    EXPECT_EQ(std::any_cast<std::string>(params.back()), "CS101");  // ID cuối cùng thường là điều kiện WHERE
}
