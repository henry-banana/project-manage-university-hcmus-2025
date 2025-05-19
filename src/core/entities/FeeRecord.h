#ifndef FEERECORD_H
#define FEERECORD_H

#include "IEntity.h"
#include <string>

class FeeRecord : public IEntity {
private:
    std::string _studentId; // Đây sẽ là ID của FeeRecord
    long _totalFee;
    long _paidFee;

public:
    FeeRecord(std::string studentId, long totalFee, long paidFee = 0);

    // Getters
    const std::string& getStudentId() const;
    long getTotalFee() const;
    long getPaidFee() const;
    long getDueFee() const;
    bool isFullyPaid() const;

    // Setters & Actions
    bool setTotalFee(long totalFee); // >= 0
    bool setPaidFee(long paidFee);   // >= 0 and <= totalFee
    bool makePayment(long amount); // Trả về true nếu thanh toán hợp lệ

    // Implement IEntity
    std::string getStringId() const override; // Trả về _studentId
    std::string display() const override;
    ValidationResult validateBasic() const override;
};

#endif // FEERECORD_H