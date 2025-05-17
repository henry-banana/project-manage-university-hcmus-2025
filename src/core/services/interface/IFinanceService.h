#ifndef IFINANCESERVICE_H
#define IFINANCESERVICE_H

#include <string>
#include <vector>
#include <optional> // Cho getFeeRecord
#include "../../../common/OperationResult.h"
#include "../../entities/FeeRecord.h"
#include "../../entities/SalaryRecord.h"

class IFinanceService {
public:
    virtual ~IFinanceService() = default;

    // Fee Operations
    virtual OperationResult<FeeRecord> getStudentFeeRecord(const std::string& studentId) const = 0;
    virtual OperationResult<bool> makeFeePayment(const std::string& studentId, long amount) = 0;
    virtual OperationResult<bool> setStudentTotalFee(const std::string& studentId, long newTotalFee) = 0; // Admin/Finance only
    virtual OperationResult<bool> createInitialFeeRecord(const std::string& studentId, long initialTotalFee) = 0; // (➕) Khi duyệt SV mới
    virtual OperationResult<std::string> generateFeeReceipt(const std::string& studentId, long paymentAmount) const = 0; // (➕) Biên lai cho khoản thanh toán cụ thể

    // Salary Operations
    virtual OperationResult<SalaryRecord> getTeacherSalaryRecord(const std::string& teacherId) const = 0;
    virtual OperationResult<bool> setTeacherBasicSalary(const std::string& teacherId, long newBasicMonthlyPay) = 0; // Admin/Finance only
    virtual OperationResult<bool> createInitialSalaryRecord(const std::string& teacherId, long initialBasicPay) = 0; // (➕) Khi thêm GV mới
    virtual OperationResult<std::string> generateSalaryCertificate(const std::string& teacherId) const = 0;
};

#endif // IFINANCESERVICE_H