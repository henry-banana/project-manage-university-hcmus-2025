/**
 * @file IValidator.h
 * @brief Định nghĩa các giao diện kiểm tra tính hợp lệ của dữ liệu
 * 
 * File này định nghĩa các giao diện để kiểm tra tính hợp lệ của dữ liệu
 * bao gồm IEntityValidator cho các đối tượng Entity và IGeneralInputValidator
 * cho các giá trị đầu vào đơn lẻ.
 */
#ifndef IVALIDATOR_H
#define IVALIDATOR_H

#include "../../../common/ValidationResult.h"
#include <string>
#include <vector> // (➕) Cần cho validateDate

/**
 * @class IEntityValidator
 * @brief Giao diện kiểm tra tính hợp lệ của một loại đối tượng Entity
 * 
 * @tparam TEntity Kiểu dữ liệu của đối tượng cần kiểm tra
 */
template <typename TEntity>
class IEntityValidator {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IEntityValidator() = default;
    
    /**
     * @brief Kiểm tra tính hợp lệ của toàn bộ đối tượng Entity
     * 
     * @param entity Đối tượng cần kiểm tra
     * @return Kết quả kiểm tra
     */
    virtual ValidationResult validateEntity(const TEntity& entity) const = 0;
};

/**
 * @class IGeneralInputValidator
 * @brief Giao diện kiểm tra tính hợp lệ của các giá trị đầu vào đơn lẻ
 */
class IGeneralInputValidator {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IGeneralInputValidator() = default;
    
    /**
     * @brief Kiểm tra tính hợp lệ của địa chỉ email
     * 
     * @param email Địa chỉ email cần kiểm tra
     * @return Kết quả kiểm tra
     */
    virtual ValidationResult validateEmail(const std::string& email) const = 0;
    
    /**
     * @brief Kiểm tra tính hợp lệ của số điện thoại
     * 
     * @param phone Số điện thoại cần kiểm tra
     * @param countryCode Mã quốc gia (mặc định là "VN")
     * @return Kết quả kiểm tra
     */
    virtual ValidationResult validatePhoneNumber(const std::string& phone, const std::string& countryCode = "VN") const = 0; // (➕) Thêm countryCode
    
    /**
     * @brief Kiểm tra độ phức tạp của mật khẩu
     * 
     * @param password Mật khẩu cần kiểm tra
     * @return Kết quả kiểm tra
     */
    virtual ValidationResult validatePasswordComplexity(const std::string& password) const = 0;
    
    /**
     * @brief Kiểm tra định dạng của ID
     * 
     * @param id ID cần kiểm tra
     * @param idName Tên của trường ID
     * @param minLength Độ dài tối thiểu
     * @param maxLength Độ dài tối đa
     * @return Kết quả kiểm tra
     */
    virtual ValidationResult validateIdFormat(const std::string& id, const std::string& idName, size_t minLength = 1, size_t maxLength = 20) const = 0;
    
    /**
     * @brief Kiểm tra chuỗi bắt buộc
     * 
     * @param value Giá trị chuỗi cần kiểm tra
     * @param fieldName Tên trường
     * @param maxLength Độ dài tối đa
     * @return Kết quả kiểm tra
     */
    virtual ValidationResult validateRequiredString(const std::string& value, const std::string& fieldName, size_t maxLength = 255) const = 0;
    
    /**
     * @brief Kiểm tra chuỗi không bắt buộc
     * 
     * @param value Giá trị chuỗi cần kiểm tra
     * @param fieldName Tên trường
     * @param maxLength Độ dài tối đa
     * @return Kết quả kiểm tra
     */
    virtual ValidationResult validateOptionalString(const std::string& value, const std::string& fieldName, size_t maxLength = 255) const = 0;
    
    /**
     * @brief Kiểm tra giá trị số nguyên
     * 
     * @param value Giá trị số nguyên cần kiểm tra
     * @param fieldName Tên trường
     * @param minValue Giá trị tối thiểu
     * @param maxValue Giá trị tối đa
     * @return Kết quả kiểm tra
     */
    virtual ValidationResult validateInteger(long long value, const std::string& fieldName, long long minValue, long long maxValue) const = 0;
    
    /**
     * @brief Kiểm tra ngày tháng
     * 
     * @param day Ngày
     * @param month Tháng
     * @param year Năm
     * @param fieldNamePrefix Tiền tố cho tên trường
     * @return Kết quả kiểm tra
     */    virtual ValidationResult validateDate(int day, int month, int year, const std::string& fieldNamePrefix = "Date") const = 0;
    
    /**
     * @brief Kiểm tra số CMND/CCCD
     * 
     * @param citizenId Số CMND/CCCD cần kiểm tra
     * @param countryCode Mã quốc gia (mặc định là "VN")
     * @return Kết quả kiểm tra
     */
    virtual ValidationResult validateCitizenId(const std::string& citizenId, const std::string& countryCode = "VN") const = 0; // (➕)
};

#endif // IVALIDATOR_H