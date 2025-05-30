#include "../src/core/parsing/impl_sql_parser/SqlParserUtils.h"
#include <gtest/gtest.h>

class SqlParserUtilsTest : public ::testing::Test {
protected:
    DbQueryResultRow row; 

    void SetUp() override {
        row.clear();
        row["id"] = 42;
        row["name"] = std::string("Alice");
        row["score"] = 95.5;
        row["null_field"] = std::any{};
        row["char_field"] = "hello"; // const char*
    }
};

TEST_F(SqlParserUtilsTest, GetOptional_ExistingKeyCorrectType) {
    EXPECT_EQ(SqlParserUtils::getOptional<int>(row, "id", -1), 42);
    EXPECT_DOUBLE_EQ(SqlParserUtils::getOptional<double>(row, "score", 0.0), 95.5);
    EXPECT_EQ(SqlParserUtils::getOptional<std::string>(row, "name", "default"), "Alice");
    EXPECT_EQ(SqlParserUtils::getOptional<std::string>(row, "char_field", "default"), "hello");
}

TEST_F(SqlParserUtilsTest, GetOptional_NonExistingKeyReturnsDefault) {
    EXPECT_EQ(SqlParserUtils::getOptional<int>(row, "nonexist", -1), -1);
    EXPECT_EQ(SqlParserUtils::getOptional<std::string>(row, "nonexist", "default"), "default");
}

TEST_F(SqlParserUtilsTest, GetOptional_WrongTypeReturnsDefault) {
    EXPECT_EQ(SqlParserUtils::getOptional<int>(row, "name", -1), -1);
    EXPECT_EQ(SqlParserUtils::getOptional<std::string>(row, "id", "default"), "default");
    EXPECT_EQ(SqlParserUtils::getOptional<int>(row, "score", -1), -1);
}

TEST_F(SqlParserUtilsTest, GetOptional_EmptyAnyReturnsDefault) {
    EXPECT_EQ(SqlParserUtils::getOptional<int>(row, "null_field", -1), -1);
    EXPECT_EQ(SqlParserUtils::getOptional<std::string>(row, "null_field", "default"), "default");
}
