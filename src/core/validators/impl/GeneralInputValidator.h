#ifndef GENERALINPUTVALIDATOR_H
#define GENERALINPUTVALIDATOR_H

#include "../interface/IValidator.h" // Để kế thừa IGeneralInputValidator
#include <regex> // Cho email, phone

class GeneralInputValidator : public IGeneralInputValidator {
public:
    GeneralInputValidator() = default;
    ~GeneralInputValidator() override = default;

    ValidationResult validateEmail(const std::string& email) const override;
    ValidationResult validatePhoneNumber(const std::string& phone, const std::string& countryCode = "VN") const override;
    ValidationResult validatePasswordComplexity(const std::string& password) const override;
    ValidationResult validateIdFormat(const std::string& id, const std::string& idName, size_t minLength = 1, size_t maxLength = 20) const override;
    ValidationResult validateRequiredString(const std::string& value, const std::string& fieldName, size_t maxLength = 255) const override;
    ValidationResult validateOptionalString(const std::string& value, const std::string& fieldName, size_t maxLength = 255) const override;
    ValidationResult validateInteger(long long value, const std::string& fieldName, long long minValue, long long maxValue) const override;
    ValidationResult validateDate(int day, int month, int year, const std::string& fieldNamePrefix = "Date") const override;
    ValidationResult validateCitizenId(const std::string& citizenId, const std::string& countryCode = "VN") const override;
};

#endif // GENERALINPUTVALIDATOR_H