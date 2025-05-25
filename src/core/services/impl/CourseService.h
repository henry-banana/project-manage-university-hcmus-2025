/**
 * @file CourseService.h
 * @brief Triển khai dịch vụ quản lý khóa học
 * 
 * CourseService triển khai các chức năng quản lý khóa học theo giao diện
 * ICourseService, cung cấp các phương thức để quản lý thông tin khóa học.
 */
#ifndef COURSESERVICE_H
#define COURSESERVICE_H

#include "../interface/ICourseService.h" // Đã được cập nhật
#include "../../data_access/interface/ICourseDao.h"
#include "../../data_access/interface/IFacultyDao.h"
#include "../../data_access/interface/IEnrollmentDao.h" // (➕) Để check ràng buộc
#include "../../data_access/interface/ICourseResultDao.h"// (➕) Để check ràng buộc
#include "../../validators/interface/IValidator.h"
#include "../SessionContext.h" // (➕)

/**
 * @class CourseService
 * @brief Lớp triển khai dịch vụ quản lý khóa học
 * 
 * Lớp này triển khai các phương thức của giao diện ICourseService để
 * cung cấp các chức năng quản lý thông tin khóa học.
 */
class CourseService : public ICourseService {
private:
    std::shared_ptr<ICourseDao> _courseDao;             ///< Đối tượng dao để truy cập dữ liệu khóa học
    std::shared_ptr<IFacultyDao> _facultyDao;           ///< Đối tượng dao để truy cập dữ liệu khoa
    std::shared_ptr<IEnrollmentDao> _enrollmentDao;     ///< Đối tượng dao để truy cập dữ liệu đăng ký khóa học
    std::shared_ptr<ICourseResultDao> _courseResultDao; ///< Đối tượng dao để truy cập dữ liệu kết quả khóa học
    std::shared_ptr<IGeneralInputValidator> _inputValidator; ///< Đối tượng validator để kiểm tra đầu vào
    std::shared_ptr<SessionContext> _sessionContext;    ///< Đối tượng quản lý phiên làm việc

public:
    /**
     * @brief Hàm khởi tạo CourseService
     * @param courseDao Đối tượng dao để truy cập dữ liệu khóa học
     * @param facultyDao Đối tượng dao để truy cập dữ liệu khoa
     * @param enrollmentDao Đối tượng dao để truy cập dữ liệu đăng ký khóa học
     * @param courseResultDao Đối tượng dao để truy cập dữ liệu kết quả khóa học
     * @param inputValidator Đối tượng validator để kiểm tra đầu vào
     * @param sessionContext Đối tượng quản lý phiên làm việc
     */
    CourseService(std::shared_ptr<ICourseDao> courseDao,
                  std::shared_ptr<IFacultyDao> facultyDao,
                  std::shared_ptr<IEnrollmentDao> enrollmentDao, // (➕)
                  std::shared_ptr<ICourseResultDao> courseResultDao, // (➕)
                  std::shared_ptr<IGeneralInputValidator> inputValidator,
                  std::shared_ptr<SessionContext> sessionContext); // (➕)
    
    /**
     * @brief Hàm hủy ảo mặc định
     */
    ~CourseService() override = default;    /**
     * @brief Lấy thông tin khóa học theo ID
     * @param courseId ID của khóa học
     * @return Đối tượng Course nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Course, Error> getCourseById(const std::string& courseId) const override;
    
    /**
     * @brief Lấy danh sách tất cả khóa học
     * @return Danh sách các đối tượng Course nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Course>, Error> getAllCourses() const override;
    
    /**
     * @brief Lấy danh sách khóa học theo khoa
     * @param facultyId ID của khoa
     * @return Danh sách các đối tượng Course thuộc khoa, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Course>, Error> getCoursesByFaculty(const std::string& facultyId) const override;
    
    /**
     * @brief Thêm khóa học mới
     * @param id ID của khóa học mới
     * @param name Tên khóa học
     * @param credits Số tín chỉ
     * @param facultyId ID của khoa quản lý khóa học
     * @return Đối tượng Course mới nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Course, Error> addCourse(const std::string& id, const std::string& name, int credits, const std::string& facultyId) override;
    
    /**
     * @brief Cập nhật thông tin khóa học
     * @param courseId ID của khóa học cần cập nhật
     * @param newName Tên mới của khóa học
     * @param newCredits Số tín chỉ mới
     * @param newFacultyId ID mới của khoa quản lý
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> updateCourse(const std::string& courseId, const std::string& newName, int newCredits, const std::string& newFacultyId) override;
    
    /**
     * @brief Xóa khóa học
     * @param courseId ID của khóa học cần xóa
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> removeCourse(const std::string& courseId) override;
};

#endif // COURSESERVICE_H