#ifndef IVALIDATOR_H
#define IVALIDATOR_H

#include "../../../common/ValidationResult.h"
#include <string> // Ví dụ cho validate một chuỗi, hoặc một field cụ thể

// Validator cho một đối tượng Entity cụ thể
template <typename TEntity>
class IEntityValidator {
public:
    virtual ~IEntityValidator() = default;
    // Validate toàn bộ entity, thường dùng trước khi add/update
    virtual ValidationResult validateEntity(const TEntity& entity) const = 0;
};

// Validator cho một giá trị đầu vào đơn lẻ (ví dụ, khi người dùng nhập)
class IInputValidator {
public:
    virtual ~IInputValidator() = default;
    virtual ValidationResult validateEmail(const std::string& email) const = 0;
    virtual ValidationResult validatePhoneNumber(const std::string& phone) const = 0;
    virtual ValidationResult validatePasswordComplexity(const std::string& password) const = 0;
    virtual ValidationResult validateIdFormat(const std::string& id, const std::string& entityName) const = 0; // e.g., studentId
    // ...
};

#endif