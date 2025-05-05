#ifndef SALARY_RECORD_H
#define SALARY_RECORD_H

#include <string>

class SalaryRecord {
private:
    std::string _teacherId;
    int _basicMonthlyPay;
    // double bonusRate_; // Có thể lưu tỉ lệ bonus ở đây

protected:

public:
    SalaryRecord();
    SalaryRecord(std::string facultyId, int basicMonthlyPay);
    SalaryRecord(const SalaryRecord& other);
    SalaryRecord& operator=(const SalaryRecord& other);
    ~SalaryRecord() = default; 

    // Getter & Setter
    const std::string& teacherId() const;
    int basicMonthlyPay() const;
    
    void setTeacherId(const std::string& teacherId);
    void setBasicMonthlyPay(int newPay);
    // Tính toán lương (có thể đặt ở FinanceService)

    void display() const; // Hiển thị thông tin lương

    int calculateAnnualBasicPay() const;
    int calculateAnnualBonus(double bonusRate = 0.9) const; // Ví dụ bonus rate
    int calculateTotalAnnualPay(double bonusRate = 0.9) const;
};

#endif // SALARY_RECORD_H