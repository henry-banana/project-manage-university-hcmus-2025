#ifndef USERROLE_H
#define USERROLE_H

enum class UserRole {
    ADMIN,
    STUDENT,
    TEACHER,
    PENDING_STUDENT, // Sinh viên đã đăng ký nhưng chờ duyệt
    UNKNOWN          // Trạng thái không xác định hoặc chưa đăng nhập
};

#endif // USERROLE_H