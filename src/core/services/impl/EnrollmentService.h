/**
 * @file EnrollmentService.h
 * @brief Triển khai dịch vụ quản lý đăng ký khóa học
 * 
 * EnrollmentService triển khai các chức năng quản lý đăng ký khóa học theo giao diện
 * IEnrollmentService, cung cấp các phương thức để quản lý việc đăng ký và hủy đăng ký khóa học.
 */
#ifndef ENROLLMENTSERVICE_H
#define ENROLLMENTSERVICE_H

#include "../interface/IEnrollmentService.h"
#include "../../data_access/interface/IEnrollmentDao.h"
#include "../../data_access/interface/IStudentDao.h"  // Để kiểm tra Student tồn tại và status
#include "../../data_access/interface/ICourseDao.h"   // Để kiểm tra Course tồn tại
#include "../../validators/interface/IValidator.h"    // GeneralInputValidator
#include "../SessionContext.h"

/**
 * @class EnrollmentService
 * @brief Lớp triển khai dịch vụ quản lý đăng ký khóa học
 * 
 * Lớp này triển khai các phương thức của giao diện IEnrollmentService để
 * cung cấp các chức năng quản lý việc đăng ký và hủy đăng ký khóa học.
 */
class EnrollmentService : public IEnrollmentService {
private:
    std::shared_ptr<IEnrollmentDao> _enrollmentDao;   ///< Đối tượng dao để truy cập dữ liệu đăng ký khóa học
    std::shared_ptr<IStudentDao> _studentDao;         ///< Đối tượng dao để truy cập dữ liệu sinh viên
    std::shared_ptr<ICourseDao> _courseDao;           ///< Đối tượng dao để truy cập dữ liệu khóa học
    std::shared_ptr<IGeneralInputValidator> _inputValidator; ///< Đối tượng validator để kiểm tra đầu vào
    std::shared_ptr<SessionContext> _sessionContext;  ///< Đối tượng quản lý phiên làm việc

public:
    /**
     * @brief Hàm khởi tạo EnrollmentService
     * @param enrollmentDao Đối tượng dao để truy cập dữ liệu đăng ký khóa học
     * @param studentDao Đối tượng dao để truy cập dữ liệu sinh viên
     * @param courseDao Đối tượng dao để truy cập dữ liệu khóa học
     * @param inputValidator Đối tượng validator để kiểm tra đầu vào
     * @param sessionContext Đối tượng quản lý phiên làm việc
     */
    EnrollmentService(std::shared_ptr<IEnrollmentDao> enrollmentDao,
                      std::shared_ptr<IStudentDao> studentDao,
                      std::shared_ptr<ICourseDao> courseDao,
                      std::shared_ptr<IGeneralInputValidator> inputValidator,
                      std::shared_ptr<SessionContext> sessionContext);
    
    /**
     * @brief Hàm hủy ảo mặc định
     */
    ~EnrollmentService() override = default;    /**
     * @brief Đăng ký khóa học cho sinh viên
     * @param studentId ID của sinh viên
     * @param courseId ID của khóa học
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> enrollStudentInCourse(const std::string& studentId, const std::string& courseId) override;
    
    /**
     * @brief Hủy đăng ký khóa học cho sinh viên
     * @param studentId ID của sinh viên
     * @param courseId ID của khóa học
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> dropCourseForStudent(const std::string& studentId, const std::string& courseId) override;
    
    /**
     * @brief Lấy danh sách khóa học mà sinh viên đã đăng ký
     * @param studentId ID của sinh viên
     * @return Danh sách các đối tượng Course nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Course>, Error> getEnrolledCoursesByStudent(const std::string& studentId) const override;
    
    /**
     * @brief Lấy danh sách sinh viên đã đăng ký khóa học
     * @param courseId ID của khóa học
     * @return Danh sách các đối tượng Student nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Student>, Error> getEnrolledStudentsByCourse(const std::string& courseId) const override;
    
    /**
     * @brief Kiểm tra xem sinh viên đã đăng ký khóa học hay chưa
     * @param studentId ID của sinh viên
     * @param courseId ID của khóa học
     * @return true nếu sinh viên đã đăng ký, false nếu chưa đăng ký, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> isStudentEnrolled(const std::string& studentId, const std::string& courseId) const override;
};

#endif // ENROLLMENTSERVICE_H