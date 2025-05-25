/**
 * @file ResultService.h
 * @brief Triển khai dịch vụ quản lý kết quả học tập
 * 
 * ResultService triển khai các chức năng quản lý kết quả học tập theo giao diện
 * IResultService, cung cấp các phương thức để nhập và truy vấn điểm số của sinh viên.
 */
#ifndef RESULTSERVICE_H
#define RESULTSERVICE_H

#include "../interface/IResultService.h"
#include "../../data_access/interface/ICourseResultDao.h"
#include "../../data_access/interface/IStudentDao.h"
#include "../../data_access/interface/ICourseDao.h"
#include "../../data_access/interface/IFacultyDao.h"
#include "../../data_access/interface/IEnrollmentDao.h" // Để kiểm tra SV có đăng ký môn đó không
#include "../../validators/interface/IValidator.h"
#include "../SessionContext.h"
#include <iomanip> // For setprecision in report

/**
 * @class ResultService
 * @brief Lớp triển khai dịch vụ quản lý kết quả học tập
 * 
 * Lớp này triển khai các phương thức của giao diện IResultService để
 * cung cấp các chức năng quản lý điểm số và kết quả học tập của sinh viên.
 */
class ResultService : public IResultService {
private:
    std::shared_ptr<ICourseResultDao> _resultDao;      ///< Đối tượng dao để truy cập dữ liệu kết quả khóa học
    std::shared_ptr<IFacultyDao> _facultyDao;          ///< Đối tượng dao để truy cập dữ liệu khoa
    std::shared_ptr<IStudentDao> _studentDao;          ///< Đối tượng dao để truy cập dữ liệu sinh viên
    std::shared_ptr<ICourseDao> _courseDao;            ///< Đối tượng dao để truy cập dữ liệu khóa học
    std::shared_ptr<IEnrollmentDao> _enrollmentDao;    ///< Đối tượng dao để truy cập dữ liệu đăng ký khóa học
    std::shared_ptr<IGeneralInputValidator> _inputValidator; ///< Đối tượng validator để kiểm tra đầu vào
    std::shared_ptr<SessionContext> _sessionContext;   ///< Đối tượng quản lý phiên làm việc

public:
    /**
     * @brief Hàm khởi tạo ResultService
     * @param resultDao Đối tượng dao để truy cập dữ liệu kết quả khóa học
     * @param _facultyDao Đối tượng dao để truy cập dữ liệu khoa
     * @param studentDao Đối tượng dao để truy cập dữ liệu sinh viên
     * @param courseDao Đối tượng dao để truy cập dữ liệu khóa học
     * @param enrollmentDao Đối tượng dao để truy cập dữ liệu đăng ký khóa học
     * @param inputValidator Đối tượng validator để kiểm tra đầu vào
     * @param sessionContext Đối tượng quản lý phiên làm việc
     */
    ResultService(std::shared_ptr<ICourseResultDao> resultDao,
                    std::shared_ptr<IFacultyDao> _facultyDao,
                  std::shared_ptr<IStudentDao> studentDao,
                  std::shared_ptr<ICourseDao> courseDao,
                  std::shared_ptr<IEnrollmentDao> enrollmentDao,
                  std::shared_ptr<IGeneralInputValidator> inputValidator,
                  std::shared_ptr<SessionContext> sessionContext);
    
    /**
     * @brief Hàm hủy ảo mặc định
     */
    ~ResultService() override = default;    /**
     * @brief Nhập điểm cho sinh viên trong một khóa học
     * @param studentId ID của sinh viên
     * @param courseId ID của khóa học
     * @param marks Điểm số
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> enterMarks(const std::string& studentId, const std::string& courseId, int marks) override;
    
    /**
     * @brief Nhập nhiều điểm cho sinh viên trong nhiều khóa học
     * @param studentId ID của sinh viên
     * @param courseMarksMap Map chứa cặp <courseId, marks> để nhập điểm cho nhiều khóa học
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> enterMultipleMarks(const std::string& studentId, const std::map<std::string, int>& courseMarksMap) override;
    
    /**
     * @brief Lấy kết quả cụ thể của sinh viên trong một khóa học
     * @param studentId ID của sinh viên
     * @param courseId ID của khóa học
     * @return Đối tượng CourseResult nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<CourseResult, Error> getSpecificResult(const std::string& studentId, const std::string& courseId) const override;
    
    /**
     * @brief Lấy tất cả kết quả của sinh viên
     * @param studentId ID của sinh viên
     * @return Danh sách các đối tượng CourseResult nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::vector<CourseResult>, Error> getResultsByStudent(const std::string& studentId) const override;
    
    /**
     * @brief Lấy tất cả kết quả của một khóa học
     * @param courseId ID của khóa học
     * @return Danh sách các đối tượng CourseResult nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::vector<CourseResult>, Error> getResultsByCourse(const std::string& courseId) const override;
    
    /**
     * @brief Tạo báo cáo kết quả học tập của sinh viên
     * @param studentId ID của sinh viên
     * @return Chuỗi báo cáo kết quả nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::string, Error> generateStudentResultReport(const std::string& studentId) const override;
    
    /**
     * @brief Tính điểm trung bình (CGPA) của sinh viên
     * @param studentId ID của sinh viên
     * @return Điểm trung bình nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<double, Error> calculateCGPA(const std::string& studentId) const override;
};

#endif // RESULTSERVICE_H