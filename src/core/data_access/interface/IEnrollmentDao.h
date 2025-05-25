/**
 * @file IEnrollmentDao.h
 * @brief Định nghĩa giao diện DAO cho truy cập dữ liệu đăng ký khóa học
 * 
 * Giao diện IEnrollmentDao định nghĩa các phương thức để truy cập và
 * thao tác với dữ liệu đăng ký khóa học trong hệ thống.
 */
#ifndef IENROLLMENTDAO_H
#define IENROLLMENTDAO_H

#include <string>
#include <vector>
#include <expected> // (➕)
#include "../../../common/ErrorType.h" // (➕)

// EnrollmentRecord đã được khai báo trong file header của IEnrollmentDao trước đó, giữ nguyên
/**
 * @struct EnrollmentRecord
 * @brief Cấu trúc dữ liệu chứa thông tin đăng ký khóa học
 * 
 * Cấu trúc này chứa ID của sinh viên và ID của khóa học mà sinh viên đã đăng ký.
 */
struct EnrollmentRecord {
    std::string studentId; ///< ID của sinh viên
    std::string courseId;  ///< ID của khóa học
};

/**
 * @class IEnrollmentDao
 * @brief Giao diện DAO cho truy cập dữ liệu đăng ký khóa học
 * 
 * Giao diện này định nghĩa các phương thức để truy cập và thao tác
 * với dữ liệu đăng ký khóa học của sinh viên.
 */
class IEnrollmentDao {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~IEnrollmentDao() = default;    /**
     * @brief Thêm bản ghi đăng ký khóa học mới
     * @param studentId ID của sinh viên
     * @param courseId ID của khóa học
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> addEnrollment(const std::string& studentId, const std::string& courseId) = 0;
    
    /**
     * @brief Xóa bản ghi đăng ký khóa học
     * @param studentId ID của sinh viên
     * @param courseId ID của khóa học
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> removeEnrollment(const std::string& studentId, const std::string& courseId) = 0;
    
    /**
     * @brief Xóa tất cả bản ghi đăng ký khóa học của một sinh viên
     * @param studentId ID của sinh viên
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> removeEnrollmentsByStudent(const std::string& studentId) = 0;
    
    /**
     * @brief Xóa tất cả bản ghi đăng ký của một khóa học
     * @param courseId ID của khóa học
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> removeEnrollmentsByCourse(const std::string& courseId) = 0;
    
    /**
     * @brief Kiểm tra xem sinh viên đã đăng ký khóa học hay chưa
     * @param studentId ID của sinh viên
     * @param courseId ID của khóa học
     * @return true nếu sinh viên đã đăng ký, false nếu chưa đăng ký, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> isEnrolled(const std::string& studentId, const std::string& courseId) const = 0; // Trả về bool (true/false) hoặc Error
    
    /**
     * @brief Tìm danh sách ID khóa học mà sinh viên đã đăng ký
     * @param studentId ID của sinh viên
     * @return Danh sách ID khóa học nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<std::string>, Error> findCourseIdsByStudentId(const std::string& studentId) const = 0;
    
    /**
     * @brief Tìm danh sách ID sinh viên đã đăng ký khóa học
     * @param courseId ID của khóa học
     * @return Danh sách ID sinh viên nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<std::string>, Error> findStudentIdsByCourseId(const std::string& courseId) const = 0;
    
    /**
     * @brief Lấy tất cả bản ghi đăng ký khóa học
     * @return Danh sách các đối tượng EnrollmentRecord nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<EnrollmentRecord>, Error> getAllEnrollments() const = 0;
};

#endif // IENROLLMENTDAO_H