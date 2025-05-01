#ifndef FEERECORD_H
#define FEERECORD_H

#include <string>

class FeeRecord {
private:
    std::string _studentId;
    long _totalFee;
    long _pendingFee;

protected:

public:
    FeeRecord(std::string studentId, long totalFee, long pendingFee);

    const std::string& getStudentId() const { return _studentId; }
    long getTotalFee() const { return _totalFee; }
    long getPendingFee() const { return _pendingFee; }
    long getPaidFee() const { return _totalFee - _pendingFee; }
    bool isFullyPaid() const { return _pendingFee <= 0; }

    // Trả về true nếu thanh toán thành công, false nếu số tiền không hợp lệ
    bool makePayment(long amount);
};

#endif // FEERECORD_H