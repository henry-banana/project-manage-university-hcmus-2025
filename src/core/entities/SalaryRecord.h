#ifndef SALARYRECORD_H
#define SALARYRECORD_H

#include "IEntity.h"
#include <string>

class SalaryRecord : public IEntity {
private:
    std::string _teacherId; // ID của SalaryRecord
    long _basicMonthlyPay;
    // double _bonusRate; // (Optional)
    // int _allowances;   // (Optional)

public:
    SalaryRecord(std::string teacherId, long basicMonthlyPay);

    // Getters
    const std::string& getTeacherId() const;
    long getBasicMonthlyPay() const;

    // Setters
    bool setBasicMonthlyPay(long pay); // >= 0

    // Implement IEntity
    std::string getStringId() const override; // Trả về _teacherId
    std::string display() const override;
    ValidationResult validateBasic() const override;
};

#endif // SALARYRECORD_H