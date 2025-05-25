#ifndef MENUOPTION_H
#define MENUOPTION_H

/**
 * @file MenuOption.h
 * @brief Định nghĩa các enum cho các tùy chọn menu trong ứng dụng
 */

/**
 * @enum MainMenuOption
 * @brief Các tùy chọn trong menu chính
 */
enum class MainMenuOption {
    FINANCE = 1, ///< Tùy chọn quản lý tài chính
    ADMIN = 2,   ///< Tùy chọn quản trị hệ thống
    LOGIN = 3,   ///< Tùy chọn đăng nhập
    EXIT = 0     ///< Tùy chọn thoát ứng dụng
};

/**
 * @enum StudentMenuOption
 * @brief Các tùy chọn trong menu của sinh viên
 */
enum class StudentMenuOption {
    VIEW_RESULT = 1,  ///< Xem kết quả học tập
    VIEW_DETAILS = 2, ///< Xem thông tin chi tiết
    BACK = 0          ///< Quay lại menu trước
};

/**
 * @enum FacultyMenuOption
 * @brief Các tùy chọn trong menu của khoa
 */
enum class FacultyMenuOption {
    VIEW_DETAILS = 1, ///< Xem thông tin chi tiết
    BACK = 0          ///< Quay lại menu trước
};

/**
 * @enum AdminMainMenuOption
 * @brief Các tùy chọn trong menu chính của quản trị viên
 */
enum class AdminMainMenuOption {
    STUDENT_SECTION = 1, ///< Mục quản lý sinh viên
    FACULTY_SECTION = 2, ///< Mục quản lý khoa
    BACK = 0             ///< Quay lại menu trước
};

/**
 * @enum AdminStudentMenuOption
 * @brief Các tùy chọn trong menu quản lý sinh viên của quản trị viên
 */
enum class AdminStudentMenuOption {
    FIND_DISPLAY_STUDENT = 1, ///< Tìm và hiển thị thông tin sinh viên
    LIST_BY_DEPARTMENT = 2,   ///< Liệt kê sinh viên theo khoa
    BACK = 0                  ///< Quay lại menu trước
};

/**
 * @enum AdminFacultyMenuOption
 * @brief Các tùy chọn trong menu quản lý khoa của quản trị viên
 */
enum class AdminFacultyMenuOption {
    FIND_DISPLAY_FACULTY = 1, ///< Tìm và hiển thị thông tin khoa
    LIST_HODS = 2,            ///< Liệt kê trưởng khoa
    LIST_BY_DEPARTMENT = 3,   ///< Liệt kê theo khoa
    BACK = 0                  ///< Quay lại menu trước
};

/**
 * @enum FinanceStudentMenuOption
 * @brief Các tùy chọn trong menu tài chính của sinh viên
 */
enum class FinanceStudentMenuOption {
    PAY_FEES = 1,     ///< Thanh toán học phí
    VIEW_RECEIPT = 2, ///< Xem biên lai
    BACK = 0          ///< Quay lại menu trước
};

/**
 * @enum FinanceFacultyMenuOption
 * @brief Các tùy chọn trong menu tài chính của khoa
 */
enum class FinanceFacultyMenuOption {
    GENERATE_SALARY_CERT = 1, ///< Tạo chứng nhận lương
    BACK = 0                  ///< Quay lại menu trước
};

#endif // MENUOPTION_H