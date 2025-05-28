#include "gtest/gtest.h"
#include "../src/core/validators/impl/TeacherValidator.h"
#include "../src/core/entities/Teacher.h"
#include "../src/core/validators/interface/IValidator.h"
#include "../src/core/validators/impl/GeneralInputValidator.h"
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
    ValidationResult validateInteger(long long value, const std::string& fieldName, long long minValue, long long maxValue) const override { return isValidInteger ? ValidationResult() : ValidationResult(false, {{5, "Invalid integer"}}); }
    ValidationResult validateDate(int day, int month, int year, const std::string& fieldNamePrefix) const override { return ValidationResult(); }
    ValidationResult validateCitizenId(const std::string& citizenId, const std::string& countryCode) const override { return ValidationResult(); }

    bool isValidEmail = true;
    bool isValidPhoneNumber = true;
    bool isValidIdFormat = true;
    bool isValidRequiredString = true;
    bool isValidInteger = true;
};

TEST(TeacherValidatorTest, ConstructorThrowsOnNullGeneralValidator) {
    ASSERT_THROW(TeacherValidator(nullptr), std::invalid_argument);
}

TEST(TeacherValidatorTest, ValidateValidTeacher) {
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();
    TeacherValidator validator(mockGeneralValidator);
    Teacher teacher("123", "John", "Doe", "FIT");
    ValidationResult result = validator.validateEntity(teacher);
    ASSERT_TRUE(result.isValid);
}

TEST(TeacherValidatorTest, ValidateInvalidTeacherId) {
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();
    mockGeneralValidator->isValidIdFormat = false;
    TeacherValidator validator(mockGeneralValidator);
    Teacher teacher("invalid", "John", "Doe", "FIT");
    ValidationResult result = validator.validateEntity(teacher);
    ASSERT_FALSE(result.isValid);
}

TEST(TeacherValidatorTest, ValidateInvalidFirstName) {
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();
    mockGeneralValidator->isValidRequiredString = false;
    TeacherValidator validator(mockGeneralValidator);
    Teacher teacher("123", "", "Doe", "FIT");
    ValidationResult result = validator.validateEntity(teacher);
    ASSERT_FALSE(result.isValid);
}

TEST(TeacherValidatorTest, ValidateInvalidLastName) {
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();
    mockGeneralValidator->isValidRequiredString = false;
    TeacherValidator validator(mockGeneralValidator);
    Teacher teacher("123", "John", "", "FIT");
    ValidationResult result = validator.validateEntity(teacher);
    ASSERT_FALSE(result.isValid);
}

TEST(TeacherValidatorTest, ValidateInvalidFacultyId) {
    auto mockGeneralValidator = std::make_shared<MockGeneralInputValidator>();
    mockGeneralValidator->isValidRequiredString = false;
    TeacherValidator validator(mockGeneralValidator);
    Teacher teacher("123", "John", "Doe", "");
    ValidationResult result = validator.validateEntity(teacher);
    ASSERT_FALSE(result.isValid);
}