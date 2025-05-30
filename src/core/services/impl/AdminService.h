/**
 * @file AdminService.h
 * @brief Triển khai dịch vụ quản trị hệ thống
 * 
 * AdminService triển khai các chức năng quản trị theo giao diện
 * IAdminService, cung cấp các phương thức để quản lý người dùng và hệ thống.
 */
#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include "../interface/IAdminService.h"
#include "../../data_access/interface/IStudentDao.h"
#include "../../data_access/interface/ITeacherDao.h"
#include "../../data_access/interface/IFacultyDao.h"
#include "../../data_access/interface/ILoginDao.h"
#include "../../data_access/interface/IFeeRecordDao.h"      // Để xóa khi remove student
#include "../../data_access/interface/ISalaryRecordDao.h"   // Để xóa khi remove teacher
#include "../../data_access/interface/IEnrollmentDao.h"   // Để xóa khi remove student
#include "../../data_access/interface/ICourseResultDao.h" // Để xóa khi remove student
#include "../../validators/interface/IValidator.h"
#include "../SessionContext.h"
#include "../../../utils/PasswordInput.h" // Để hash password mới

/**
 * @class AdminService
 * @brief Lớp triển khai dịch vụ quản trị hệ thống
 * 
 * Lớp này triển khai các phương thức của giao diện IAdminService để
 * cung cấp các chức năng quản trị hệ thống và quản lý người dùng.
 */
class AdminService : public IAdminService {
private:
    std::shared_ptr<IStudentDao> _studentDao;         ///< Đối tượng dao để truy cập dữ liệu sinh viên
    std::shared_ptr<ITeacherDao> _teacherDao;         ///< Đối tượng dao để truy cập dữ liệu giáo viên
    std::shared_ptr<IFacultyDao> _facultyDao;         ///< Đối tượng dao để truy cập dữ liệu khoa (nếu cần)
    std::shared_ptr<ILoginDao> _loginDao;             ///< Đối tượng dao để truy cập dữ liệu đăng nhập
    std::shared_ptr<IFeeRecordDao> _feeDao;           ///< Đối tượng dao để truy cập dữ liệu học phí
    std::shared_ptr<ISalaryRecordDao> _salaryDao;     ///< Đối tượng dao để truy cập dữ liệu lương
    std::shared_ptr<IEnrollmentDao> _enrollmentDao;   ///< Đối tượng dao để truy cập dữ liệu đăng ký khóa học
    std::shared_ptr<ICourseResultDao> _courseResultDao; ///< Đối tượng dao để truy cập dữ liệu kết quả khóa học
    std::shared_ptr<IGeneralInputValidator> _inputValidator; ///< Đối tượng validator để kiểm tra đầu vào
    std::shared_ptr<SessionContext> _sessionContext;  ///< Đối tượng quản lý phiên làm việc
    // Có thể inject IAuthService để dùng lại logic register (tạo User + Login)
    // std::shared_ptr<IAuthService> _authServiceForRegistration;

public:
    /**
     * @brief Hàm khởi tạo AdminService
     * @param studentDao Đối tượng dao để truy cập dữ liệu sinh viên
     * @param teacherDao Đối tượng dao để truy cập dữ liệu giáo viên
     * @param loginDao Đối tượng dao để truy cập dữ liệu đăng nhập
     * @param feeDao Đối tượng dao để truy cập dữ liệu học phí
     * @param salaryDao Đối tượng dao để truy cập dữ liệu lương
     * @param enrollmentDao Đối tượng dao để truy cập dữ liệu đăng ký khóa học
     * @param courseResultDao Đối tượng dao để truy cập dữ liệu kết quả khóa học
     * @param inputValidator Đối tượng validator để kiểm tra đầu vào
     * @param sessionContext Đối tượng quản lý phiên làm việc
     */
    AdminService(std::shared_ptr<IStudentDao> studentDao,
                 std::shared_ptr<ITeacherDao> teacherDao,
                 std::shared_ptr<IFacultyDao> facultyDao,
                 std::shared_ptr<ILoginDao> loginDao,
                 std::shared_ptr<IFeeRecordDao> feeDao,
                 std::shared_ptr<ISalaryRecordDao> salaryDao,
                 std::shared_ptr<IEnrollmentDao> enrollmentDao,
                 std::shared_ptr<ICourseResultDao> courseResultDao,
                 std::shared_ptr<IGeneralInputValidator> inputValidator,
                 std::shared_ptr<SessionContext> sessionContext);
    
    /**
     * @brief Hàm hủy ảo mặc định
     */
    ~AdminService() override = default;    /**
     * @brief Phê duyệt đăng ký tài khoản sinh viên
     * @param studentIdToApprove ID của sinh viên cần phê duyệt
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> approveStudentRegistration(const std::string& studentIdToApprove) override;
    
    /**
     * @brief Lấy danh sách sinh viên theo trạng thái đăng nhập
     * @param status Trạng thái đăng nhập cần lọc
     * @return Danh sách các đối tượng Student có trạng thái tương ứng, hoặc Error nếu thất bại
     */
    std::expected<std::vector<Student>, Error> getStudentsByStatus(LoginStatus status) const override;
    
    /**
     * @brief Thêm tài khoản sinh viên mới bởi quản trị viên
     * @param data Dữ liệu sinh viên mới
     * @return Đối tượng Student mới nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Student, Error> addStudentByAdmin(const NewStudentDataByAdmin& data) override;
    
    /**
     * @brief Xóa tài khoản sinh viên
     * @param studentId ID của sinh viên cần xóa
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> removeStudentAccount(const std::string& studentId) override;    /**
     * @brief Thêm tài khoản giáo viên mới bởi quản trị viên
     * @param data Dữ liệu giáo viên mới
     * @return Đối tượng Teacher mới nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<Teacher, Error> addTeacherByAdmin(const NewTeacherDataByAdmin& data) override;
    
    /**
     * @brief Xóa tài khoản giáo viên
     * @param teacherId ID của giáo viên cần xóa
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> removeTeacherAccount(const std::string& teacherId) override;    /**
     * @brief Đặt lại mật khẩu cho người dùng
     * @param userId ID của người dùng cần đặt lại mật khẩu
     * @param newPassword Mật khẩu mới
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> resetUserPassword(const std::string& userId, const std::string& newPassword) override;
    
    /**
     * @brief Vô hiệu hóa tài khoản người dùng
     * @param userId ID của người dùng cần vô hiệu hóa
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> disableUserAccount(const std::string& userId) override;
    
    /**
     * @brief Kích hoạt tài khoản người dùng
     * @param userId ID của người dùng cần kích hoạt
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> enableUserAccount(const std::string& userId) override;
    
    /**
     * @brief Kiểm tra xem người dùng hiện tại có phải là quản trị viên đã đăng nhập không
     * @return true nếu là quản trị viên đã đăng nhập, false nếu không phải
     */
    bool isAdminAuthenticated() const;
};

#endif // ADMINSERVICE_H