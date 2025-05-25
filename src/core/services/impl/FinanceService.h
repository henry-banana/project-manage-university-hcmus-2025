/**
 * @file FinanceService.h
 * @brief Triển khai dịch vụ quản lý tài chính
 * 
 * FinanceService triển khai các chức năng quản lý tài chính theo giao diện
 * IFinanceService, cung cấp các phương thức để quản lý học phí và lương của giáo viên.
 */
#ifndef FINANCESERVICE_H
#define FINANCESERVICE_H

#include "../interface/IFinanceService.h"
#include "../../data_access/interface/IFeeRecordDao.h"
#include "../../data_access/interface/ISalaryRecordDao.h"
#include "../../data_access/interface/IStudentDao.h" // Để lấy thông tin SV cho receipt
#include "../../data_access/interface/ITeacherDao.h" // Để lấy thông tin GV cho certificate
#include "../../data_access/interface/IFacultyDao.h" // Để lấy thông tin khoa cho certificate
#include "../../validators/interface/IValidator.h"
#include "../SessionContext.h"
#include <iomanip> // For setprecision in receipt/certificate

/**
 * @class FinanceService
 * @brief Lớp triển khai dịch vụ quản lý tài chính
 * 
 * Lớp này triển khai các phương thức của giao diện IFinanceService để
 * cung cấp các chức năng quản lý học phí và lương của giáo viên.
 */
class FinanceService : public IFinanceService {
private:
    std::shared_ptr<IFeeRecordDao> _feeDao;           ///< Đối tượng dao để truy cập dữ liệu học phí
    std::shared_ptr<ISalaryRecordDao> _salaryDao;     ///< Đối tượng dao để truy cập dữ liệu lương
    std::shared_ptr<IStudentDao> _studentDao;         ///< Đối tượng dao để truy cập dữ liệu sinh viên
    std::shared_ptr<ITeacherDao> _teacherDao;         ///< Đối tượng dao để truy cập dữ liệu giáo viên
    std::shared_ptr<IFacultyDao> _facultyDao;         ///< Đối tượng dao để truy cập dữ liệu khoa
    std::shared_ptr<IGeneralInputValidator> _inputValidator; ///< Đối tượng validator để kiểm tra đầu vào
    std::shared_ptr<SessionContext> _sessionContext;  ///< Đối tượng quản lý phiên làm việc

public:
    /**
     * @brief Hàm khởi tạo FinanceService
     * @param feeDao Đối tượng dao để truy cập dữ liệu học phí
     * @param salaryDao Đối tượng dao để truy cập dữ liệu lương
     * @param studentDao Đối tượng dao để truy cập dữ liệu sinh viên
     * @param teacherDao Đối tượng dao để truy cập dữ liệu giáo viên
     * @param facultyDao Đối tượng dao để truy cập dữ liệu khoa
     * @param inputValidator Đối tượng validator để kiểm tra đầu vào
     * @param sessionContext Đối tượng quản lý phiên làm việc
     */
    FinanceService(std::shared_ptr<IFeeRecordDao> feeDao,
                   std::shared_ptr<ISalaryRecordDao> salaryDao,
                   std::shared_ptr<IStudentDao> studentDao,
                   std::shared_ptr<ITeacherDao> teacherDao,
                   std::shared_ptr<IFacultyDao> facultyDao,
                   std::shared_ptr<IGeneralInputValidator> inputValidator,
                   std::shared_ptr<SessionContext> sessionContext);
    
    /**
     * @brief Hàm hủy ảo mặc định
     */
    ~FinanceService() override = default;    /**
     * @brief Lấy thông tin học phí của sinh viên
     * @param studentId ID của sinh viên
     * @return Đối tượng FeeRecord nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<FeeRecord, Error> getStudentFeeRecord(const std::string& studentId) const override;
    
    /**
     * @brief Thực hiện thanh toán học phí cho sinh viên
     * @param studentId ID của sinh viên
     * @param amount Số tiền thanh toán
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> makeFeePayment(const std::string& studentId, long amount) override;
    
    /**
     * @brief Thiết lập tổng học phí mới cho sinh viên
     * @param studentId ID của sinh viên
     * @param newTotalFee Tổng học phí mới
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> setStudentTotalFee(const std::string& studentId, long newTotalFee) override;
    
    /**
     * @brief Tạo bản ghi học phí ban đầu cho sinh viên
     * @param studentId ID của sinh viên
     * @param initialTotalFee Tổng học phí ban đầu
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> createInitialFeeRecord(const std::string& studentId, long initialTotalFee) override;
    
    /**
     * @brief Tạo biên lai thanh toán học phí
     * @param studentId ID của sinh viên
     * @param paymentAmount Số tiền thanh toán
     * @return Chuỗi biên lai nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::string, Error> generateFeeReceipt(const std::string& studentId, long paymentAmount) const override;    /**
     * @brief Lấy thông tin lương của giáo viên
     * @param teacherId ID của giáo viên
     * @return Đối tượng SalaryRecord nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<SalaryRecord, Error> getTeacherSalaryRecord(const std::string& teacherId) const override;
    
    /**
     * @brief Thiết lập lương cơ bản mới cho giáo viên
     * @param teacherId ID của giáo viên
     * @param newBasicMonthlyPay Lương cơ bản hàng tháng mới
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> setTeacherBasicSalary(const std::string& teacherId, long newBasicMonthlyPay) override;
    
    /**
     * @brief Tạo bản ghi lương ban đầu cho giáo viên
     * @param teacherId ID của giáo viên
     * @param initialBasicPay Lương cơ bản ban đầu
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<bool, Error> createInitialSalaryRecord(const std::string& teacherId, long initialBasicPay) override;
    
    /**
     * @brief Tạo chứng từ lương cho giáo viên
     * @param teacherId ID của giáo viên
     * @return Chuỗi chứng từ lương nếu thành công, hoặc Error nếu thất bại
     */
    std::expected<std::string, Error> generateSalaryCertificate(const std::string& teacherId) const override;
};

#endif // FINANCESERVICE_H