#include "FeeRecord.h"
#include <iostream>  // For display()
#include <format>   

FeeRecord::FeeRecord() 
    : _studentId(""), _totalFee(0), _paidFee(0) {} // Default constructor

FeeRecord::FeeRecord(std::string studentId, int totalFee, int paidFee)
    : _studentId(studentId), _totalFee(totalFee), _paidFee(paidFee) {
    // Ensure total fee and paid fee are non-negative
    if (totalFee < 0) {
        _totalFee = 0;
    }
    if (paidFee < 0) {
        _paidFee = 0;
    }
}

FeeRecord::FeeRecord(const FeeRecord& other)
    : _studentId(other._studentId), _totalFee(other._totalFee), _paidFee(other._paidFee) {
}

FeeRecord& FeeRecord::operator=(const FeeRecord& other) {
    if (this != &other) { // Self-assignment check
        _studentId = other._studentId;
        _totalFee = other._totalFee;
        _paidFee = other._paidFee;
    }
    return *this; // Return the current object
}

// Getters
const std::string& FeeRecord::studentId() const { return _studentId; }
int FeeRecord::totalFee() const { return _totalFee; }
int FeeRecord::paidFee() const { return _paidFee; }

bool FeeRecord::isFullyPaid() const {
    return _paidFee >= _totalFee;
}

// Setters
void FeeRecord::setTotalFee(int totalFee) {
    _totalFee = (totalFee >= 0) ? totalFee : 0;
    // Adjust paid fee if it now exceeds the new total fee
    if (_paidFee > _totalFee) {
        _paidFee = _totalFee;
    }
}

void FeeRecord::setStudentId(const std::string& studentId) {
    _studentId = studentId;
}

void FeeRecord::setPaidFee(int paidFee) {
    if (paidFee < 0) {
        _paidFee = 0;
    } else if (paidFee > _totalFee) {
        _paidFee = _totalFee; // Clamp to total fee
    } else {
        _paidFee = paidFee;
    }
} 

// Actions
bool FeeRecord::makePayment(int amount) {
    if (amount <= 0) {
        // Payment amount must be positive
        return false;
    }
    int currentPending = _totalFee - _paidFee;
    if (amount > currentPending) {
        // Cannot pay more than what's pending (or could allow overpayment if needed)
        // For now, just pay the pending amount if amount > pending
         _paidFee += currentPending;
         // Optionally return false or indicate partial payment applied
         // return false; // Or modify return to indicate actual amount paid
    } else {
       _paidFee += amount;
    }

    // Optional: Clamp paid fee just in case (shouldn't be necessary with logic above)
    // _paidFee = std::min(_paidFee, _totalFee);

    return true; // Payment (partially or fully) applied
}

// Display
void FeeRecord::display() const {
    std::cout << std::format(
        "--- Fee Record ---\n"
        "Student ID:   {}\n"
        "Total Fee:    {}\n"
        "Paid Fee:     {}\n"
        "Pending Fee:  {}\n"
        "Status:       {}\n"
        "------------------\n",
        _studentId,
        _totalFee,
        _paidFee,
        _totalFee - _paidFee,
        (isFullyPaid() ? "Fully Paid" : "Pending")
    );
}