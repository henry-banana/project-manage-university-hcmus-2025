#include "gtest/gtest.h"
#include "../src/common/ValidationResult.h"

TEST(ValidationResultTest, DefaultConstructor) {
    ValidationResult result;
    ASSERT_TRUE(result.isValid);
    ASSERT_TRUE(result.errors.empty());
}

TEST(ValidationResultTest, AddErrorIntString) {
    ValidationResult result;
    result.addError(400, "Bad Request");
    ASSERT_FALSE(result.isValid);
    ASSERT_EQ(result.errors.size(), 1);
    ASSERT_EQ(result.errors[0].code, 400);
    ASSERT_EQ(result.errors[0].message, "Bad Request");
}

TEST(ValidationResultTest, AddErrorObject) {
    ValidationResult result;
    Error error(500, "Internal Server Error");
    result.addError(error);
    ASSERT_FALSE(result.isValid);
    ASSERT_EQ(result.errors.size(), 1);
    ASSERT_EQ(result.errors[0].code, 500);
    ASSERT_EQ(result.errors[0].message, "Internal Server Error");
}

TEST(ValidationResultTest, AddMultipleErrors) {
    ValidationResult result;
    result.addError(400, "Bad Request");
    result.addError(Error(500, "Internal Server Error"));
    ASSERT_FALSE(result.isValid);
    ASSERT_EQ(result.errors.size(), 2);
    ASSERT_EQ(result.errors[0].code, 400);
    ASSERT_EQ(result.errors[0].message, "Bad Request");
    ASSERT_EQ(result.errors[1].code, 500);
    ASSERT_EQ(result.errors[1].message, "Internal Server Error");
}

TEST(ValidationResultTest, GetErrorMessagesCombinedDefaultSeparator) {
    ValidationResult result;
    result.addError(400, "Bad Request");
    result.addError(Error(500, "Internal Server Error"));
    std::string combinedMessage = result.getErrorMessagesCombined();
    ASSERT_EQ(combinedMessage, "Bad Request\nInternal Server Error");
}

TEST(ValidationResultTest, GetErrorMessagesCombinedCustomSeparator) {
    ValidationResult result;
    result.addError(400, "Bad Request");
    result.addError(Error(500, "Internal Server Error"));
    std::string combinedMessage = result.getErrorMessagesCombined(", ");
    ASSERT_EQ(combinedMessage, "Bad Request, Internal Server Error");
}

TEST(ValidationResultTest, GetErrorMessagesCombinedNoErrors) {
    ValidationResult result;
    std::string combinedMessage = result.getErrorMessagesCombined();
    ASSERT_EQ(combinedMessage, "");
}

TEST(ValidationResultTest, IsValidRemainsFalseAfterAddingErrors) {
    ValidationResult result;
    result.addError(400, "Bad Request");
    ASSERT_FALSE(result.isValid);
    result.addError(500, "Internal Server Error");
    ASSERT_FALSE(result.isValid);
}