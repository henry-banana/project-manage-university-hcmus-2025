#include "SalaryRecord.h"
#include <sstream>
#include "../../utils/StringUtils.h" 

SalaryRecord::SalaryRecord(std::string teacherId, long basicMonthlyPay)
    : _teacherId(std::move(teacherId)), _basicMonthlyPay(0) {
    setBasicMonthlyPay(basicMonthlyPay); // Dùng setter
}

const std::string& SalaryRecord::getTeacherId() const { return _teacherId; }
long SalaryRecord::getBasicMonthlyPay() const { return _basicMonthlyPay; }

bool SalaryRecord::setBasicMonthlyPay(long pay) {
    if (pay < 0) return false;
    _basicMonthlyPay = pay;
    return true;
}

std::string SalaryRecord::getStringId() const { return _teacherId; }

std::string SalaryRecord::display() const {
    std::ostringstream oss;
    oss << "--- Salary Record for Teacher: " << _teacherId << " ---\n"
        << "Basic Monthly Pay : " << _basicMonthlyPay << "\n"
        // << "Annual Basic Pay  : " << (_basicMonthlyPay * 12) << "\n" // Có thể tính ở Service
        << "---------------------------------------";
    return oss.str();
}

ValidationResult SalaryRecord::validateBasic() const {
    ValidationResult vr;
    if (StringUtils::trim(_teacherId).empty()) vr.addError(ErrorCode::VALIDATION_ERROR, "Teacher ID in SalaryRecord cannot be empty.");
    if (_basicMonthlyPay < 0) vr.addError(ErrorCode::VALIDATION_ERROR, "Basic monthly pay cannot be negative.");
    return vr;
}