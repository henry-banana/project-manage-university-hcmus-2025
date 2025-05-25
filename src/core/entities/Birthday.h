#ifndef BIRTHDAY_H
#define BIRTHDAY_H

/**
 * @file Birthday.h
 * @brief Định nghĩa lớp Birthday để quản lý thông tin ngày sinh
 * 
 * File này định nghĩa lớp Birthday, được sử dụng để quản lý thông tin ngày sinh
 * trong hệ thống quản lý trường đại học. Lớp này cung cấp các phương thức để đặt,
 * lấy và kiểm tra tính hợp lệ của ngày sinh.
 */

#include <string>
#include <vector> // for std::vector in ValidationResult
#include "../../common/ErrorType.h" // For Error struct
#include "../../common/ValidationResult.h"

/**
 * @class Birthday
 * @brief Lớp quản lý thông tin ngày sinh
 * 
 * Lớp này cung cấp các phương thức để đặt, lấy và kiểm tra tính hợp lệ
 * của ngày sinh. Nó hỗ trợ cả định dạng số và chuỗi cho ngày sinh.
 */
class Birthday {
private:
    int _day; ///< Ngày trong tháng
    int _month; ///< Tháng trong năm
    int _year; ///< Năm
    bool _isSet; ///< Để biết ngày sinh đã được đặt hay chưa

public:
    /**
     * @brief Constructor mặc định
     * 
     * Khởi tạo một đối tượng Birthday với giá trị mặc định và _isSet là false
     */
    Birthday();

    /**
     * @brief Constructor với ngày, tháng, năm
     * 
     * @param day Ngày trong tháng
     * @param month Tháng trong năm
     * @param year Năm
     */
    Birthday(int day, int month, int year);

    /**
     * @brief Đặt ngày sinh với các giá trị số
     * 
     * @param day Ngày trong tháng
     * @param month Tháng trong năm
     * @param year Năm
     * @return true nếu set thành công, false nếu ngày không hợp lệ
     */
    bool set(int day, int month, int year);

    /**
     * @brief Đặt ngày sinh từ một chuỗi định dạng dd/mm/yyyy
     * 
     * @param dateStr_ddmmyyyy Chuỗi ngày sinh theo định dạng dd/mm/yyyy
     * @param delimiter Ký tự phân cách (mặc định là '/')
     * @return true nếu set thành công, false nếu chuỗi không hợp lệ
     */
    bool setFromString(const std::string& dateStr_ddmmyyyy, char delimiter = '/');

    /**
     * @brief Lấy ngày trong tháng
     * @return Ngày trong tháng
     */
    int getDay() const;

    /**
     * @brief Lấy tháng trong năm
     * @return Tháng trong năm
     */
    int getMonth() const;

    /**
     * @brief Lấy năm
     * @return Năm
     */
    int getYear() const;

    /**
     * @brief Kiểm tra đã set ngày sinh chưa
     * @return true nếu đã set, false nếu chưa
     */
    bool isSet() const; // Kiểm tra đã set ngày sinh chưa

    /**
     * @brief Chuyển đổi ngày sinh thành chuỗi định dạng dd/mm/yyyy
     * 
     * @param delimiter Ký tự phân cách (mặc định là '/')
     * @return Chuỗi ngày sinh theo định dạng dd/mm/yyyy
     */
    std::string toString_ddmmyyyy(char delimiter = '/') const;

    /**
     * @brief Kiểm tra tính hợp lệ của ngày tháng hiện tại
     * @return Kết quả kiểm tra tính hợp lệ
     */

    ValidationResult validate() const; // Kiểm tra tính hợp lệ của ngày tháng hiện tại
};

#endif // BIRTHDAY_H