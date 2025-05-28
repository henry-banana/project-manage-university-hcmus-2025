#include <gtest/gtest.h>
#include "../src/core/entities/FeeRecord.h"
#include "../src/common/ValidationResult.h"
#include "../src/common/ErrorType.h"

// Test constructor và getter
TEST(FeeRecordTest, ConstructorAndGetters) {
    FeeRecord fee("SV001", 5000000, 1000000);

    EXPECT_EQ(fee.getStudentId(), "SV001");
    EXPECT_EQ(fee.getTotalFee(), 5000000);
    EXPECT_EQ(fee.getPaidFee(), 1000000);
    EXPECT_EQ(fee.getDueFee(), 4000000);
    EXPECT_FALSE(fee.isFullyPaid());
}

// Test setTotalFee
TEST(FeeRecordTest, SetTotalFee) {
    FeeRecord fee("SV001", 3000000, 1000000);

    EXPECT_TRUE(fee.setTotalFee(4000000));
    EXPECT_EQ(fee.getTotalFee(), 4000000);

    EXPECT_FALSE(fee.setTotalFee(-1));
    EXPECT_EQ(fee.getTotalFee(), 4000000);
}

// Test setPaidFee
TEST(FeeRecordTest, SetPaidFee) {
    FeeRecord fee("SV001", 4000000, 1000000);

    EXPECT_TRUE(fee.setPaidFee(2000000));
    EXPECT_EQ(fee.getPaidFee(), 2000000);

    EXPECT_FALSE(fee.setPaidFee(-1));               // âm
    EXPECT_FALSE(fee.setPaidFee(5000000));          // lớn hơn tổng
    EXPECT_EQ(fee.getPaidFee(), 2000000);           // giữ nguyên
}

// Test makePayment
TEST(FeeRecordTest, MakePayment) {
    FeeRecord fee("SV001", 3000000, 1000000);

    EXPECT_TRUE(fee.makePayment(1000000));
    EXPECT_EQ(fee.getPaidFee(), 2000000);

    EXPECT_FALSE(fee.makePayment(-1));              // không hợp lệ
    EXPECT_FALSE(fee.makePayment(2000000));         // vượt quá tổng

    EXPECT_TRUE(fee.makePayment(1000000));          // đủ tiền
    EXPECT_TRUE(fee.isFullyPaid());
}

// Test getStringId
TEST(FeeRecordTest, GetStringId) {
    FeeRecord fee("SV001", 3000000);
    EXPECT_EQ(fee.getStringId(), "SV001");
}

// Test display
TEST(FeeRecordTest, Display) {
    FeeRecord fee("SV001", 3000000, 1000000);
    std::string output = fee.display();

    EXPECT_NE(output.find("SV001"), std::string::npos);
    EXPECT_NE(output.find("3000000"), std::string::npos);
    EXPECT_NE(output.find("1000000"), std::string::npos);
}

// Test validateBasic - hợp lệ
TEST(FeeRecordTest, ValidateBasic_Valid) {
    FeeRecord fee("SV001", 3000000, 2000000);
    ValidationResult result = fee.validateBasic();
    EXPECT_TRUE(result.errors.empty());
}

// Test validateBasic - không hợp lệ
TEST(FeeRecordTest, ValidateBasic_Invalid) {
    FeeRecord fee1("", 3000000, 1000000);             // thiếu ID
    FeeRecord fee2("SV001", -1000, 0);                // totalFee < 0
    FeeRecord fee3("SV001", 3000000, -1000);          // paidFee < 0
    FeeRecord fee4("SV001", 3000000, 4000000);        // paidFee > totalFee

    EXPECT_FALSE(fee1.validateBasic().errors.empty());
    EXPECT_FALSE(fee2.validateBasic().errors.empty());
    EXPECT_FALSE(fee3.validateBasic().errors.empty());
    EXPECT_FALSE(fee4.validateBasic().errors.empty());
}
