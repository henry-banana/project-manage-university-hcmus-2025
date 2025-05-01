#ifndef SALARYRECORD_H
#define SALARYRECORD_H

#include <string>

class SalaryRecord {
private:
    std::string _facultyId;
    long _basicMonthlyPay;
    // double bonusRate_; // Có thể lưu tỉ lệ bonus ở đây

protected:

public:
    // Có thể thêm các yếu tố khác như bonus rate, deductions,...
    SalaryRecord(std::string facultyId, long basicMonthlyPay);

    const std::string& getFacultyId() const { return _facultyId; }
    long getBasicMonthlyPay() const { return _basicMonthlyPay; }

    // Tính toán lương (có thể đặt ở FinanceService)
    long calculateAnnualBasicPay() const;
    long calculateAnnualBonus(double bonusRate = 0.9) const; // Ví dụ bonus rate
    long calculateTotalAnnualPay(double bonusRate = 0.9) const;

    // Setter nếu lương cơ bản có thể thay đổi
    void setBasicMonthlyPay(long newPay);
};

#endif // SALARYRECORD_H