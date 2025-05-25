/**
 * @file FeeRecord.h
 * @brief Định nghĩa lớp FeeRecord đại diện cho bản ghi học phí của sinh viên
 * 
 * File này định nghĩa lớp FeeRecord, triển khai giao diện IEntity, đại diện cho 
 * thông tin học phí của sinh viên trong hệ thống quản lý trường đại học. Lớp này cung cấp 
 * các thuộc tính và phương thức để quản lý thông tin học phí, bao gồm tổng học phí,
 * học phí đã đóng và các thao tác thanh toán.
 */
#ifndef FEERECORD_H
#define FEERECORD_H

#include "IEntity.h"
#include <string>

/**
 * @class FeeRecord
 * @brief Lớp đại diện cho bản ghi học phí của sinh viên
 * 
 * Lớp này triển khai giao diện IEntity và cung cấp các thuộc tính và phương thức
 * để quản lý thông tin học phí của sinh viên, bao gồm tổng học phí, học phí đã đóng
 * và các thao tác thanh toán.
 */
class FeeRecord : public IEntity {
private:
    std::string _studentId; ///< ID của sinh viên
    long _totalFee; ///< Tổng học phí
    long _paidFee; ///< Học phí đã đóng

public:
    /**
     * @brief Constructor tạo đối tượng FeeRecord
     * 
     * @param studentId ID của sinh viên
     * @param totalFee Tổng học phí
     * @param paidFee Học phí đã đóng (mặc định là 0)
     */
    FeeRecord(std::string studentId, long totalFee, long paidFee = 0);

    /**
     * @brief Lấy ID của sinh viên
     * @return ID của sinh viên
     */
    const std::string& getStudentId() const;
    
    /**
     * @brief Lấy tổng học phí
     * @return Tổng học phí
     */
    long getTotalFee() const;
    
    /**
     * @brief Lấy học phí đã đóng
     * @return Học phí đã đóng
     */
    long getPaidFee() const;
    
    /**
     * @brief Lấy học phí còn nợ
     * @return Học phí còn nợ
     */
    long getDueFee() const;
    
    /**
     * @brief Kiểm tra đã đóng đủ học phí chưa
     * @return true nếu đã đóng đủ, false nếu chưa
     */
    bool isFullyPaid() const;

    /**
     * @brief Đặt tổng học phí
     * @param totalFee Tổng học phí mới (phải >= 0)
     * @return true nếu thành công, false nếu thất bại
     */
    bool setTotalFee(long totalFee);
    
    /**
     * @brief Đặt học phí đã đóng
     * @param paidFee Học phí đã đóng mới (phải >= 0 và <= tổng học phí)
     * @return true nếu thành công, false nếu thất bại
     */
    bool setPaidFee(long paidFee);
    
    /**
     * @brief Thực hiện thanh toán học phí
     * @param amount Số tiền thanh toán
     * @return true nếu thanh toán hợp lệ, false nếu không
     */
    bool makePayment(long amount);

    /**
     * @brief Lấy ID của bản ghi học phí dưới dạng chuỗi
     * @return ID của sinh viên
     */
    std::string getStringId() const override;
      /**
     * @brief Tạo chuỗi hiển thị thông tin học phí
     * @return Chuỗi chứa thông tin học phí
     */
    std::string display() const override;
    
    /**
     * @brief Kiểm tra tính hợp lệ của các thuộc tính cơ bản
     * @return Kết quả kiểm tra tính hợp lệ
     */
    ValidationResult validateBasic() const override;
};

#endif // FEERECORD_H