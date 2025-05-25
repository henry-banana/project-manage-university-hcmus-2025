/**
 * @file FacultyService.h
 * @brief Triển khai dịch vụ quản lý khoa
 * 
 * FacultyService triển khai các chức năng quản lý khoa theo giao diện
 * IFacultyService, cung cấp các phương thức để quản lý thông tin khoa.
 */
#ifndef FACULTYSERVICE_H
#define FACULTYSERVICE_H

#include "../interface/IFacultyService.h" // Đã được cập nhật
#include "../../data_access/interface/IFacultyDao.h"
#include "../../data_access/interface/IStudentDao.h"  // (➕) Để check ràng buộc
#include "../../data_access/interface/ITeacherDao.h"  // (➕)
#include "../../data_access/interface/ICourseDao.h"   // (➕)
#include "../../validators/interface/IValidator.h"
#include "../SessionContext.h" // (➕)

/**
 * @class FacultyService
 * @brief Lớp triển khai dịch vụ quản lý khoa
 * 
 * Lớp này triển khai các phương thức của giao diện IFacultyService để
 * cung cấp các chức năng quản lý thông tin khoa.
 */
class FacultyService : public IFacultyService {
private:
    std::shared_ptr<IFacultyDao> _facultyDao;        ///< Đối tượng dao để truy cập dữ liệu khoa
    std::shared_ptr<IStudentDao> _studentDao;        ///< Đối tượng dao để truy cập dữ liệu sinh viên
    std::shared_ptr<ITeacherDao> _teacherDao;        ///< Đối tượng dao để truy cập dữ liệu giáo viên
    std::shared_ptr<ICourseDao> _courseDao;          ///< Đối tượng dao để truy cập dữ liệu khóa học
    std::shared_ptr<IGeneralInputValidator> _inputValidator; ///< Đối tượng validator để kiểm tra đầu vào
    std::shared_ptr<SessionContext> _sessionContext; ///< Đối tượng quản lý phiên làm việc

public:
    /**
     * @brief Hàm khởi tạo FacultyService
     * @param facultyDao Đối tượng dao để truy cập dữ liệu khoa
     * @param studentDao Đối tượng dao để truy cập dữ liệu sinh viên
     * @param teacherDao Đối tượng dao để truy cập dữ liệu giáo viên
     * @param courseDao Đối tượng dao để truy cập dữ liệu khóa học
     * @param inputValidator Đối tượng validator để kiểm tra đầu vào
     * @param sessionContext Đối tượng quản lý phiên làm việc
     */
    FacultyService(std::shared_ptr<IFacultyDao> facultyDao,
                   std::shared_ptr<IStudentDao> studentDao, // (➕)
                   std::shared_ptr<ITeacherDao> teacherDao, // (➕)
                   std::shared_ptr<ICourseDao> courseDao,   // (➕)
                   std::shared_ptr<IGeneralInputValidator> inputValidator,
                   std::shared_ptr<SessionContext> sessionContext); // (➕)
    
    /**
     * @brief Hàm hủy ảo mặc định
     */
    ~FacultyService() override = default;    /**
     * @brief Lấy thông tin khoa theo ID
     * @param facultyId ID của khoa
     * @return Đối tượng Faculty nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Faculty, Error> getFacultyById(const std::string& facultyId) const override;
    
    /**
     * @brief Lấy thông tin khoa theo tên
     * @param name Tên của khoa
     * @return Đối tượng Faculty nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Faculty, Error> getFacultyByName(const std::string& name) const override;
    
    /**
     * @brief Lấy danh sách tất cả các khoa
     * @return Danh sách các đối tượng Faculty nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Faculty>, Error> getAllFaculties() const override;
    
    /**
     * @brief Thêm khoa mới
     * @param id ID của khoa mới
     * @param name Tên khoa mới
     * @return Đối tượng Faculty mới nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Faculty, Error> addFaculty(const std::string& id, const std::string& name) override;
    
    /**
     * @brief Cập nhật thông tin khoa
     * @param facultyId ID của khoa cần cập nhật
     * @param newName Tên mới của khoa
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> updateFaculty(const std::string& facultyId, const std::string& newName) override;
    
    /**
     * @brief Xóa khoa
     * @param facultyId ID của khoa cần xóa
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> removeFaculty(const std::string& facultyId) override;
};

#endif // FACULTYSERVICE_H