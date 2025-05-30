#include <gtest/gtest.h>
#include <expected>
#include <string>
#include <map>
#include <any>
#include <vector>

#include "FacultySqlParser.h"
#include "../../entities/Faculty.h"
#include "../../common/ErrorType.h"  // chứa Error, ErrorCode
#include "../../core/parsing/impl_sql_parser/FacultySqlParser.h"

using DbQueryResultRow = std::map<std::string, std::any>;

class FacultySqlParserTest : public ::testing::Test {
protected:
    FacultySqlParser parser;
};

// Giả lập Faculty constructor
Faculty makeFaculty(const std::string& id, const std::string& name) {
    return Faculty(id, name);
}

// Test parse thành công khi có đủ id, name
TEST_F(FacultySqlParserTest, Parse_ValidRow_Success) {
    DbQueryResultRow row = {
        {"id", std::string("IT")},
        {"name", std::string("Information Technology")}
    };

    auto result = parser.parse(row);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getId(), "IT");
    EXPECT_EQ(result->getName(), "Information Technology");
}

// Test parse lỗi khi thiếu id
TEST_F(FacultySqlParserTest, Parse_MissingId_Error) {
    DbQueryResultRow row = {
        {"name", std::string("Information Technology")}
    };

    auto result = parser.parse(row);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::PARSING_ERROR);
}

// Test parse lỗi khi thiếu name
TEST_F(FacultySqlParserTest, Parse_MissingName_Error) {
    DbQueryResultRow row = {
        {"id", std::string("IT")}
    };

    auto result = parser.parse(row);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::PARSING_ERROR);
}

// Test serialize trả về đúng map với id, name
TEST_F(FacultySqlParserTest, Serialize_ValidFaculty_Success) {
    Faculty faculty = makeFaculty("IT", "Information Technology");
    auto result = parser.serialize(faculty);

    ASSERT_TRUE(result.has_value());
    const auto& row = result.value();
    ASSERT_TRUE(row.find("id") != row.end());
    ASSERT_TRUE(row.find("name") != row.end());

    EXPECT_EQ(std::any_cast<std::string>(row.at("id")), "IT");
    EXPECT_EQ(std::any_cast<std::string>(row.at("name")), "Information Technology");
}

// Test toQueryInsertParams trả về đúng vector any chứa id, name
TEST_F(FacultySqlParserTest, ToQueryInsertParams_ValidFaculty_Success) {
    Faculty faculty = makeFaculty("IT", "Information Technology");
    auto result = parser.toQueryInsertParams(faculty);

    ASSERT_TRUE(result.has_value());
    const auto& params = result.value();

    ASSERT_EQ(params.size(), 2u);
    EXPECT_EQ(std::any_cast<std::string>(params[0]), "IT");
    EXPECT_EQ(std::any_cast<std::string>(params[1]), "Information Technology");
}

// Test toQueryUpdateParams trả về vector any đúng id, name (thường update cả 2)
TEST_F(FacultySqlParserTest, ToQueryUpdateParams_ValidFaculty_Success) {
    Faculty faculty = makeFaculty("IT", "Information Technology");
    auto result = parser.toQueryUpdateParams(faculty);

    ASSERT_TRUE(result.has_value());
    const auto& params = result.value();

    // Giả định cập nhật theo thứ tự: name, id (ví dụ)
    ASSERT_EQ(params.size(), 2u);
    EXPECT_EQ(std::any_cast<std::string>(params[0]), "Information Technology");
    EXPECT_EQ(std::any_cast<std::string>(params[1]), "IT");
}
