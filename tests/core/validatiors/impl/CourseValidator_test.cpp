#include "gtest/gtest.h"
#include "../src/core/validators/impl/CourseValidator.h"
#include "../src/core/entities/Course.h"
#include "../src/core/validators/interface/IValidator.h"
#include "../src/core/validators/impl/GeneralInputValidator.h"
#include <memory>
// Mock class for GeneralInputValidator (Hand-written)
class MockGeneralInputValidator : public GeneralInputValidator {
public:
    ValidationResult validateEmail(const std::string& email) const override { return {true, {}}; }
    ValidationResult validatePhoneNumber(const std::string& phone, const std::string& countryCode) const override { return {true, {}}; }
    ValidationResult validatePasswordComplexity(const std::string& password) const override { return {true, {}}; }
    ValidationResult validateIdFormat(const std::string& id, const std::string& idName, size_t minLength, size_t maxLength) const override {
        isValidIdCalled = true;
        isValidIdValue = id;
        if (isValidIdResult) {
            return {true, {}};
        } else {
            return {false, {{1, "Invalid course ID"}}};
        }
    }
    ValidationResult validateRequiredString(const std::string& value, const std::string& fieldName, size_t maxLength) const override {
        if (fieldName == "Course Name") {
            isValidNameCalled = true;
            isValidNameValue = value;
            if (isValidNameResult) {
                return {true, {}};
            } else {
                return {false, {{2, "Invalid course name"}}};
            }
        } else if (fieldName == "Course Description") {
            isValidDescriptionCalled = true;
            isValidDescriptionValue = value;
            if (isValidDescriptionResult) {
                return {true, {}};
            } else {
                return {false, {{3, "Invalid course description"}}};
            }
        }
        return {true, {}};
    }
    ValidationResult validateOptionalString(const std::string& value, const std::string& fieldName, size_t maxLength) const override { return {true, {}}; }
    ValidationResult validateInteger(long long value, const std::string& fieldName, long long minValue, long long maxValue) const override {
        isValidCreditsCalled = true;
        isValidCreditsValue = value;
        if (isValidCreditsResult) {
            return {true, {}};
        }
        else {
            return {false, {{4, "Course credits must be non-negative"}}};
        }
    }
    ValidationResult validateDate(int day, int month, int year, const std::string& fieldNamePrefix) const override { return {true, {}}; }
    ValidationResult validateCitizenId(const std::string& citizenId, const std::string& countryCode) const override { return {true, {}}; }

    // Configuration for tests
    bool isValidIdResult = true;
    bool isValidNameResult = true;
    bool isValidDescriptionResult = true;
    bool isValidCreditsResult = true;

    // Interaction verification
    mutable bool isValidIdCalled = false;
    mutable std::string isValidIdValue;
    mutable bool isValidNameCalled = false;
    mutable std::string isValidNameValue;
    mutable bool isValidDescriptionCalled = false;
    mutable std::string isValidDescriptionValue;
    mutable bool isValidCreditsCalled = false;
    mutable long long isValidCreditsValue;
};

TEST(CourseValidatorTest, Constructor) {
    // Create a mock GeneralInputValidator
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();

    // Create a CourseValidator object
    CourseValidator validator(mockGeneralValidator);

    // Verify that the CourseValidator object was created successfully
    ASSERT_NE(nullptr, &validator);
}

TEST(CourseValidatorTest, ValidateValidCourse) {
    // Create a mock GeneralInputValidator
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();
    mockGeneralValidator->isValidIdResult = true;
    mockGeneralValidator->isValidNameResult = true;
    mockGeneralValidator->isValidDescriptionResult = true;
    mockGeneralValidator->isValidCreditsResult = true;

    // Create a CourseValidator object
    CourseValidator validator(mockGeneralValidator);

    // Create a valid Course object
    Course course("123", "Test Course", 3, "This is a test course.");

    // Validate the Course object
    ValidationResult result = validator.validateEntity(course);

    // Verify that the validation result is valid
    ASSERT_TRUE(result.isValid);
    ASSERT_TRUE(result.errors.empty());
}

TEST(CourseValidatorTest, ValidateInvalidCourseId) {
    // Create a mock GeneralInputValidator
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();
    mockGeneralValidator->isValidIdResult = false;
    mockGeneralValidator->isValidNameResult = true;
    mockGeneralValidator->isValidDescriptionResult = true;
    mockGeneralValidator->isValidCreditsResult = true;

    // Create a CourseValidator object
    CourseValidator validator(mockGeneralValidator);

    // Create an invalid Course object (invalid ID)
    Course course("invalid", "Test Course", 3, "This is a test course.");

    // Validate the Course object
    ValidationResult result = validator.validateEntity(course);

    // Verify that the validation result is invalid
    ASSERT_FALSE(result.isValid);
    ASSERT_FALSE(result.errors.empty());
    ASSERT_EQ(result.errors[0].message, "Invalid course ID");
    ASSERT_TRUE(mockGeneralValidator->isValidIdCalled);
    ASSERT_EQ(mockGeneralValidator->isValidIdValue, "invalid");
}

TEST(CourseValidatorTest, ValidateInvalidCourseName) {
    // Create a mock GeneralInputValidator
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();
    mockGeneralValidator->isValidIdResult = true;
    mockGeneralValidator->isValidNameResult = false;
    mockGeneralValidator->isValidDescriptionResult = true;
    mockGeneralValidator->isValidCreditsResult = true;

    // Create a CourseValidator object
    CourseValidator validator(mockGeneralValidator);

    // Create an invalid Course object (invalid name)
    Course course("123", "", 3, "This is a test course.");

    // Validate the Course object
    ValidationResult result = validator.validateEntity(course);

    // Verify that the validation result is invalid
    ASSERT_FALSE(result.isValid);
    ASSERT_FALSE(result.errors.empty());
    ASSERT_EQ(result.errors[0].message, "Invalid course name");
    ASSERT_TRUE(mockGeneralValidator->isValidNameCalled);
    ASSERT_EQ(mockGeneralValidator->isValidNameValue, "");
}

TEST(CourseValidatorTest, ValidateInvalidCourseCredits) {
    // Create a mock GeneralInputValidator
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();
    mockGeneralValidator->isValidIdResult = true;
    mockGeneralValidator->isValidNameResult = true;
    mockGeneralValidator->isValidDescriptionResult = true;
    mockGeneralValidator->isValidCreditsResult = false;

    // Create a CourseValidator object
    CourseValidator validator(mockGeneralValidator);

    // Create an invalid Course object (invalid credits)
    Course course("123", "Test Course", -1, "This is a test course.");

    // Validate the Course object
    ValidationResult result = validator.validateEntity(course);

    // Verify that the validation result is invalid
    ASSERT_FALSE(result.isValid);
    ASSERT_FALSE(result.errors.empty());
    ASSERT_EQ(result.errors[0].message, "Course credits must be non-negative");
    ASSERT_TRUE(mockGeneralValidator->isValidCreditsCalled);
    ASSERT_EQ(mockGeneralValidator->isValidCreditsValue, -1);
}

TEST(CourseValidatorTest, ValidateInvalidCourseDescription) {
    // Create a mock GeneralInputValidator
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();
    mockGeneralValidator->isValidIdResult = true;
    mockGeneralValidator->isValidNameResult = true;
    mockGeneralValidator->isValidDescriptionResult = false;
    mockGeneralValidator->isValidCreditsResult = true;

    // Create a CourseValidator object
    CourseValidator validator(mockGeneralValidator);

    // Create an invalid Course object (invalid description)
    Course course("123", "Test Course", 3, "");

    // Validate the Course object
    ValidationResult result = validator.validateEntity(course);

    // Verify that the validation result is invalid
    ASSERT_FALSE(result.isValid);
    ASSERT_FALSE(result.errors.empty());
    ASSERT_EQ(result.errors[0].message, "Invalid course description");
    ASSERT_TRUE(mockGeneralValidator->isValidDescriptionCalled);
    ASSERT_EQ(mockGeneralValidator->isValidDescriptionValue, "");
}