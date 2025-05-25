#ifndef FINANCESERVICE_H
#define FINANCESERVICE_H

#include "../interface/IFinanceService.h"
#include "../../data_access/interface/IFeeRecordDao.h"
#include "../../data_access/interface/ISalaryRecordDao.h"
#include "../../data_access/interface/IStudentDao.h" // Để lấy thông tin SV cho receipt
#include "../../data_access/interface/ITeacherDao.h" // Để lấy thông tin GV cho certificate
#include "../../data_access/interface/IFacultyDao.h" // Để lấy thông tin khoa cho certificate
#include "../../validators/interface/IValidator.h"
#include "../SessionContext.h"
#include <iomanip> // For setprecision in receipt/certificate

class FinanceService : public IFinanceService {
private:
    std::shared_ptr<IFeeRecordDao> _feeDao;
    std::shared_ptr<ISalaryRecordDao> _salaryDao;
    std::shared_ptr<IStudentDao> _studentDao;
    std::shared_ptr<ITeacherDao> _teacherDao;
    std::shared_ptr<IFacultyDao> _facultyDao;
    std::shared_ptr<IGeneralInputValidator> _inputValidator;
    std::shared_ptr<SessionContext> _sessionContext;

public:
    FinanceService(std::shared_ptr<IFeeRecordDao> feeDao,
                   std::shared_ptr<ISalaryRecordDao> salaryDao,
                   std::shared_ptr<IStudentDao> studentDao,
                   std::shared_ptr<ITeacherDao> teacherDao,
                   std::shared_ptr<IFacultyDao> facultyDao,
                   std::shared_ptr<IGeneralInputValidator> inputValidator,
                   std::shared_ptr<SessionContext> sessionContext);
    ~FinanceService() override = default;

    std::expected<FeeRecord, Error> getStudentFeeRecord(const std::string& studentId) const override;
    std::expected<bool, Error> makeFeePayment(const std::string& studentId, long amount) override;
    std::expected<bool, Error> setStudentTotalFee(const std::string& studentId, long newTotalFee) override;
    std::expected<bool, Error> createInitialFeeRecord(const std::string& studentId, long initialTotalFee) override;
    std::expected<std::string, Error> generateFeeReceipt(const std::string& studentId, long paymentAmount) const override;

    std::expected<SalaryRecord, Error> getTeacherSalaryRecord(const std::string& teacherId) const override;
    std::expected<bool, Error> setTeacherBasicSalary(const std::string& teacherId, long newBasicMonthlyPay) override;
    std::expected<bool, Error> createInitialSalaryRecord(const std::string& teacherId, long initialBasicPay) override;
    std::expected<std::string, Error> generateSalaryCertificate(const std::string& teacherId) const override;
};

#endif // FINANCESERVICE_H