#include <gtest/gtest.h>
#include <expected>
#include <string>
#include <vector>
#include <map>
#include <any>

#include "EnrollmentRecordSqlParser.h"
#include "../../data_access/interface/IEnrollmentDao.h" // Để dùng EnrollmentRecord
#include "../../common/ErrorType.h" // Để dùng Error, ErrorCode
#include "SqlParserUtils.h" // Nếu cần mock hoặc dùng trực tiếp

// Giả sử DbQueryResultRow được typedef như này:
using DbQueryResultRow = std::map<std::string, std::any>;

class EnrollmentRecordSqlParserTest : public ::testing::Test {
protected:
    EnrollmentRecordSqlParser parser;
};

// Test parse thành công với dữ liệu hợp lệ
TEST_F(EnrollmentRecordSqlParserTest, Parse_ValidRow_Success) {
    DbQueryResultRow row = {
        {"studentId", std::string("S123")},
        {"courseId", std::string("C456")}
    };

    auto result = parser.parse(row);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->studentId, "S123");
    EXPECT_EQ(result->courseId, "C456");
}

// Test parse lỗi khi thiếu studentId
TEST_F(EnrollmentRecordSqlParserTest, Parse_MissingStudentId_Error) {
    DbQueryResultRow row = {
        {"courseId", std::string("C456")}
    };

    auto result = parser.parse(row);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::PARSING_ERROR);
}

// Test parse lỗi khi thiếu courseId
TEST_F(EnrollmentRecordSqlParserTest, Parse_MissingCourseId_Error) {
    DbQueryResultRow row = {
        {"studentId", std::string("S123")}
    };

    auto result = parser.parse(row);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::PARSING_ERROR);
}

// Test serialize trả về map đúng giá trị
TEST_F(EnrollmentRecordSqlParserTest, Serialize_ValidRecord_Success) {
    EnrollmentRecord record{"S123", "C456"};
    auto result = parser.serialize(record);

    ASSERT_TRUE(result.has_value());
    const auto& row = result.value();

    ASSERT_TRUE(row.find("studentId") != row.end());
    ASSERT_TRUE(row.find("courseId") != row.end());
    
    EXPECT_EQ(std::any_cast<std::string>(row.at("studentId")), "S123");
    EXPECT_EQ(std::any_cast<std::string>(row.at("courseId")), "C456");
}

// Test toQueryInsertParams trả về vector any chứa đúng giá trị
TEST_F(EnrollmentRecordSqlParserTest, ToQueryInsertParams_ValidRecord_Success) {
    EnrollmentRecord record{"S123", "C456"};
    auto result = parser.toQueryInsertParams(record);

    ASSERT_TRUE(result.has_value());

    const auto& params = result.value();
    ASSERT_EQ(params.size(), 2u);

    EXPECT_EQ(std::any_cast<std::string>(params[0]), "S123");
    EXPECT_EQ(std::any_cast<std::string>(params[1]), "C456");
}

// Test toQueryUpdateParams luôn trả về lỗi
TEST_F(EnrollmentRecordSqlParserTest, ToQueryUpdateParams_AlwaysFails) {
    EnrollmentRecord record{"S123", "C456"};
    auto result = parser.toQueryUpdateParams(record);

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::OPERATION_FAILED);
}
