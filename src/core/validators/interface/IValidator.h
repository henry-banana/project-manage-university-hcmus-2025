#ifndef IVALIDATOR_H
#define IVALIDATOR_H

#include "../../../common/ValidationResult.h"
#include <string>
#include <vector> // (➕) Cần cho validateDate

// Interface cho việc validate một đối tượng Entity cụ thể
template <typename TEntity>
class IEntityValidator {
public:
    virtual ~IEntityValidator() = default;
    // Validate toàn bộ entity, thường dùng trước khi add/update
    virtual ValidationResult validateEntity(const TEntity& entity) const = 0;
};

// Interface cho việc validate các giá trị đầu vào đơn lẻ
class IGeneralInputValidator {
public:
    virtual ~IGeneralInputValidator() = default;
    virtual ValidationResult validateEmail(const std::string& email) const = 0;
    virtual ValidationResult validatePhoneNumber(const std::string& phone, const std::string& countryCode = "VN") const = 0; // (➕) Thêm countryCode
    virtual ValidationResult validatePasswordComplexity(const std::string& password) const = 0;
    virtual ValidationResult validateIdFormat(const std::string& id, const std::string& idName, size_t minLength = 1, size_t maxLength = 20) const = 0;
    virtual ValidationResult validateRequiredString(const std::string& value, const std::string& fieldName, size_t maxLength = 255) const = 0;
    virtual ValidationResult validateOptionalString(const std::string& value, const std::string& fieldName, size_t maxLength = 255) const = 0;
    virtual ValidationResult validateInteger(long long value, const std::string& fieldName, long long minValue, long long maxValue) const = 0;
    virtual ValidationResult validateDate(int day, int month, int year, const std::string& fieldNamePrefix = "Date") const = 0;
    virtual ValidationResult validateCitizenId(const std::string& citizenId, const std::string& countryCode = "VN") const = 0; // (➕)
};

#endif // IVALIDATOR_H