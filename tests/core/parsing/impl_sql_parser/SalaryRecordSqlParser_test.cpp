#include <gtest/gtest.h>
#include <expected>
#include <string>
#include <map>
#include <any>
#include <vector>

#include "../../core/parsing/impl_sql_parser/SalaryRecordSqlParser.h"
#include "../../entities/SalaryRecord.h"
#include "../../common/ErrorType.h"  // chứa Error, ErrorCode
#include "../../core/parsing/impl_sql_parser/SqlParserUtils.h"

using DbQueryResultRow = std::map<std::string, std::any>;

class SalaryRecordSqlParserTest : public ::testing::Test {
protected:
    SalaryRecordSqlParser parser;
};

// Giúp tạo đối tượng SalaryRecord
SalaryRecord makeSalaryRecord(const std::string& teacherId, long basicMonthlyPay) {
    return SalaryRecord(teacherId, basicMonthlyPay);
}

// Test parse thành công với dữ liệu hợp lệ
TEST_F(SalaryRecordSqlParserTest, Parse_ValidRow_Success) {
    DbQueryResultRow row = {
        {"teacherId", std::string("GV001")},
        {"basicMonthlyPay", int64_t(15000000)}
    };

    auto result = parser.parse(row);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTeacherId(), "GV001");
    EXPECT_EQ(result->getBasicMonthlyPay(), 15000000);
}

// Test parse lỗi khi thiếu teacherId
TEST_F(SalaryRecordSqlParserTest, Parse_MissingTeacherId_Error) {
    DbQueryResultRow row = {
        {"basicMonthlyPay", int64_t(15000000)}
    };

    auto result = parser.parse(row);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::PARSING_ERROR);
}

// Test parse lỗi khi thiếu basicMonthlyPay
TEST_F(SalaryRecordSqlParserTest, Parse_MissingBasicMonthlyPay_Error) {
    DbQueryResultRow row = {
        {"teacherId", std::string("GV001")}
    };

    auto result = parser.parse(row);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().code, ErrorCode::PARSING_ERROR);
}

// Test serialize trả về map có đúng teacherId và basicMonthlyPay
TEST_F(SalaryRecordSqlParserTest, Serialize_ValidRecord_Success) {
    SalaryRecord record = makeSalaryRecord("GV001", 15000000);
    auto result = parser.serialize(record);

    ASSERT_TRUE(result.has_value());
    const auto& row = result.value();
    ASSERT_TRUE(row.find("teacherId") != row.end());
    ASSERT_TRUE(row.find("basicMonthlyPay") != row.end());

    EXPECT_EQ(std::any_cast<std::string>(row.at("teacherId")), "GV001");
    EXPECT_EQ(std::any_cast<long>(row.at("basicMonthlyPay")), 15000000);
}

// Test toQueryInsertParams trả về vector đúng teacherId, basicMonthlyPay
TEST_F(SalaryRecordSqlParserTest, ToQueryInsertParams_ValidRecord_Success) {
    SalaryRecord record = makeSalaryRecord("GV001", 15000000);
    auto result = parser.toQueryInsertParams(record);

    ASSERT_TRUE(result.has_value());
    const auto& params = result.value();

    ASSERT_EQ(params.size(), 2u);
    EXPECT_EQ(std::any_cast<std::string>(params[0]), "GV001");
    EXPECT_EQ(std::any_cast<int64_t>(params[1]), 15000000);
}

// Test toQueryUpdateParams trả về vector đúng basicMonthlyPay, teacherId (giả định)
TEST_F(SalaryRecordSqlParserTest, ToQueryUpdateParams_ValidRecord_Success) {
    SalaryRecord record = makeSalaryRecord("GV001", 15000000);
    auto result = parser.toQueryUpdateParams(record);

    ASSERT_TRUE(result.has_value());
    const auto& params = result.value();

    ASSERT_EQ(params.size(), 2u);
    // Giả định update theo thứ tự: basicMonthlyPay, teacherId
    EXPECT_EQ(std::any_cast<int64_t>(params[0]), 15000000);
    EXPECT_EQ(std::any_cast<std::string>(params[1]), "GV001");
}
