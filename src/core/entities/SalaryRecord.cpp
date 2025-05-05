#include "SalaryRecord.h"
#include <iostream>
#include <format> // C++20 for std::format

SalaryRecord::SalaryRecord() : _teacherId(""), _basicMonthlyPay(0) {}   

SalaryRecord::SalaryRecord(std::string facultyId, int basicMonthlyPay) 
    : _teacherId(facultyId), _basicMonthlyPay(basicMonthlyPay) {
    // Ensure basic monthly pay is non-negative
    if (basicMonthlyPay < 0) {
        _basicMonthlyPay = 0;
    }
}

SalaryRecord::SalaryRecord(const SalaryRecord& other)
    : _teacherId(other._teacherId), _basicMonthlyPay(other._basicMonthlyPay) {
    // Copy constructor
}

SalaryRecord& SalaryRecord::operator=(const SalaryRecord& other) {
    if (this != &other) { // Self-assignment check
        _teacherId = other._teacherId;
        _basicMonthlyPay = other._basicMonthlyPay;
    }
    return *this; // Return the current object
}

// Getters
const std::string& SalaryRecord::teacherId() const { return _teacherId; }
int SalaryRecord::basicMonthlyPay() const { return _basicMonthlyPay; }

// Setters
void SalaryRecord::setTeacherId(const std::string& teacherId) {
    _teacherId = teacherId;
}

void SalaryRecord::setBasicMonthlyPay(int newPay) {
    _basicMonthlyPay = (newPay >= 0) ? newPay : 0;
}

// Display
void SalaryRecord::display() const {
    std::cout << std::format(
        "--- Salary Record ---\n"
        "Teacher ID:      {}\n"
        "Basic Monthly:   {} VND", 
        _teacherId, _basicMonthlyPay
    );
}

/* Example Calculation implementations (if kept here, though Service is better)
int SalaryRecord::calculateAnnualBasicPay() const {
    return _basicMonthlyPay * 12;
}

int SalaryRecord::calculateAnnualBonus(double bonusRate = 0.9) const { // Example rate
    // Bonus calculation logic (e.g., based on basic pay)
    return static_cast<int>(calculateAnnualBasicPay() * bonusRate); // Just an example
}

int SalaryRecord::calculateTotalAnnualPay(double bonusRate = 0.9) const {
    return calculateAnnualBasicPay() + calculateAnnualBonus(bonusRate);
}
*/