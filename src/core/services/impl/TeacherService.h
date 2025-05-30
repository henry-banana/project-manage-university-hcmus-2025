/**
 * @file TeacherService.h
 * @brief Triển khai dịch vụ quản lý giáo viên
 * 
 * TeacherService triển khai các chức năng quản lý giáo viên theo giao diện
 * ITeacherService, cung cấp các phương thức để quản lý thông tin giáo viên.
 */
#ifndef TEACHERSERVICE_H
#define TEACHERSERVICE_H

#include "../interface/ITeacherService.h" // Đã được cập nhật
#include "../../data_access/interface/ITeacherDao.h"
#include "../../data_access/interface/IStudentDao.h" // (➕) Để check email student
#include "../../data_access/interface/IFacultyDao.h"
#include "../../validators/interface/IValidator.h"
#include "../SessionContext.h" // (➕)

/**
 * @class TeacherService
 * @brief Lớp triển khai dịch vụ quản lý giáo viên
 * 
 * Lớp này triển khai các phương thức của giao diện ITeacherService để
 * cung cấp các chức năng quản lý thông tin giáo viên.
 */
class TeacherService : public ITeacherService {
private:
    std::shared_ptr<ITeacherDao> _teacherDao;      ///< Đối tượng dao để truy cập dữ liệu giáo viên
    std::shared_ptr<IStudentDao> _studentDao;      ///< Đối tượng dao để truy cập dữ liệu sinh viên
    std::shared_ptr<IFacultyDao> _facultyDao;      ///< Đối tượng dao để truy cập dữ liệu khoa
    std::shared_ptr<IGeneralInputValidator> _inputValidator; ///< Đối tượng validator để kiểm tra đầu vào
    std::shared_ptr<SessionContext> _sessionContext; ///< Đối tượng quản lý phiên làm việc

public:
    /**
     * @brief Hàm khởi tạo TeacherService
     * @param teacherDao Đối tượng dao để truy cập dữ liệu giáo viên
     * @param studentDao Đối tượng dao để truy cập dữ liệu sinh viên
     * @param facultyDao Đối tượng dao để truy cập dữ liệu khoa
     * @param inputValidator Đối tượng validator để kiểm tra đầu vào
     * @param sessionContext Đối tượng quản lý phiên làm việc
     */
    TeacherService(std::shared_ptr<ITeacherDao> teacherDao,
                   std::shared_ptr<IStudentDao> studentDao, // (➕)
                   std::shared_ptr<IFacultyDao> facultyDao,
                   std::shared_ptr<IGeneralInputValidator> inputValidator,
                   std::shared_ptr<SessionContext> sessionContext); // (➕)
    
    /**
     * @brief Hàm hủy ảo mặc định
     */
    ~TeacherService() override = default;    /**
     * @brief Lấy thông tin chi tiết của giáo viên theo ID
     * @param teacherId ID của giáo viên
     * @return Đối tượng Teacher nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Teacher, Error> getTeacherDetails(const std::string& teacherId) const override;
    
    /**
     * @brief Lấy danh sách tất cả giáo viên
     * @return Danh sách các đối tượng Teacher nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Teacher>, Error> getAllTeachers() const override;
    
    /**
     * @brief Lấy danh sách giáo viên theo khoa
     * @param facultyId ID của khoa
     * @return Danh sách các đối tượng Teacher thuộc khoa, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Teacher>, Error> getTeachersByFaculty(const std::string& facultyId) const override;
    
    /**
     * @brief Lấy danh sách giáo viên theo chức danh
     * @param designation Chức danh cần tìm
     * @return Danh sách các đối tượng Teacher có chức danh tương ứng, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Teacher>, Error> getTeachersByDesignation(const std::string& designation) const override;
    
    /**
     * @brief Cập nhật thông tin giáo viên
     * @param data Cấu trúc dữ liệu chứa thông tin cần cập nhật
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> updateTeacherDetails(const TeacherUpdateData& data) override;
};

#endif // TEACHERSERVICE_H