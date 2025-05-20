#include "FeeRecord.h"
#include <sstream>
#include "../../utils/StringUtils.h" // Giả sử có StringUtils::trim

FeeRecord::FeeRecord(std::string studentId, long totalFee, long paidFee)
    : _studentId(std::move(studentId)), _totalFee(0), _paidFee(0) {
    setTotalFee(totalFee); // Dùng setter để có validation
    setPaidFee(paidFee);   // Dùng setter
}

const std::string& FeeRecord::getStudentId() const { return _studentId; }
long FeeRecord::getTotalFee() const { return _totalFee; }
long FeeRecord::getPaidFee() const { return _paidFee; }
long FeeRecord::getDueFee() const { return _totalFee - _paidFee; }
bool FeeRecord::isFullyPaid() const { return _paidFee >= _totalFee; }

bool FeeRecord::setTotalFee(long totalFee) {
    if (totalFee < 0) return false;
    _totalFee = totalFee;
    // Đảm bảo paidFee không lớn hơn totalFee mới
    if (_paidFee > _totalFee) {
        _paidFee = _totalFee;
    }
    return true;
}

bool FeeRecord::setPaidFee(long paidFee) {
    if (paidFee < 0) return false;
    if (paidFee > _totalFee) { // Không cho phép trả nhiều hơn tổng số nợ
        _paidFee = _totalFee; // Chỉ trả tối đa bằng tổng số nợ
        return true; // Hoặc false nếu coi đây là lỗi
    }
    _paidFee = paidFee;
    return true;
}

bool FeeRecord::makePayment(long amount) {
    if (amount <= 0) return false; // Số tiền thanh toán phải dương
    if (isFullyPaid()) return true; // Đã trả đủ, coi như thành công (không làm gì)

    long newPaidFee = _paidFee + amount;
    return setPaidFee(newPaidFee); // Dùng setPaidFee để có validation
}

std::string FeeRecord::getStringId() const { return _studentId; }

std::string FeeRecord::display() const {
    std::ostringstream oss;
    oss << "--- Fee Record for Student: " << _studentId << " ---\n"
        << "Total Fee  : " << _totalFee << "\n"
        << "Paid Fee   : " << _paidFee << "\n"
        << "Due Fee    : " << getDueFee() << "\n"
        << "Status     : " << (isFullyPaid() ? "Fully Paid" : "Payment Due") << "\n"
        << "------------------------------------";
    return oss.str();
}

ValidationResult FeeRecord::validateBasic() const {
    ValidationResult vr;
    if (StringUtils::trim(_studentId).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Student ID in FeeRecord cannot be empty.");
    if (_totalFee < 0) vr.addError(ErrorCode::VALIDATION_ERROR, "Total fee cannot be negative.");
    if (_paidFee < 0) vr.addError(ErrorCode::VALIDATION_ERROR, "Paid fee cannot be negative.");
    if (_paidFee > _totalFee) vr.addError(ErrorCode::VALIDATION_ERROR, "Paid fee cannot exceed total fee.");
    return vr;
}