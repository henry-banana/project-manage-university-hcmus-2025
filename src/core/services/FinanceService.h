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

class IFinanceService {
public:
    virtual ~IFinanceService() = default;
    
    // Fee operations
    virtual std::optional<FeeRecord> getStudentFeeRecord(const std::string& studentId) const = 0;
    virtual bool payFees(const std::string& studentId, long amount) = 0;
    virtual bool updateFeeStructure(const std::string& studentId, long totalFee) = 0;
    
    // Salary operations
    virtual std::optional<SalaryRecord> getFacultySalaryRecord(const std::string& facultyId) const = 0;
    virtual bool updateSalary(const std::string& facultyId, long salary) = 0;
    virtual bool generateSalaryCertificate(const std::string& facultyId) = 0;
};

class FinanceService : public IFinanceService {
private:
    std::shared_ptr<IFeeRepository> _feeRepo;
    std::shared_ptr<ISalaryRepository> _salaryRepo;
    std::shared_ptr<IStudentRepository> _studentRepo;
    std::shared_ptr<IFacultyRepository> _facultyRepo;
    
public:
    FinanceService(
        std::shared_ptr<IFeeRepository> feeRepo,
        std::shared_ptr<ISalaryRepository> salaryRepo,
        std::shared_ptr<IStudentRepository> studentRepo,
        std::shared_ptr<IFacultyRepository> facultyRepo
    );
    
    // Fee operations
    std::optional<FeeRecord> getStudentFeeRecord(const std::string& studentId) const override;
    bool payFees(const std::string& studentId, long amount) override;
    bool updateFeeStructure(const std::string& studentId, long totalFee) override;
    
    // Salary operations
    std::optional<SalaryRecord> getFacultySalaryRecord(const std::string& facultyId) const override;
    bool updateSalary(const std::string& facultyId, long salary) override;
    bool generateSalaryCertificate(const std::string& facultyId) override;
};

#endif // FINANCE_SERVICE_H