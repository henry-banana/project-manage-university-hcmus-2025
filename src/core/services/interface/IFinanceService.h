/**
 * @file IFinanceService.h
 * @brief Định nghĩa giao diện dịch vụ quản lý tài chính
 * 
 * File này định nghĩa giao diện IFinanceService, cung cấp các phương thức
 * để quản lý thông tin tài chính trong hệ thống quản lý trường đại học,
 * bao gồm quản lý học phí của sinh viên và lương của giảng viên.
 */
#ifndef IFINANCESERVICE_H
#define IFINANCESERVICE_H

#include <string>
#include <vector>
#include <optional>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/FeeRecord.h"
#include "../../entities/SalaryRecord.h"

/**
 * @class IFinanceService
 * @brief Giao diện dịch vụ quản lý tài chính
 * 
 * Giao diện này định nghĩa các phương thức để quản lý thông tin tài chính
 * trong hệ thống, bao gồm quản lý học phí của sinh viên và lương của giảng viên.
 */
class IFinanceService {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IFinanceService() = default;

    /**
     * @brief Lấy bản ghi học phí của sinh viên
     * 
     * @param studentId ID của sinh viên
     * @return Đối tượng FeeRecord nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<FeeRecord, Error> getStudentFeeRecord(const std::string& studentId) const = 0;
    
    /**
     * @brief Thực hiện thanh toán học phí
     * 
     * @param studentId ID của sinh viên
     * @param amount Số tiền thanh toán
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> makeFeePayment(const std::string& studentId, long amount) = 0;
    
    /**
     * @brief Đặt tổng học phí cho sinh viên
     * 
     * @param studentId ID của sinh viên
     * @param newTotalFee Tổng học phí mới
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> setStudentTotalFee(const std::string& studentId, long newTotalFee) = 0;
    
    /**
     * @brief Tạo bản ghi học phí ban đầu cho sinh viên
     * 
     * @param studentId ID của sinh viên
     * @param initialTotalFee Tổng học phí ban đầu
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> createInitialFeeRecord(const std::string& studentId, long initialTotalFee) = 0;
    
    /**
     * @brief Tạo biên lai học phí
     * 
     * @param studentId ID của sinh viên
     * @param paymentAmount Số tiền thanh toán
     * @return Chuỗi biên lai nếu thành công, Error nếu thất bại
     */
    virtual std::expected<std::string, Error> generateFeeReceipt(const std::string& studentId, long paymentAmount) const = 0;

    /**
     * @brief Lấy bản ghi lương của giảng viên
     * 
     * @param teacherId ID của giảng viên
     * @return Đối tượng SalaryRecord nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<SalaryRecord, Error> getTeacherSalaryRecord(const std::string& teacherId) const = 0;
    
    /**
     * @brief Đặt lương cơ bản cho giảng viên
     * 
     * @param teacherId ID của giảng viên
     * @param newBasicMonthlyPay Lương cơ bản hàng tháng mới
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> setTeacherBasicSalary(const std::string& teacherId, long newBasicMonthlyPay) = 0;
    
    /**
     * @brief Tạo bản ghi lương ban đầu cho giảng viên
     * 
     * @param teacherId ID của giảng viên
     * @param initialBasicPay Lương cơ bản ban đầu
     * @return true nếu thành công, Error nếu thất bại
     */
    virtual std::expected<bool, Error> createInitialSalaryRecord(const std::string& teacherId, long initialBasicPay) = 0;
    
    /**
     * @brief Tạo chứng nhận lương
     * 
     * @param teacherId ID của giảng viên
     * @return Chuỗi chứng nhận nếu thành công, Error nếu thất bại
     */
    virtual std::expected<std::string, Error> generateSalaryCertificate(const std::string& teacherId) const = 0;
};

#endif // IFINANCESERVICE_H