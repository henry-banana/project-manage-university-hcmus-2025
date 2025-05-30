#ifndef ENTITYTYPE_H
#define ENTITYTYPE_H

/**
 * @enum EntityType
 * @brief Liệt kê các loại thực thể trong hệ thống
 * 
 * Enum này định nghĩa các loại thực thể cơ bản trong hệ thống quản lý 
 * trường đại học, bao gồm sinh viên, giảng viên, khoa, khóa học, 
 * đăng ký khóa học, kết quả học tập, và các bản ghi tài chính.
 */
enum class EntityType {
    STUDENT,      ///< Thực thể sinh viên
    TEACHER,      ///< Thực thể giảng viên
    FACULTY,      ///< Thực thể khoa
    COURSE,       ///< Thực thể khóa học
    ENROLLMENT,   ///< Thực thể đăng ký khóa học
    COURSERESULT, ///< Thực thể kết quả học tập
    FEERECORD,    ///< Thực thể bản ghi học phí
    SALARYRECORD, ///< Thực thể bản ghi lương
    LOGIN         ///< Thực thể đăng nhập
};

#endif // ENTITYTYPE_H