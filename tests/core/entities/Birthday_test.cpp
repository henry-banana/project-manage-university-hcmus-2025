#include <gtest/gtest.h>
#include "../../src/core/entities/Birthday.h"

// Helper: kiểm tra chuỗi ngày đúng định dạng dd/mm/yyyy
bool isValidDateFormat(const std::string& str, char delim = '/') {
    return str.length() == 10 && str[2] == delim && str[5] == delim;
}

// ----------- CONSTRUCTORS -----------

TEST(BirthdayTest, DefaultConstructor_IsNotSet) {
    Birthday b;
    EXPECT_FALSE(b.isSet());
}

TEST(BirthdayTest, ParameterizedConstructor_SetsCorrectly) {
    Birthday b(10, 5, 2000);
    EXPECT_TRUE(b.isSet());
    EXPECT_EQ(b.getDay(), 10);
    EXPECT_EQ(b.getMonth(), 5);
    EXPECT_EQ(b.getYear(), 2000);
}

// ----------- SET & GET -----------

TEST(BirthdayTest, Set_ValidDate_ReturnsTrue) {
    Birthday b;
    EXPECT_TRUE(b.set(29, 2, 2020)); // năm nhuận
    EXPECT_TRUE(b.isSet());
    EXPECT_EQ(b.getDay(), 29);
    EXPECT_EQ(b.getMonth(), 2);
    EXPECT_EQ(b.getYear(), 2020);
}

TEST(BirthdayTest, Set_InvalidDate_ReturnsFalse) {
    Birthday b;
    EXPECT_FALSE(b.set(31, 2, 2023)); // Tháng 2 không có 31 ngày
    EXPECT_FALSE(b.isSet());
}

// ----------- setFromString -----------

TEST(BirthdayTest, SetFromString_ValidFormat_ReturnsTrue) {
    Birthday b;
    EXPECT_TRUE(b.setFromString("15/08/1999"));
    EXPECT_TRUE(b.isSet());
    EXPECT_EQ(b.getDay(), 15);
    EXPECT_EQ(b.getMonth(), 8);
    EXPECT_EQ(b.getYear(), 1999);
}

TEST(BirthdayTest, SetFromString_InvalidFormat_ReturnsFalse) {
    Birthday b;
    EXPECT_FALSE(b.setFromString("1999/08/15"));  // sai định dạng
    EXPECT_FALSE(b.setFromString("15-08-1999", '-'));  // đúng format nhưng sai delimiter
    EXPECT_FALSE(b.setFromString("15/08")); // thiếu năm
    EXPECT_FALSE(b.isSet());
}

TEST(BirthdayTest, SetFromString_InvalidDate_ReturnsFalse) {
    Birthday b;
    EXPECT_FALSE(b.setFromString("31/02/2021"));  // Ngày không hợp lệ
    EXPECT_FALSE(b.isSet());
}

// ----------- toString -----------

TEST(BirthdayTest, ToString_ddmmyyyy_ValidDate) {
    Birthday b(3, 4, 2021);
    std::string str = b.toString_ddmmyyyy();
    EXPECT_EQ(str, "03/04/2021");
    EXPECT_TRUE(isValidDateFormat(str));
}

TEST(BirthdayTest, ToString_ddmmyyyy_CustomDelimiter) {
    Birthday b(9, 12, 2022);
    std::string str = b.toString_ddmmyyyy('-');
    EXPECT_EQ(str, "09-12-2022");
}

// ----------- validate -----------

TEST(BirthdayTest, Validate_ValidBirthday_ReturnsValid) {
    Birthday b(1, 1, 2000);
    ValidationResult result = b.validate();
    EXPECT_TRUE(result.isValid) << result.getErrorMessagesCombined();
}

TEST(BirthdayTest, Validate_InvalidDay_ReturnsInvalid) {
    Birthday b;
    b.set(32, 1, 2000); // manually set invalid date
    ValidationResult result = b.validate();
    EXPECT_FALSE(result.isValid);
    EXPECT_GE(result.errors.size(), 1);
    EXPECT_NE(result.getErrorMessagesCombined().find("day"), std::string::npos);
}

TEST(BirthdayTest, Validate_InvalidMonth_ReturnsInvalid) {
    Birthday b;
    b.set(10, 13, 2000); // month > 12
    ValidationResult result = b.validate();
    EXPECT_FALSE(result.isValid);
    EXPECT_GE(result.errors.size(), 1);
    EXPECT_NE(result.getErrorMessagesCombined().find("month"), std::string::npos);
}

TEST(BirthdayTest, Validate_InvalidYear_ReturnsInvalid) {
    Birthday b;
    b.set(10, 5, 1800); // giả sử hệ thống chỉ chấp nhận năm > 1900
    ValidationResult result = b.validate();
    EXPECT_FALSE(result.isValid);
    EXPECT_GE(result.errors.size(), 1);
    EXPECT_NE(result.getErrorMessagesCombined().find("year"), std::string::npos);
}

