#include "gtest/gtest.h"
#include "../src/core/validators/impl/StudentValidator.h"
#include "../src/core/entities/Student.h"
#include "../src/core/validators/interface/IValidator.h"
#include "../src/core/validators/impl/GeneralInputValidator.h"
#include "../src/core/entities/Birthday.h" // Include Date.h
#include <memory>

// Mock class for IGeneralInputValidator
class MockGeneralInputValidator : public IGeneralInputValidator {
public:
    ValidationResult validateEmail(const std::string& email) const override { return isValidEmail ? ValidationResult() : ValidationResult(false, {{1, "Invalid email"}}); }
    ValidationResult validatePhoneNumber(const std::string& phone, const std::string& countryCode) const override { return isValidPhoneNumber ? ValidationResult() : ValidationResult(false, {{2, "Invalid phone number"}}); }
    ValidationResult validatePasswordComplexity(const std::string& password) const override { return ValidationResult(); }
    ValidationResult validateIdFormat(const std::string& id, const std::string& idName, size_t minLength, size_t maxLength) const override { return isValidIdFormat ? ValidationResult() : ValidationResult(false, {{3, "Invalid ID format"}}); }
    ValidationResult validateRequiredString(const std::string& value, const std::string& fieldName, size_t maxLength) const override { return isValidRequiredString ? ValidationResult() : ValidationResult(false, {{4, "Invalid required string"}}); }
    ValidationResult validateOptionalString(const std::string& value, const std::string& fieldName, size_t maxLength) const override { return ValidationResult(); }
    ValidationResult validateInteger(long long value, const std::string& fieldName, long long minValue, long long maxValue) const override { return ValidationResult(); }
    ValidationResult validateDate(int day, int month, int year, const std::string& fieldNamePrefix) const override { return isValidDate ? ValidationResult() : ValidationResult(false, {{5, "Invalid date"}}); }
    ValidationResult validateCitizenId(const std::string& citizenId, const std::string& countryCode) const override { return isValidCitizenId ? ValidationResult() : ValidationResult(false, {{6, "Invalid citizen ID"}}); }

    bool isValidEmail = true;
    bool isValidPhoneNumber = true;
    bool isValidIdFormat = true;
    bool isValidRequiredString = true;
    bool isValidDate = true;
    bool isValidCitizenId = true;
};

TEST(StudentValidatorTest, ConstructorThrowsOnNullGeneralValidator) {
    ASSERT_THROW(StudentValidator(nullptr), std::invalid_argument);
}

TEST(StudentValidatorTest, ValidateValidStudent) {
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();
    StudentValidator validator(mockGeneralValidator);
    Student student("123", "John", "Doe", "FIT");
    ValidationResult result = validator.validateEntity(student);
    ASSERT_TRUE(result.isValid);
}

TEST(StudentValidatorTest, ValidateInvalidStudentId) {
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();
    mockGeneralValidator->isValidIdFormat = false;
    StudentValidator validator(mockGeneralValidator);
    Student student("invalid", "John", "Doe", "FIT");
    ValidationResult result = validator.validateEntity(student);
    ASSERT_FALSE(result.isValid);
}

TEST(StudentValidatorTest, ValidateInvalidFirstName) {
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();
    mockGeneralValidator->isValidRequiredString = false;
    StudentValidator validator(mockGeneralValidator);
    Student student("123", "", "Doe", "FIT");
    ValidationResult result = validator.validateEntity(student);
    ASSERT_FALSE(result.isValid);
}

TEST(StudentValidatorTest, ValidateInvalidLastName) {
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();
    mockGeneralValidator->isValidRequiredString = false;
    StudentValidator validator(mockGeneralValidator);
    Student student("123", "John", "", "FIT");
    ValidationResult result = validator.validateEntity(student);
    ASSERT_FALSE(result.isValid);
}

TEST(StudentValidatorTest, ValidateInvalidFacultyId) {
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();
    mockGeneralValidator->isValidRequiredString = false;
    StudentValidator validator(mockGeneralValidator);
    Student student("123", "John", "Doe", "");
    ValidationResult result = validator.validateEntity(student);
    ASSERT_FALSE(result.isValid);
}