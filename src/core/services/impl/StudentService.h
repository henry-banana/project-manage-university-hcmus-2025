/**
 * @file StudentService.h
 * @brief Triển khai dịch vụ quản lý sinh viên
 * 
 * StudentService triển khai các chức năng quản lý sinh viên theo giao diện
 * IStudentService, cung cấp các phương thức để quản lý thông tin sinh viên.
 */
#ifndef STUDENTSERVICE_H
#define STUDENTSERVICE_H

#include "../interface/IStudentService.h" // Đã được cập nhật
#include "../../data_access/interface/ITeacherDao.h" // (➕) Thêm TeacherDao
#include "../../data_access/interface/IStudentDao.h"
#include "../../data_access/interface/IFacultyDao.h"
#include "../../validators/interface/IValidator.h"
#include "../SessionContext.h" // (Optional)

/**
 * @class StudentService
 * @brief Lớp triển khai dịch vụ quản lý sinh viên
 * 
 * Lớp này triển khai các phương thức của giao diện IStudentService để
 * cung cấp các chức năng quản lý thông tin sinh viên.
 */
class StudentService : public IStudentService {
private:
    std::shared_ptr<IStudentDao> _studentDao;      ///< Đối tượng dao để truy cập dữ liệu sinh viên
    std::shared_ptr<ITeacherDao> _teacherDao;      ///< Đối tượng dao để truy cập dữ liệu giáo viên
    std::shared_ptr<IFacultyDao> _facultyDao;      ///< Đối tượng dao để truy cập dữ liệu khoa
    std::shared_ptr<IGeneralInputValidator> _inputValidator; ///< Đối tượng validator để kiểm tra đầu vào
    std::shared_ptr<SessionContext> _sessionContext; ///< Đối tượng quản lý phiên làm việc

public:
    /**
     * @brief Hàm khởi tạo StudentService
     * @param studentDao Đối tượng dao để truy cập dữ liệu sinh viên
     * @param teacherDao Đối tượng dao để truy cập dữ liệu giáo viên
     * @param facultyDao Đối tượng dao để truy cập dữ liệu khoa
     * @param inputValidator Đối tượng validator để kiểm tra đầu vào
     * @param sessionContext Đối tượng quản lý phiên làm việc
     */
    StudentService(std::shared_ptr<IStudentDao> studentDao,
                   std::shared_ptr<ITeacherDao> teacherDao,
                   std::shared_ptr<IFacultyDao> facultyDao,
                   std::shared_ptr<IGeneralInputValidator> inputValidator,
                   std::shared_ptr<SessionContext> sessionContext); // (➕)
    
    /**
     * @brief Hàm hủy ảo mặc định
     */
    ~StudentService() override = default;    /**
     * @brief Lấy thông tin chi tiết của sinh viên theo ID
     * @param studentId ID của sinh viên
     * @return Đối tượng Student nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Student, Error> getStudentDetails(const std::string& studentId) const override;
    
    /**
     * @brief Lấy danh sách tất cả sinh viên
     * @return Danh sách các đối tượng Student nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Student>, Error> getAllStudents() const override;
    
    /**
     * @brief Lấy danh sách sinh viên theo khoa
     * @param facultyId ID của khoa
     * @return Danh sách các đối tượng Student thuộc khoa, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Student>, Error> getStudentsByFaculty(const std::string& facultyId) const override;
    
    /**
     * @brief Cập nhật thông tin sinh viên
     * @param data Cấu trúc dữ liệu chứa thông tin cần cập nhật
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> updateStudentDetails(const StudentUpdateData& data) override;
};

#endif // STUDENTSERVICE_H