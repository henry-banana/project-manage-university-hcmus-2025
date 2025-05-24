#include "FinanceService.h"
#include "../../../utils/Logger.h"
#include <sstream>   // For receipt/certificate generation
#include <chrono>    // For date on receipt/certificate
#include <ctime>     // For date formatting

// Helper function for current date string
std::string getCurrentDateString() {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = {};
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&now_tm, &now_c);
    #else
        localtime_r(&now_c, &now_tm);
    #endif
    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%d/%m/%Y");
    return oss.str();
}


FinanceService::FinanceService(
    std::shared_ptr<IFeeRecordDao> feeDao,
    std::shared_ptr<ISalaryRecordDao> salaryDao,
    std::shared_ptr<IStudentDao> studentDao,
    std::shared_ptr<ITeacherDao> teacherDao,
    std::shared_ptr<IFacultyDao> facultyDao,
    std::shared_ptr<IGeneralInputValidator> inputValidator,
    std::shared_ptr<SessionContext> sessionContext)
    : _feeDao(std::move(feeDao)),
      _salaryDao(std::move(salaryDao)),
      _studentDao(std::move(studentDao)),
      _teacherDao(std::move(teacherDao)),
      _facultyDao(std::move(facultyDao)),
      _inputValidator(std::move(inputValidator)),
      _sessionContext(std::move(sessionContext)) {
    if (!_feeDao) throw std::invalid_argument("FeeRecordDao cannot be null.");
    if (!_salaryDao) throw std::invalid_argument("SalaryRecordDao cannot be null.");
    if (!_studentDao) throw std::invalid_argument("StudentDao cannot be null.");
    if (!_teacherDao) throw std::invalid_argument("TeacherDao cannot be null.");
    if (!_inputValidator) throw std::invalid_argument("InputValidator cannot be null.");
    if (!_sessionContext) throw std::invalid_argument("SessionContext cannot be null.");
}

// --- Fee Operations ---
std::expected<FeeRecord, Error> FinanceService::getStudentFeeRecord(const std::string& studentId) const {
    if (!_sessionContext->isAuthenticated()){
         return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    auto currentRole = _sessionContext->getCurrentUserRole();
    auto currentId = _sessionContext->getCurrentUserId();
    bool canAccess = false;
    if(currentRole.has_value()){
        if(currentRole.value() == UserRole::ADMIN) canAccess = true;
        else if (currentRole.value() == UserRole::STUDENT && currentId.has_value() && currentId.value() == studentId) canAccess = true;
    }
    if(!canAccess) return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Permission denied to view fee record."});

    ValidationResult idVr = _inputValidator->validateIdFormat(studentId, "Student ID");
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);
    return _feeDao->getById(studentId);
}

std::expected<bool, Error> FinanceService::makeFeePayment(const std::string& studentId, long amount) {
    if (!_sessionContext->isAuthenticated()){
         return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    // Chỉ Student tự thanh toán hoặc Admin/Finance Staff (chưa có role Finance Staff)
    auto currentRole = _sessionContext->getCurrentUserRole();
    auto currentId = _sessionContext->getCurrentUserId();
    if (!currentRole.has_value() || 
        (currentRole.value() != UserRole::ADMIN && 
         (currentRole.value() != UserRole::STUDENT || !currentId.has_value() || currentId.value() != studentId)
        )
       ) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Permission denied to make fee payment."});
    }

    ValidationResult idVr = _inputValidator->validateIdFormat(studentId, "Student ID");
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);
    if (amount <= 0) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Payment amount must be positive."});
    }

    auto feeRecordExp = _feeDao->getById(studentId);
    if (!feeRecordExp.has_value()) {
        return std::unexpected(feeRecordExp.error());
    }
    FeeRecord record = feeRecordExp.value();
    if (!record.makePayment(amount)) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Payment amount exceeds due fee or is invalid."});
    }
    
    auto updateResult = _feeDao->update(record);
    if (updateResult.has_value() && updateResult.value()) {
        LOG_INFO("Fee payment of " + std::to_string(amount) + " made for student " + studentId);
    }
    return updateResult;
}

std::expected<bool, Error> FinanceService::setStudentTotalFee(const std::string& studentId, long newTotalFee) {
    if (!_sessionContext->isAuthenticated() || !_sessionContext->getCurrentUserRole().has_value() || _sessionContext->getCurrentUserRole().value() != UserRole::ADMIN) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can set total fee."});
    }
    ValidationResult idVr = _inputValidator->validateIdFormat(studentId, "Student ID");
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);
    if (newTotalFee < 0) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Total fee cannot be negative."});
    }

    auto feeRecordExp = _feeDao->getById(studentId);
    if (!feeRecordExp.has_value()) {
        // Nếu chưa có record, tạo mới
        FeeRecord newRecord(studentId, newTotalFee);
        return _feeDao->add(newRecord).has_value(); // true if add successful
    }
    FeeRecord record = feeRecordExp.value();
    if (!record.setTotalFee(newTotalFee)) { // setTotalFee có thể fail nếu paidFee > newTotalFee
         return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Could not set total fee (e.g., new total fee is less than already paid amount)."});
    }
    
    auto updateResult = _feeDao->update(record);
    if (updateResult.has_value() && updateResult.value()) {
        LOG_INFO("Total fee for student " + studentId + " set to " + std::to_string(newTotalFee));
    }
    return updateResult;
}

std::expected<bool, Error> FinanceService::createInitialFeeRecord(const std::string& studentId, long initialTotalFee) {
     if (!_sessionContext->isAuthenticated() || !_sessionContext->getCurrentUserRole().has_value() || _sessionContext->getCurrentUserRole().value() != UserRole::ADMIN) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can create initial fee records."});
    }
    ValidationResult idVr = _inputValidator->validateIdFormat(studentId, "Student ID");
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);
    if (initialTotalFee < 0) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Initial total fee cannot be negative."});
    }

    auto studentExists = _studentDao->exists(studentId);
    if(!studentExists.has_value()) return std::unexpected(studentExists.error());
    if(!studentExists.value()) return std::unexpected(Error{ErrorCode::NOT_FOUND, "Student " + studentId + " not found to create fee record."});

    auto feeRecordExists = _feeDao->exists(studentId);
    if(!feeRecordExists.has_value()) return std::unexpected(feeRecordExists.error());
    if(feeRecordExists.value()) {
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Fee record for student " + studentId + " already exists."});
    }

    FeeRecord newRecord(studentId, initialTotalFee, 0);
    auto addResult = _feeDao->add(newRecord);
    if (addResult.has_value()) {
        LOG_INFO("Initial fee record created for student " + studentId + " with total fee " + std::to_string(initialTotalFee));
        return true;
    }
    return std::unexpected(addResult.error());
}

std::expected<std::string, Error> FinanceService::generateFeeReceipt(const std::string& studentId, long paymentAmount) const {
    auto studentExp = _studentDao->getById(studentId);
    if(!studentExp.has_value()) return std::unexpected(studentExp.error());
    const Student& student = studentExp.value();

    auto feeRecordExp = getStudentFeeRecord(studentId); // Check quyền bên trong
    if(!feeRecordExp.has_value()) return std::unexpected(feeRecordExp.error());
    const FeeRecord& record = feeRecordExp.value();

    std::ostringstream receipt;
    receipt << "========================================\n";
    receipt << "          TUITION FEE RECEIPT           \n";
    receipt << "========================================\n";
    receipt << "Date: " << getCurrentDateString() << "\n\n";
    receipt << "Student ID   : " << student.getId() << "\n";
    receipt << "Student Name : " << student.getFullName() << "\n";
    receipt << "----------------------------------------\n";
    receipt << "Payment Amount : " << paymentAmount << " VND\n";
    receipt << "----------------------------------------\n";
    receipt << "Total Fee      : " << record.getTotalFee() << " VND\n";
    receipt << "Paid Fee       : " << record.getPaidFee() << " VND (after this payment)\n";
    receipt << "Due Fee        : " << record.getDueFee() << " VND\n";
    receipt << "========================================\n";
    receipt << "Thank you!\n";
    return receipt.str();
}


// --- Salary Operations ---
std::expected<SalaryRecord, Error> FinanceService::getTeacherSalaryRecord(const std::string& teacherId) const {
    if (!_sessionContext->isAuthenticated()){
         return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    auto currentRole = _sessionContext->getCurrentUserRole();
    auto currentId = _sessionContext->getCurrentUserId();
    bool canAccess = false;
    if(currentRole.has_value()){
        if(currentRole.value() == UserRole::ADMIN) canAccess = true;
        else if (currentRole.value() == UserRole::TEACHER && currentId.has_value() && currentId.value() == teacherId) canAccess = true;
    }
    if(!canAccess) return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Permission denied to view salary record."});

    ValidationResult idVr = _inputValidator->validateIdFormat(teacherId, "Teacher ID");
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);
    return _salaryDao->getById(teacherId);
}

std::expected<bool, Error> FinanceService::setTeacherBasicSalary(const std::string& teacherId, long newBasicMonthlyPay) {
    if (!_sessionContext->isAuthenticated() || !_sessionContext->getCurrentUserRole().has_value() || _sessionContext->getCurrentUserRole().value() != UserRole::ADMIN) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can set teacher salary."});
    }
    ValidationResult idVr = _inputValidator->validateIdFormat(teacherId, "Teacher ID");
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);
    if (newBasicMonthlyPay < 0) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Basic monthly pay cannot be negative."});
    }

    auto salaryRecordExp = _salaryDao->getById(teacherId);
    if (!salaryRecordExp.has_value()) {
        // Nếu chưa có, tạo mới
        SalaryRecord newRecord(teacherId, newBasicMonthlyPay);
        return _salaryDao->add(newRecord).has_value();
    }
    SalaryRecord record = salaryRecordExp.value();
    record.setBasicMonthlyPay(newBasicMonthlyPay);
    
    auto updateResult = _salaryDao->update(record);
    if (updateResult.has_value() && updateResult.value()) {
        LOG_INFO("Basic salary for teacher " + teacherId + " set to " + std::to_string(newBasicMonthlyPay));
    }
    return updateResult;
}

std::expected<bool, Error> FinanceService::createInitialSalaryRecord(const std::string& teacherId, long initialBasicPay) {
    if (!_sessionContext->isAuthenticated() || !_sessionContext->getCurrentUserRole().has_value() || _sessionContext->getCurrentUserRole().value() != UserRole::ADMIN) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only admins can create initial salary records."});
    }
    ValidationResult idVr = _inputValidator->validateIdFormat(teacherId, "Teacher ID");
    if (!idVr.isValid) return std::unexpected(idVr.errors[0]);
     if (initialBasicPay < 0) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Initial basic pay cannot be negative."});
    }

    auto teacherExists = _teacherDao->exists(teacherId);
    if(!teacherExists.has_value()) return std::unexpected(teacherExists.error());
    if(!teacherExists.value()) return std::unexpected(Error{ErrorCode::NOT_FOUND, "Teacher " + teacherId + " not found to create salary record."});

    auto salaryRecordExists = _salaryDao->exists(teacherId);
    if(!salaryRecordExists.has_value()) return std::unexpected(salaryRecordExists.error());
    if(salaryRecordExists.value()) {
        return std::unexpected(Error{ErrorCode::ALREADY_EXISTS, "Salary record for teacher " + teacherId + " already exists."});
    }

    SalaryRecord newRecord(teacherId, initialBasicPay);
    auto addResult = _salaryDao->add(newRecord);
    if (addResult.has_value()) {
        LOG_INFO("Initial salary record created for teacher " + teacherId + " with basic pay " + std::to_string(initialBasicPay));
        return true;
    }
    return std::unexpected(addResult.error());
}

std::expected<std::string, Error> FinanceService::generateSalaryCertificate(const std::string& teacherId) const {
    auto teacherExp = _teacherDao->getById(teacherId);
    if(!teacherExp.has_value()) return std::unexpected(teacherExp.error());
    const Teacher& teacher = teacherExp.value();

    auto salaryRecordExp = getTeacherSalaryRecord(teacherId); // Check quyền bên trong
    if(!salaryRecordExp.has_value()) return std::unexpected(salaryRecordExp.error());
    const SalaryRecord& record = salaryRecordExp.value();

    std::ostringstream cert;
    cert << "========================================\n";
    cert << "         SALARY CERTIFICATE             \n";
    cert << "========================================\n";
    cert << "Date: " << getCurrentDateString() << "\n\n";
    cert << "This is to certify that Mr./Ms. " << teacher.getFullName() << " (ID: " << teacher.getId() << ")\n";
    cert << "is employed as " << teacher.getDesignation() << " in the faculty of ";
    
    auto facultyExp = _facultyDao->getById(teacher.getFacultyId());
    if(facultyExp.has_value()){
        cert << facultyExp.value().getName();
    } else {
        cert << teacher.getFacultyId() << " (Faculty details not found)";
    }
    cert << ".\n\n";
    cert << "Current Basic Monthly Salary: " << record.getBasicMonthlyPay() << " VND.\n";
    cert << "========================================\n";
    cert << "This certificate is issued upon request.\n";
    cert << "Head of Finance Department\n"; // Hoặc người có thẩm quyền
    return cert.str();
}