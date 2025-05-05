#ifndef FINANCE_SERVICE_H
#define FINANCE_SERVICE_H

#include <memory>
#include <vector>
#include <string>
#include <optional>
#include "../entities/Student.h"
#include "../entities/Faculty.h"
#include "../entities/FeeRecord.h"
#include "../entities/SalaryRecord.h"
#include "../repo/interface/IFeeRepository.h"
#include "../repo/interface/ISalaryRepository.h"
#include "../repo/interface/IStudentRepository.h"
#include "../repo/interface/IFacultyRepository.h"
#include "../repo/interface/ITeacherRepository.h"

class IFinanceService {
public:
    virtual ~IFinanceService() = default;
    
    // --- Student Fee Operations ---
    virtual std::optional<FeeRecord> getStudentFeeRecord(const std::string& studentId) const = 0;
    // Processes a payment, returns true if successful, false otherwise.
    virtual bool makeFeePayment(const std::string& studentId, int amount) = 0;
    // Updates the total fee assigned to a student (e.g., for a new academic year)
    virtual bool setStudentTotalFee(const std::string& studentId, int newTotalFee) = 0;
    // Generates a formatted fee receipt string
    virtual std::optional<std::string> generateFeeReceipt(const std::string& studentId) const = 0;

    // --- Teacher Salary Operations ---
    virtual std::optional<SalaryRecord> getTeacherSalaryRecord(const std::string& teacherId) const = 0;
    // Updates the basic monthly pay for a teacher
    virtual bool setTeacherBasicSalary(const std::string& teacherId, int newBasicMonthlyPay) = 0;
    // Generates a formatted salary certificate string
    virtual std::optional<std::string> generateSalaryCertificate(const std::string& teacherId) const = 0;
    // Calculates total annual pay (could include bonus logic)
    virtual std::optional<int> calculateTeacherAnnualPay(const std::string& teacherId, double bonusRate = 0.9) const = 0; // Example bonus rate
};

class FinanceService : public IFinanceService {
private:
    std::shared_ptr<IFeeRepository> _feeRepo;
    std::shared_ptr<ISalaryRepository> _salaryRepo;
    std::shared_ptr<IStudentRepository> _studentRepo; // Needed for validation/receipt info
    std::shared_ptr<ITeacherRepository> _teacherRepo; // Needed for validation/certificate info

public:
    FinanceService(std::shared_ptr<IFeeRepository> feeRepo,
                    std::shared_ptr<ISalaryRepository> salaryRepo,
                    std::shared_ptr<IStudentRepository> studentRepo,
                    std::shared_ptr<ITeacherRepository> teacherRepo);

    // --- Student Fee Operations ---

    std::optional<FeeRecord> getStudentFeeRecord(const std::string& studentId) const override;

    bool makeFeePayment(const std::string& studentId, int amount) override;

    bool setStudentTotalFee(const std::string& studentId, int newTotalFee) override;

    std::optional<std::string> generateFeeReceipt(const std::string& studentId) const override;

    // --- Teacher Salary Operations ---

    std::optional<SalaryRecord> getTeacherSalaryRecord(const std::string& teacherId) const override;

    bool setTeacherBasicSalary(const std::string& teacherId, int newBasicMonthlyPay) override;

    std::optional<int> calculateTeacherAnnualPay(const std::string& teacherId, double bonusRate) const override;

    std::optional<std::string> generateSalaryCertificate(const std::string& teacherId) const override;
};

#endif // FINANCE_SERVICE_H