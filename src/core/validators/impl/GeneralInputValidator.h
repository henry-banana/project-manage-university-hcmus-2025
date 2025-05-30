/**
 * @file GeneralInputValidator.h
 * @brief Định nghĩa lớp kiểm tra tính hợp lệ của các giá trị đầu vào chung
 * 
 * Lớp GeneralInputValidator triển khai giao diện IGeneralInputValidator
 * để kiểm tra tính hợp lệ của các loại giá trị đầu vào phổ biến như email,
 * số điện thoại, mật khẩu, v.v.
 */
#ifndef GENERALINPUTVALIDATOR_H
#define GENERALINPUTVALIDATOR_H

#include "../interface/IValidator.h" // Để kế thừa IGeneralInputValidator
#include <regex> // Cho email, phone

/**
 * @class GeneralInputValidator
 * @brief Lớp kiểm tra tính hợp lệ của các giá trị đầu vào chung
 * 
 * Lớp này triển khai giao diện IGeneralInputValidator để kiểm tra
 * tính hợp lệ của các loại giá trị đầu vào phổ biến.
 */
class GeneralInputValidator : public IGeneralInputValidator {
public:
    /**
     * @brief Hàm khởi tạo mặc định
     */
    GeneralInputValidator() = default;
    
    /**
     * @brief Hàm hủy mặc định
     */
    ~GeneralInputValidator() override = default;

    /**
     * @brief Kiểm tra tính hợp lệ của địa chỉ email
     * @param email Địa chỉ email cần kiểm tra
     * @return Kết quả kiểm tra
     */
    ValidationResult validateEmail(const std::string& email) const override;
    
    /**
     * @brief Kiểm tra tính hợp lệ của số điện thoại
     * @param phone Số điện thoại cần kiểm tra
     * @param countryCode Mã quốc gia (mặc định là "VN")
     * @return Kết quả kiểm tra
     */
    ValidationResult validatePhoneNumber(const std::string& phone, const std::string& countryCode = "VN") const override;
    
    /**
     * @brief Kiểm tra độ phức tạp của mật khẩu
     * @param password Mật khẩu cần kiểm tra
     * @return Kết quả kiểm tra
     */
    ValidationResult validatePasswordComplexity(const std::string& password) const override;
    
    /**
     * @brief Kiểm tra định dạng của ID
     * @param id ID cần kiểm tra
     * @param idName Tên của trường ID
     * @param minLength Độ dài tối thiểu
     * @param maxLength Độ dài tối đa
     * @return Kết quả kiểm tra
     */
    ValidationResult validateIdFormat(const std::string& id, const std::string& idName, size_t minLength = 1, size_t maxLength = 20) const override;
    
    /**
     * @brief Kiểm tra chuỗi bắt buộc
     * @param value Giá trị chuỗi cần kiểm tra
     * @param fieldName Tên trường
     * @param maxLength Độ dài tối đa
     * @return Kết quả kiểm tra
     */
    ValidationResult validateRequiredString(const std::string& value, const std::string& fieldName, size_t maxLength = 255) const override;
    
    /**
     * @brief Kiểm tra chuỗi không bắt buộc
     * @param value Giá trị chuỗi cần kiểm tra
     * @param fieldName Tên trường
     * @param maxLength Độ dài tối đa
     * @return Kết quả kiểm tra
     */
    ValidationResult validateOptionalString(const std::string& value, const std::string& fieldName, size_t maxLength = 255) const override;
    
    /**
     * @brief Kiểm tra giá trị số nguyên
     * @param value Giá trị số nguyên cần kiểm tra
     * @param fieldName Tên trường
     * @param minValue Giá trị tối thiểu
     * @param maxValue Giá trị tối đa
     * @return Kết quả kiểm tra
     */
    ValidationResult validateInteger(long long value, const std::string& fieldName, long long minValue, long long maxValue) const override;
    
    /**
     * @brief Kiểm tra ngày tháng
     * @param day Ngày
     * @param month Tháng
     * @param year Năm
     * @param fieldNamePrefix Tiền tố cho tên trường
     * @return Kết quả kiểm tra
     */
    ValidationResult validateDate(int day, int month, int year, const std::string& fieldNamePrefix = "Date") const override;
    
    /**
     * @brief Kiểm tra số CMND/CCCD
     * @param citizenId Số CMND/CCCD cần kiểm tra
     * @param countryCode Mã quốc gia (mặc định là "VN")
     * @return Kết quả kiểm tra
     */
    ValidationResult validateCitizenId(const std::string& citizenId, const std::string& countryCode = "VN") const override;
};

#endif // GENERALINPUTVALIDATOR_H