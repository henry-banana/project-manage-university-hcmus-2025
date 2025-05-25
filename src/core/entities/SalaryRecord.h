/**
 * @file SalaryRecord.h
 * @brief Định nghĩa lớp SalaryRecord đại diện cho bản ghi lương của giảng viên
 * 
 * File này định nghĩa lớp SalaryRecord, triển khai giao diện IEntity, đại diện cho 
 * thông tin lương của giảng viên trong hệ thống quản lý trường đại học. Lớp này cung cấp 
 * các thuộc tính và phương thức để quản lý thông tin lương cơ bản.
 */
#ifndef SALARYRECORD_H
#define SALARYRECORD_H

#include "IEntity.h"
#include <string>

/**
 * @class SalaryRecord
 * @brief Lớp đại diện cho bản ghi lương của giảng viên
 * 
 * Lớp này triển khai giao diện IEntity và cung cấp các thuộc tính và phương thức
 * để quản lý thông tin lương cơ bản của giảng viên.
 */
class SalaryRecord : public IEntity {
private:
    std::string _teacherId; ///< ID của giảng viên
    long _basicMonthlyPay; ///< Lương cơ bản hàng tháng
    // double _bonusRate; // (Optional) Tỷ lệ thưởng
    // int _allowances;   // (Optional) Phụ cấp

public:
    /**
     * @brief Constructor tạo đối tượng SalaryRecord
     * 
     * @param teacherId ID của giảng viên
     * @param basicMonthlyPay Lương cơ bản hàng tháng
     */
    SalaryRecord(std::string teacherId, long basicMonthlyPay);

    /**
     * @brief Lấy ID của giảng viên
     * @return ID của giảng viên
     */
    const std::string& getTeacherId() const;
    
    /**
     * @brief Lấy lương cơ bản hàng tháng
     * @return Lương cơ bản hàng tháng
     */
    long getBasicMonthlyPay() const;

    /**
     * @brief Đặt lương cơ bản hàng tháng
     * @param pay Lương cơ bản mới (phải >= 0)
     * @return true nếu thành công, false nếu thất bại
     */
    bool setBasicMonthlyPay(long pay);

    /**
     * @brief Lấy ID của bản ghi lương dưới dạng chuỗi
     * @return ID của giảng viên
     */
    std::string getStringId() const override;
    
    /**
     * @brief Tạo chuỗi hiển thị thông tin lương
     * @return Chuỗi chứa thông tin lương
     */
    std::string display() const override;
    
    /**
     * @brief Kiểm tra tính hợp lệ của các thuộc tính cơ bản
     * @return Kết quả kiểm tra tính hợp lệ
     */
    ValidationResult validateBasic() const override;
};

#endif // SALARYRECORD_H