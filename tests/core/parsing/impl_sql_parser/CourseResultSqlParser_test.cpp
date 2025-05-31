#include <gtest/gtest.h>
#include <any>
#include <string>
#include <map>
#include <vector>
#include "../src/core/parsing/impl_sql_parser/CourseResultSqlParser.h"
#include "../src/common/ErrorType.h"

using DbQueryResultRow = std::map<std::string, std::any>;

class CourseResultSqlParserTest : public ::testing::Test {
protected:
    CourseResultSqlParser parser;

    DbQueryResultRow makeValidRow() {
        return DbQueryResultRow{
            {"studentId", std::string("SV001")},
            {"courseId", std::string("CS101")},
            {"marks", static_cast<long long>(85)}, // Hoặc int(85)
        };
    }
};

TEST_F(CourseResultSqlParserTest, Parse_ValidRow_ReturnsCourseResult) {
    auto row = makeValidRow(); // row["marks"] = 85LL
    auto result = parser.parse(row);
    ASSERT_TRUE(result.has_value()) << "Parsing valid row should succeed";
    const CourseResult& cr = result.value();
    EXPECT_EQ(cr.getStudentId(), "SV001");
    EXPECT_EQ(cr.getCourseId(), "CS101");
    EXPECT_EQ(cr.getMarks(), 85); // Kiểm tra marks
    EXPECT_EQ(cr.getGrade(), 'A');
}

TEST_F(CourseResultSqlParserTest, Parse_MissingStudentId_ReturnsError) {
    auto row = makeValidRow();
    row.erase("studentId");

    auto result = parser.parse(row);
    EXPECT_FALSE(result.has_value());
    ASSERT_EQ(result.error().code, ErrorCode::PARSING_ERROR);
}

TEST_F(CourseResultSqlParserTest, Serialize_ValidCourseResult_ReturnsRow) {
    CourseResult cr("SV001", "CS101", 85);

    auto result = parser.serialize(cr);
    ASSERT_TRUE(result.has_value());

    const auto& row = result.value();
    EXPECT_EQ(std::any_cast<std::string>(row.at("studentId")), "SV001");
    EXPECT_EQ(std::any_cast<std::string>(row.at("courseId")), "CS101");
    EXPECT_EQ(std::any_cast<int>(row.at("marks")), 85);
}

TEST_F(CourseResultSqlParserTest, ToQueryInsertParams_ReturnsCorrectParams) {
    CourseResult cr("SV001", "CS101", 85);

    auto result = parser.toQueryInsertParams(cr);
    ASSERT_TRUE(result.has_value());

    auto params = result.value();
    ASSERT_GE(params.size(), 3);

    EXPECT_EQ(std::any_cast<std::string>(params[0]), "SV001");  // studentId
    EXPECT_EQ(std::any_cast<std::string>(params[1]), "CS101");  // courseId
    EXPECT_EQ(std::any_cast<int>(params[2]), 85);               // marks
}

TEST_F(CourseResultSqlParserTest, ToQueryUpdateParams_ReturnsCorrectParams) {
    CourseResult cr("SV001", "CS101", 85);

    auto result = parser.toQueryUpdateParams(cr);
    ASSERT_TRUE(result.has_value());

    auto params = result.value();
    ASSERT_GE(params.size(), 3);

    EXPECT_EQ(std::any_cast<int>(params[0]), 85);               // marks
    EXPECT_EQ(std::any_cast<std::string>(params[1]), "SV001");  // studentId for WHERE
    EXPECT_EQ(std::any_cast<std::string>(params[2]), "CS101");  // courseId for WHERE
}
