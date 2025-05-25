#ifndef USERROLE_H
#define USERROLE_H

/**
 * @enum UserRole
 * @brief Liệt kê các vai trò người dùng trong hệ thống
 * 
 * Enum này định nghĩa các vai trò có thể có của người dùng trong hệ thống
 * quản lý trường đại học, bao gồm quản trị viên, sinh viên, giảng viên,
 * sinh viên chờ duyệt, và không xác định.
 */
enum class UserRole {
    ADMIN,           ///< Quản trị viên hệ thống
    STUDENT,         ///< Sinh viên đã được phê duyệt
    TEACHER,         ///< Giảng viên
    PENDING_STUDENT, ///< Sinh viên đã đăng ký nhưng chờ duyệt
    UNKNOWN          ///< Trạng thái không xác định hoặc chưa đăng nhập
};

#endif // USERROLE_H