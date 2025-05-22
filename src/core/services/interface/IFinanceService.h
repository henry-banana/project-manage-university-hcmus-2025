#ifndef IFINANCESERVICE_H
#define IFINANCESERVICE_H

#include <string>
#include <vector>
#include <optional>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)
#include "../../entities/FeeRecord.h"
#include "../../entities/SalaryRecord.h"

class IFinanceService {
public:
    virtual ~IFinanceService() = default;

    virtual std::expected<FeeRecord, Error> getStudentFeeRecord(const std::string& studentId) const = 0;
    virtual std::expected<bool, Error> makeFeePayment(const std::string& studentId, long amount) = 0;
    virtual std::expected<bool, Error> setStudentTotalFee(const std::string& studentId, long newTotalFee) = 0;
    virtual std::expected<bool, Error> createInitialFeeRecord(const std::string& studentId, long initialTotalFee) = 0;
    virtual std::expected<std::string, Error> generateFeeReceipt(const std::string& studentId, long paymentAmount) const = 0;

    virtual std::expected<SalaryRecord, Error> getTeacherSalaryRecord(const std::string& teacherId) const = 0;
    virtual std::expected<bool, Error> setTeacherBasicSalary(const std::string& teacherId, long newBasicMonthlyPay) = 0;
    virtual std::expected<bool, Error> createInitialSalaryRecord(const std::string& teacherId, long initialBasicPay) = 0;
    virtual std::expected<std::string, Error> generateSalaryCertificate(const std::string& teacherId) const = 0;
};

#endif // IFINANCESERVICE_H