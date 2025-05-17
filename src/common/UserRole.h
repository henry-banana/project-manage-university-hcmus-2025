#ifndef USER_ROLE_H
#define USER_ROLE_H

enum class UserRole {
    ADMIN,
    STUDENT,
    TEACHER,
    PENDING_STUDENT // Dành cho sinh viên đăng ký và chờ duyệt
    // Có thể thêm FINANCE_STAFF nếu cần phân quyền riêng cho phòng tài vụ
};

#endif // USER_ROLE_H