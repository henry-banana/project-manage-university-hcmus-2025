#include <gtest/gtest.h>
#include "../src/core/entities/SalaryRecord.h"
#include "../src/common/ValidationResult.h"
#include "../src/common/ErrorType.h"

// Test constructor và getter
TEST(SalaryRecordTest, ConstructorAndGetters) {
    SalaryRecord salary("GV001", 12000000);

    EXPECT_EQ(salary.getTeacherId(), "GV001");
    EXPECT_EQ(salary.getBasicMonthlyPay(), 12000000);
}

// Test setBasicMonthlyPay
TEST(SalaryRecordTest, SetBasicMonthlyPay) {
    SalaryRecord salary("GV001", 10000000);

    EXPECT_TRUE(salary.setBasicMonthlyPay(15000000));
    EXPECT_EQ(salary.getBasicMonthlyPay(), 15000000);

    EXPECT_FALSE(salary.setBasicMonthlyPay(-1));
    EXPECT_EQ(salary.getBasicMonthlyPay(), 15000000); // giữ nguyên
}

// Test getStringId
TEST(SalaryRecordTest, GetStringId) {
    SalaryRecord salary("GV001", 9000000);
    EXPECT_EQ(salary.getStringId(), "GV001");
}

// Test display
TEST(SalaryRecordTest, Display) {
    SalaryRecord salary("GV001", 10000000);
    std::string output = salary.display();

    EXPECT_NE(output.find("GV001"), std::string::npos);
    EXPECT_NE(output.find("10000000"), std::string::npos);
}

// Test validateBasic - hợp lệ
TEST(SalaryRecordTest, ValidateBasic_Valid) {
    SalaryRecord salary("GV001", 10000000);
    ValidationResult result = salary.validateBasic();
    EXPECT_TRUE(result.errors.empty());
}

// Test validateBasic - không hợp lệ
TEST(SalaryRecordTest, ValidateBasic_Invalid) {
    SalaryRecord salary1("", 10000000);        // thiếu ID
    SalaryRecord salary2("GV001", -5000);      // lương âm

    ValidationResult r1 = salary1.validateBasic();
    ValidationResult r2 = salary2.validateBasic();

    EXPECT_FALSE(r1.errors.empty());
    EXPECT_FALSE(r2.errors.empty());
}
