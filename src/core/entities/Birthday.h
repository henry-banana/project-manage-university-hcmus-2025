#ifndef BIRTHDAY_H
#define BIRTHDAY_H

#include <string>
#include <vector> // for std::vector in ValidationResult
#include "../../common/ErrorType.h" // For Error struct
#include "../../common/ValidationResult.h"

class Birthday {
private:
    int _day;
    int _month;
    int _year;
    bool _isSet; // Để biết ngày sinh đã được đặt hay chưa

public:
    Birthday();
    Birthday(int day, int month, int year);

    // Trả về true nếu set thành công, false nếu ngày không hợp lệ
    bool set(int day, int month, int year);
    bool setFromString(const std::string& dateStr_ddmmyyyy, char delimiter = '/');

    int getDay() const;
    int getMonth() const;
    int getYear() const;
    bool isSet() const; // Kiểm tra đã set ngày sinh chưa

    std::string toString_ddmmyyyy(char delimiter = '/') const;

    ValidationResult validate() const; // Kiểm tra tính hợp lệ của ngày tháng hiện tại
};

#endif // BIRTHDAY_H