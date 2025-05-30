#include <gtest/gtest.h>
#include <expected>
#include <string>
#include <map>
#include <any>
#include <vector>

#include "../../core/parsing/impl_sql_parser/FeeRecordSqlParser.h"
#include "../../entities/FeeRecord.h"
#include "../../common/ErrorType.h"

using DbQueryResultRow = std::map<std::string, std::any>;

class FeeRecordSqlParserTest : public ::testing::Test {
protected:
    FeeRecordSqlParser parser;
};

// Tạo đối tượng FeeRecord helper
FeeRecord makeFeeRecord(const std::string& studentId, long totalFee, long paidFee = 0) {
    return FeeRecord(studentId, totalFee, paidFee);
}

// Test parse thành công với dữ liệu hợp lệ
TEST_F(FeeRecordSqlParserTest, Parse_ValidRow_Success) {
    DbQueryResultRow row = {
        {"studentId", std::string("SV001")},
        {"totalFee", int64_t(20000000)},
        {"paidFee", int64_t(5000000)}
    };

    auto result = parser.parse(row);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getStudentId(), "SV001");
    EXPECT_EQ(result->getTotalFee(), 20000000);
    EXPECT_EQ(result->getPaidFee(), 5000000);
}

// Test parse lỗi khi thiếu studentId
TEST_F(FeeRecordSqlParserTest, Parse_MissingStudentId_Error) {
    DbQueryResultRow row = {
        {"totalFee", int64_t(20000000)},
        {"paidFee", int64_t(5000000)}
    };

    auto result = parser.parse(row);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::PARSING_ERROR);
}

// Test parse lỗi khi thiếu totalFee
TEST_F(FeeRecordSqlParserTest, Parse_MissingTotalFee_Error) {
    DbQueryResultRow row = {
        {"studentId", std::string("SV001")},
        {"paidFee", int64_t(5000000)}
    };

    auto result = parser.parse(row);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::PARSING_ERROR);
}

// Test parse lỗi khi thiếu paidFee
TEST_F(FeeRecordSqlParserTest, Parse_MissingPaidFee_Error) {
    DbQueryResultRow row = {
        {"studentId", std::string("SV001")},
        {"totalFee", int64_t(20000000)}
    };

    auto result = parser.parse(row);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::PARSING_ERROR);
}

// Test serialize trả về map có đủ trường
TEST_F(FeeRecordSqlParserTest, Serialize_ValidRecord_Success) {
    FeeRecord record = makeFeeRecord("SV001", 20000000, 5000000);
    auto result = parser.serialize(record);

    ASSERT_TRUE(result.has_value());
    const auto& row = result.value();

    ASSERT_TRUE(row.find("studentId") != row.end());
    ASSERT_TRUE(row.find("totalFee") != row.end());
    ASSERT_TRUE(row.find("paidFee") != row.end());

    EXPECT_EQ(std::any_cast<std::string>(row.at("studentId")), "SV001");
    EXPECT_EQ(std::any_cast<int64_t>(row.at("totalFee")), 20000000);
    EXPECT_EQ(std::any_cast<int64_t>(row.at("paidFee")), 5000000);
}

// Test toQueryInsertParams trả về vector đúng thứ tự: studentId, totalFee, paidFee
TEST_F(FeeRecordSqlParserTest, ToQueryInsertParams_ValidRecord_Success) {
    FeeRecord record = makeFeeRecord("SV001", 20000000, 5000000);
    auto result = parser.toQueryInsertParams(record);

    ASSERT_TRUE(result.has_value());
    const auto& params = result.value();

    ASSERT_EQ(params.size(), 3u);
    EXPECT_EQ(std::any_cast<std::string>(params[0]), "SV001");
    EXPECT_EQ(std::any_cast<int64_t>(params[1]), 20000000);
    EXPECT_EQ(std::any_cast<int64_t>(params[2]), 5000000);
}

// Test toQueryUpdateParams trả về vector đúng thứ tự: totalFee, paidFee, studentId (giả định)
TEST_F(FeeRecordSqlParserTest, ToQueryUpdateParams_ValidRecord_Success) {
    FeeRecord record = makeFeeRecord("SV001", 20000000, 5000000);
    auto result = parser.toQueryUpdateParams(record);

    ASSERT_TRUE(result.has_value());
    const auto& params = result.value();

    ASSERT_EQ(params.size(), 3u);
    EXPECT_EQ(std::any_cast<int64_t>(params[0]), 20000000);
    EXPECT_EQ(std::any_cast<int64_t>(params[1]), 5000000);
    EXPECT_EQ(std::any_cast<std::string>(params[2]), "SV001");
}
