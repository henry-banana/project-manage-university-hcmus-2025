#include "FinanceService.h"
#include "../repo/interface/IFeeRepository.h"
#include "../repo/interface/ISalaryRepository.h"
#include "../repo/interface/IStudentRepository.h" // To validate student ID
#include "../repo/interface/ITeacherRepository.h" // To validate teacher ID and get details
#include "../../utils/Logger.h"
#include <memory>
#include <sstream> // For report generation
#include <iomanip> // For report formatting
#include <cmath>   // For std::round

FinanceService::FinanceService(std::shared_ptr<IFeeRepository> feeRepo,
                std::shared_ptr<ISalaryRepository> salaryRepo,
                std::shared_ptr<IStudentRepository> studentRepo,
                std::shared_ptr<ITeacherRepository> teacherRepo)
    : _feeRepo(feeRepo), _salaryRepo(salaryRepo), _studentRepo(studentRepo), _teacherRepo(teacherRepo) {
    if (!_feeRepo || !_salaryRepo || !_studentRepo || !_teacherRepo) {
        throw std::invalid_argument("Repositories cannot be null in FinanceService.");
    }
}

// --- Student Fee Operations ---

std::optional<FeeRecord> FinanceService::getStudentFeeRecord(const std::string& studentId) const {
        // Optional: Check if student exists first using _studentRepo->exists()
    return _feeRepo->findByStudentId(studentId);
}

bool FinanceService::makeFeePayment(const std::string& studentId, int amount) {
    if (amount <= 0) {
            LOG_WARN("Payment failed: Amount must be positive for student " + studentId);
            return false;
    }
    auto feeRecordOpt = _feeRepo->findByStudentId(studentId);
    if (!feeRecordOpt) {
        LOG_WARN("Payment failed: No fee record found for student " + studentId);
        return false; // No record to update
    }

    FeeRecord feeRecord = feeRecordOpt.value();
    int pendingBefore = feeRecord.totalFee() - feeRecord.paidFee(); // Calculate pending before payment

    if (!feeRecord.makePayment(amount)) {
        // makePayment might return false for invalid amounts, though checked above.
        // Or if payment logic is complex (e.g., exceeds pending slightly).
        LOG_WARN("Payment application failed internally for student " + studentId + ", amount " + std::to_string(amount));
        return false;
    }

    // If payment was successful (even partial), update the repository
    if (_feeRepo->update(feeRecord)) {
            int actualPaid = feeRecord.paidFee(); // Calculate how much was actually paid
            LOG_INFO("Fee payment successful for student " + studentId + ". Amount Applied: " + std::to_string(actualPaid) + ". New Pending: " + std::to_string(pendingBefore));
            return true;
    } else {
            LOG_ERROR("Failed to update fee record in repository after payment for student " + studentId);
            // Consider rollback/logging critical error
            return false;
    }
}

bool FinanceService::setStudentTotalFee(const std::string& studentId, int newTotalFee) {
    if (newTotalFee < 0) {
            LOG_WARN("Set total fee failed: Amount cannot be negative for student " + studentId);
            return false;
    }
    auto feeRecordOpt = _feeRepo->findByStudentId(studentId);
    if (!feeRecordOpt) {
        // If no record exists, should we create one? Or fail? Let's fail for now.
        // Alternative: Create new record if student exists
        if (_studentRepo->exists(studentId)) {
            LOG_INFO("Creating new fee record for student " + studentId + " with total fee " + std::to_string(newTotalFee));
                FeeRecord newRecord(studentId, newTotalFee, 0);
                return _feeRepo->add(newRecord);
        } else {
                LOG_WARN("Set total fee failed: Student ID not found: " + studentId);
                return false;
        }
    }

    FeeRecord feeRecord = feeRecordOpt.value();
    feeRecord.setTotalFee(newTotalFee); // Entity handles internal adjustments (e.g., paid > new total)

    if (_feeRepo->update(feeRecord)) {
        LOG_INFO("Total fee updated successfully for student " + studentId + " to " + std::to_string(newTotalFee));
        return true;
    } else {
            LOG_ERROR("Failed to update total fee in repository for student " + studentId);
            return false;
    }
}

std::optional<std::string> FinanceService::generateFeeReceipt(const std::string& studentId) const {
        auto studentOpt = _studentRepo->findById(studentId);
        auto feeRecordOpt = _feeRepo->findByStudentId(studentId);

        if (!studentOpt) return "Error: Student not found.";
        if (!feeRecordOpt) return "Error: Fee record not found for student.";

        const auto& student = studentOpt.value();
        const auto& fee = feeRecordOpt.value();

        std::ostringstream receipt;
        // Mimic the format from the old code, but using C++ streams
        // Header (reuse from a utility function?)
        receipt << "\n\t--- CHAROTAR UNIVERSITY OF SCIENCE AND TECHNOLOGY ---" << "\n";
        receipt << "\t                   FACULTY OF ENGINEERING AND TECHNOLOGY, CHANGA" << "\n";
        receipt << "\t                         UNIVERSITY FEE RECEIPT" << "\n";
        receipt << "------------------------------------------------------------------------" << "\n";
        receipt << " Student ID:   " << std::left << std::setw(40) << student.id() << "\n";
        receipt << " Student Name: " << std::left << std::setw(40) << student.fullName() << "\n";
        receipt << "------------------------------------------------------------------------" << "\n";
        receipt << " " << std::left << std::setw(30) << "PARTICULAR" << std::right << std::setw(15) << "AMOUNT (Rs.)" << "\n";
        receipt << " -----------------------------------------------------------------------" << "\n";
        receipt << " " << std::left << std::setw(30) << "Total Fee Assigned" << std::right << std::setw(15) << fee.totalFee() << "\n";
        receipt << " " << std::left << std::setw(30) << "Total Fee Paid" << std::right << std::setw(15) << fee.paidFee() << "\n";
        receipt << " " << std::left << std::setw(30) << "Pending Fee" << std::right << std::setw(15) << fee.totalFee() - fee.paidFee() << "\n";
        receipt << "------------------------------------------------------------------------" << "\n";
        receipt << " Status: " << (fee.isFullyPaid() ? "Fully Paid" : "Payment Pending") << "\n";
        receipt << "------------------------------------------------------------------------" << "\n";


        return receipt.str();
}

// --- Teacher Salary Operations ---

std::optional<SalaryRecord> FinanceService::getTeacherSalaryRecord(const std::string& teacherId) const {
        // Optional: Check if teacher exists first using _teacherRepo->exists()
    return _salaryRepo->findByTeacherId(teacherId);
}

bool FinanceService::setTeacherBasicSalary(const std::string& teacherId, int newBasicMonthlyPay) {
    if (newBasicMonthlyPay < 0) {
            LOG_WARN("Set basic salary failed: Amount cannot be negative for teacher " + teacherId);
            return false;
    }
    auto salaryRecordOpt = _salaryRepo->findByTeacherId(teacherId);
    if (!salaryRecordOpt) {
            // If no record, create one? Or fail? Let's fail for now.
            // Alternative: Create new if teacher exists
            if (_teacherRepo->exists(teacherId)) {
                LOG_INFO("Creating new salary record for teacher " + teacherId + " with basic pay " + std::to_string(newBasicMonthlyPay));
                SalaryRecord newRecord(teacherId, newBasicMonthlyPay);
                return _salaryRepo->add(newRecord);
            } else {
                LOG_WARN("Set basic salary failed: Teacher ID not found: " + teacherId);
                return false;
            }
    }

    SalaryRecord salaryRecord = salaryRecordOpt.value();
    salaryRecord.setBasicMonthlyPay(newBasicMonthlyPay);

    if (_salaryRepo->update(salaryRecord)) {
        LOG_INFO("Basic salary updated successfully for teacher " + teacherId + " to " + std::to_string(newBasicMonthlyPay));
        return true;
    } else {
        LOG_ERROR("Failed to update basic salary in repository for teacher " + teacherId);
        return false;
    }
}

std::optional<int> FinanceService::calculateTeacherAnnualPay(const std::string& teacherId, double bonusRate) const {
    auto salaryRecordOpt = _salaryRepo->findByTeacherId(teacherId);
    if (!salaryRecordOpt) return std::nullopt;

    int basicMonthly = salaryRecordOpt.value().basicMonthlyPay();
    int annualBasic = basicMonthly * 12;
    // Simple bonus calculation (adjust logic as needed)
    int bonus = static_cast<int>(std::round(annualBasic * bonusRate));
    return annualBasic + bonus;
}

std::optional<std::string> FinanceService::generateSalaryCertificate(const std::string& teacherId) const {
        auto teacherOpt = _teacherRepo->findById(teacherId);
        auto salaryOpt = _salaryRepo->findByTeacherId(teacherId);

        if (!teacherOpt) return "Error: Teacher not found.";
        if (!salaryOpt) return "Error: Salary record not found for teacher.";

        const auto& teacher = teacherOpt.value();
        const auto& salary = salaryOpt.value();

        int basicMonthly = salary.basicMonthlyPay();
        int annualBasic = basicMonthly * 12;
        double bonusRate = 0.9; // Example, make configurable or pass as param?
        int annualBonus = static_cast<int>(std::round(annualBasic * bonusRate));
        int totalAnnual = annualBasic + annualBonus;


        std::ostringstream cert;
        // Header
        cert << "\n\t--- CHAROTAR UNIVERSITY OF SCIENCE AND TECHNOLOGY ---" << "\n";
        cert << "\t                   FACULTY OF ENGINEERING AND TECHNOLOGY, CHANGA" << "\n";
        cert << "\t                         SALARY CERTIFICATE" << "\n";
        cert << "------------------------------------------------------------------------" << "\n";
        cert << " Teacher ID:   " << std::left << std::setw(40) << teacher.id() << "\n";
        cert << " Teacher Name: " << std::left << std::setw(40) << teacher.fullName() << "\n";
        cert << " Designation:  " << std::left << std::setw(40) << teacher.designation() << "\n";
        cert << " Faculty/Dept: " << std::left << std::setw(40) << teacher.facultyId() << "\n";
        cert << "------------------------------------------------------------------------" << "\n";
        cert << " " << std::left << std::setw(30) << "PARTICULAR" << std::right << std::setw(15) << "AMOUNT (Rs.)" << "\n";
        cert << " -----------------------------------------------------------------------" << "\n";
        cert << " " << std::left << std::setw(30) << "Basic Monthly Pay" << std::right << std::setw(15) << basicMonthly << "\n";
        cert << " " << std::left << std::setw(30) << "Annual Basic Pay" << std::right << std::setw(15) << annualBasic << "\n";
        cert << " " << std::left << std::setw(30) << "Annual Bonus (Approx)" << std::right << std::setw(15) << annualBonus << "\n";
        cert << " " << std::left << std::setw(30) << "Total Annual Pay (Approx)" << std::right << std::setw(15) << totalAnnual << "\n";
        cert << "------------------------------------------------------------------------" << "\n";

        return cert.str();
}