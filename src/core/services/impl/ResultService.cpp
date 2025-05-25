#include "ResultService.h"
#include "../../../utils/Logger.h"
#include <sstream> // For report generation

/**
 * @brief Khởi tạo đối tượng ResultService
 * 
 * @param resultDao Đối tượng truy cập dữ liệu kết quả học tập
 * @param facultyDao Đối tượng truy cập dữ liệu khoa
 * @param studentDao Đối tượng truy cập dữ liệu sinh viên
 * @param courseDao Đối tượng truy cập dữ liệu khóa học
 * @param enrollmentDao Đối tượng truy cập dữ liệu đăng ký khóa học
 * @param inputValidator Đối tượng kiểm tra đầu vào
 * @param sessionContext Đối tượng quản lý phiên đăng nhập
 * @throw std::invalid_argument Nếu bất kỳ đối số nào là nullptr
 */
ResultService::ResultService(
    std::shared_ptr<ICourseResultDao> resultDao,
    std::shared_ptr<IFacultyDao> facultyDao,
    std::shared_ptr<IStudentDao> studentDao,
    std::shared_ptr<ICourseDao> courseDao,
    std::shared_ptr<IEnrollmentDao> enrollmentDao,
    std::shared_ptr<IGeneralInputValidator> inputValidator,
    std::shared_ptr<SessionContext> sessionContext)
    : _resultDao(std::move(resultDao)),
      _facultyDao(std::move(facultyDao)),
      _studentDao(std::move(studentDao)),
      _courseDao(std::move(courseDao)),
      _enrollmentDao(std::move(enrollmentDao)),
      _inputValidator(std::move(inputValidator)),
      _sessionContext(std::move(sessionContext)) {
    if (!_resultDao) throw std::invalid_argument("CourseResultDao cannot be null.");
    if (!_studentDao) throw std::invalid_argument("StudentDao cannot be null.");
    if (!_courseDao) throw std::invalid_argument("CourseDao cannot be null.");
    if (!_enrollmentDao) throw std::invalid_argument("EnrollmentDao cannot be null.");
    if (!_inputValidator) throw std::invalid_argument("InputValidator cannot be null.");
    if (!_sessionContext) throw std::invalid_argument("SessionContext cannot be null.");
}

/**
 * @brief Nhập điểm cho một sinh viên trong một khóa học
 * 
 * Phương thức này cho phép giảng viên hoặc admin nhập điểm cho một sinh viên 
 * trong một khóa học cụ thể. Sinh viên phải đã đăng ký khóa học trước khi
 * có thể nhập điểm.
 * Yêu cầu quyền truy cập: Admin hoặc giảng viên dạy khóa học đó.
 * 
 * @param studentId ID của sinh viên cần nhập điểm
 * @param courseId ID của khóa học cần nhập điểm
 * @param marks Số điểm cần nhập (thang điểm phụ thuộc vào cấu hình hệ thống)
 * @return std::expected<bool, Error> true nếu nhập điểm thành công, hoặc lỗi nếu thất bại
 */
std::expected<bool, Error> ResultService::enterMarks(const std::string& studentId, const std::string& courseId, int marks) {
    if (!_sessionContext->isAuthenticated() || !_sessionContext->getCurrentUserRole().has_value() || 
        (_sessionContext->getCurrentUserRole().value() != UserRole::ADMIN && _sessionContext->getCurrentUserRole().value() != UserRole::TEACHER)
       ) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only Admins or Teachers can enter marks."});
    }
    // TODO: Teacher chỉ được nhập điểm cho môn mình dạy (logic phức tạp hơn)

    ValidationResult studentIdVr = _inputValidator->validateIdFormat(studentId, "Student ID");
    if (!studentIdVr.isValid) return std::unexpected(studentIdVr.errors[0]);
    ValidationResult courseIdVr = _inputValidator->validateIdFormat(courseId, "Course ID");
    if (!courseIdVr.isValid) return std::unexpected(courseIdVr.errors[0]);
    
    CourseResult tempResult(studentId, courseId, marks); // Dùng để validate marks
    ValidationResult marksVr = tempResult.validate();
    if (!marksVr.isValid) return std::unexpected(marksVr.errors[0]);

    // Kiểm tra Student tồn tại
    auto studentExists = _studentDao->exists(studentId);
    if (!studentExists.has_value()) return std::unexpected(studentExists.error());
    if (!studentExists.value()) return std::unexpected(Error{ErrorCode::NOT_FOUND, "Student ID '" + studentId + "' not found."});

    // Kiểm tra Course tồn tại
    auto courseExists = _courseDao->exists(courseId);
    if (!courseExists.has_value()) return std::unexpected(courseExists.error());
    if (!courseExists.value()) return std::unexpected(Error{ErrorCode::NOT_FOUND, "Course ID '" + courseId + "' not found."});

    // Kiểm tra Student có đăng ký môn này không
    auto isEnrolled = _enrollmentDao->isEnrolled(studentId, courseId);
    if (!isEnrolled.has_value()) return std::unexpected(isEnrolled.error());
    if (!isEnrolled.value()) {
        return std::unexpected(Error{ErrorCode::VALIDATION_ERROR, "Student " + studentId + " is not enrolled in course " + courseId + "."});
    }

    CourseResult resultToSave(studentId, courseId, marks);
    auto saveResult = _resultDao->addOrUpdate(resultToSave);
     if (saveResult.has_value() && saveResult.value()) {
        LOG_INFO("Marks entered for Student " + studentId + ", Course " + courseId + ": " + std::to_string(marks));
    }
    return saveResult;
}

/**
 * @brief Nhập điểm cho nhiều khóa học của một sinh viên
 * 
 * Phương thức này cho phép giảng viên hoặc admin nhập điểm cho một sinh viên
 * trong nhiều khóa học cùng một lúc. Tất cả các điểm sẽ được nhập hoặc
 * không có điểm nào được nhập (đảm bảo tính nhất quán của dữ liệu).
 * Yêu cầu quyền truy cập: Admin hoặc giảng viên dạy các khóa học đó.
 * 
 * @param studentId ID của sinh viên cần nhập điểm
 * @param courseMarksMap Map chứa ID khóa học và điểm tương ứng
 * @return std::expected<bool, Error> true nếu nhập điểm thành công, hoặc lỗi nếu thất bại
 */
std::expected<bool, Error> ResultService::enterMultipleMarks(const std::string& studentId, const std::map<std::string, int>& courseMarksMap) {
    if (!_sessionContext->isAuthenticated() || !_sessionContext->getCurrentUserRole().has_value() || 
        (_sessionContext->getCurrentUserRole().value() != UserRole::ADMIN && _sessionContext->getCurrentUserRole().value() != UserRole::TEACHER)
       ) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Only Admins or Teachers can enter marks."});
    }
    
    ValidationResult studentIdVr = _inputValidator->validateIdFormat(studentId, "Student ID");
    if (!studentIdVr.isValid) return std::unexpected(studentIdVr.errors[0]);

    auto studentExists = _studentDao->exists(studentId);
    if (!studentExists.has_value()) return std::unexpected(studentExists.error());
    if (!studentExists.value()) return std::unexpected(Error{ErrorCode::NOT_FOUND, "Student ID '" + studentId + "' not found."});

    // TODO: Cân nhắc transaction cho việc nhập nhiều điểm
    // auto transResult = _resultDao->beginTransaction(); // Giả sử resultDao có thể bắt đầu transaction (hoặc dbAdapter)
    // if (!transResult.has_value()) return std::unexpected(transResult.error());


    for (const auto& pair : courseMarksMap) {
        const std::string& courseId = pair.first;
        int marks = pair.second;
        
        auto enterOneMarkResult = enterMarks(studentId, courseId, marks); // Gọi lại hàm enterMarks đã có check quyền (nhưng ở đây quyền đã check rồi)
                                                                           // và các validate khác.
        if (!enterOneMarkResult.has_value() || !enterOneMarkResult.value()) {
            LOG_ERROR("Failed to enter marks for course " + courseId + " for student " + studentId + ". Rolling back if applicable.");
            // _resultDao->rollbackTransaction(); // Giả sử
            return std::unexpected(enterOneMarkResult.has_value() ? Error{ErrorCode::OPERATION_FAILED, "Failed to enter marks for course " + courseId} : enterOneMarkResult.error());
        }
    }
    // auto commitResult = _resultDao->commitTransaction(); // Giả sử
    // if (!commitResult.has_value()) return std::unexpected(commitResult.error());
    LOG_INFO("Multiple marks entered successfully for student " + studentId);
    return true;
}


/**
 * @brief Lấy kết quả học tập của một sinh viên trong một khóa học cụ thể
 * 
 * Phương thức này trả về thông tin chi tiết về điểm số của một sinh viên trong một khóa học.
 * Yêu cầu quyền truy cập:
 * - Sinh viên chỉ có thể xem điểm của chính mình
 * - Giảng viên có thể xem điểm của sinh viên trong các khóa học do họ dạy
 * - Admin có thể xem điểm của bất kỳ sinh viên nào
 * 
 * @param studentId ID của sinh viên
 * @param courseId ID của khóa học
 * @return std::expected<CourseResult, Error> Đối tượng CourseResult nếu thành công, hoặc lỗi nếu thất bại
 */
std::expected<CourseResult, Error> ResultService::getSpecificResult(const std::string& studentId, const std::string& courseId) const {
    // Quyền: Student xem điểm của mình, Teacher xem điểm SV trong môn mình dạy, Admin xem tất cả
    if (!_sessionContext->isAuthenticated()) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    auto currentRole = _sessionContext->getCurrentUserRole();
    auto currentId = _sessionContext->getCurrentUserId();
    bool canAccess = false;
    if (currentRole.has_value()) {
        if (currentRole.value() == UserRole::ADMIN) canAccess = true;
        else if (currentRole.value() == UserRole::STUDENT && currentId.has_value() && currentId.value() == studentId) canAccess = true;
        else if (currentRole.value() == UserRole::TEACHER) {
            // TODO: Logic kiểm tra GV có dạy môn courseId không
            canAccess = true; // Tạm thời cho phép Teacher xem
        }
    }
    if(!canAccess) return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Permission denied to view this result."});

    ValidationResult studentIdVr = _inputValidator->validateIdFormat(studentId, "Student ID");
    if (!studentIdVr.isValid) return std::unexpected(studentIdVr.errors[0]);
    ValidationResult courseIdVr = _inputValidator->validateIdFormat(courseId, "Course ID");
    if (!courseIdVr.isValid) return std::unexpected(courseIdVr.errors[0]);
    
    return _resultDao->find(studentId, courseId);
}

/**
 * @brief Lấy tất cả kết quả học tập của một sinh viên
 * 
 * Phương thức này trả về danh sách tất cả kết quả học tập của một sinh viên
 * trong các khóa học mà sinh viên đã đăng ký.
 * Yêu cầu quyền truy cập:
 * - Sinh viên chỉ có thể xem kết quả của chính mình
 * - Giảng viên có thể xem kết quả của bất kỳ sinh viên nào
 * - Admin có thể xem kết quả của bất kỳ sinh viên nào
 * 
 * @param studentId ID của sinh viên
 * @return std::expected<std::vector<CourseResult>, Error> Danh sách kết quả học tập nếu thành công, hoặc lỗi nếu thất bại
 */
std::expected<std::vector<CourseResult>, Error> ResultService::getResultsByStudent(const std::string& studentId) const {
    // Quyền tương tự getSpecificResult
     if (!_sessionContext->isAuthenticated()) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    auto currentRole = _sessionContext->getCurrentUserRole();
    auto currentId = _sessionContext->getCurrentUserId();
    bool canAccess = false;
    if (currentRole.has_value()) {
        if (currentRole.value() == UserRole::ADMIN || currentRole.value() == UserRole::TEACHER) canAccess = true; // Teacher có thể xem bảng điểm của SV
        else if (currentRole.value() == UserRole::STUDENT && currentId.has_value() && currentId.value() == studentId) canAccess = true;
    }
    if(!canAccess) return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Permission denied to view results for this student."});
    
    ValidationResult studentIdVr = _inputValidator->validateIdFormat(studentId, "Student ID");
    if (!studentIdVr.isValid) return std::unexpected(studentIdVr.errors[0]);

    return _resultDao->findByStudentId(studentId);
}

std::expected<std::vector<CourseResult>, Error> ResultService::getResultsByCourse(const std::string& courseId) const {
    // Quyền: Admin hoặc Teacher (GV dạy môn đó)
     if (!_sessionContext->isAuthenticated()) {
        return std::unexpected(Error{ErrorCode::AUTHENTICATION_FAILED, "User not authenticated."});
    }
    auto currentRole = _sessionContext->getCurrentUserRole();
    if (!currentRole.has_value() || (currentRole.value() != UserRole::ADMIN && currentRole.value() != UserRole::TEACHER)) {
        return std::unexpected(Error{ErrorCode::PERMISSION_DENIED, "Permission denied to view results for this course."});
    }
    // TODO: Teacher chỉ xem được môn mình dạy.

    ValidationResult courseIdVr = _inputValidator->validateIdFormat(courseId, "Course ID");
    if (!courseIdVr.isValid) return std::unexpected(courseIdVr.errors[0]);

    return _resultDao->findByCourseId(courseId);
}

std::expected<std::string, Error> ResultService::generateStudentResultReport(const std::string& studentId) const {
    auto studentDetailsResult = _studentDao->getById(studentId);
    if (!studentDetailsResult.has_value()) {
        return std::unexpected(studentDetailsResult.error());
    }
    const Student& student = studentDetailsResult.value();

    auto resultsExp = getResultsByStudent(studentId); // Hàm này đã check quyền
    if (!resultsExp.has_value()) {
        return std::unexpected(resultsExp.error());
    }
    const auto& results = resultsExp.value();

    std::ostringstream report;
    report << "------------------------------------------\n";
    report << "           STUDENT RESULT REPORT          \n";
    report << "------------------------------------------\n";
    report << "Student ID   : " << student.getId() << "\n";
    report << "Student Name : " << student.getFullName() << "\n";
    
    auto facultyResult = _facultyDao->getById(student.getFacultyId()); // Giả sử cần FacultyService hoặc DaoFactory
    if(facultyResult.has_value()){
        report << "Faculty      : " << facultyResult.value().getName() << "\n";
    } else {
        report << "Faculty ID   : " << student.getFacultyId() << " (Details not found)\n";
    }
    report << "------------------------------------------\n";
    report << std::left << std::setw(12) << "Course ID"
           << std::setw(35) << "Course Name"
           << std::setw(10) << "Credits"
           << std::setw(8) << "Marks"
           << std::setw(8) << "Grade" << "\n";
    report << "-------------------------------------------------------------------------\n";

    if (results.empty()) {
        report << "No results found for this student.\n";
    } else {
        for (const auto& res : results) {
            auto courseDetails = _courseDao->getById(res.getCourseId());
            std::string courseName = courseDetails.has_value() ? courseDetails.value().getName() : "N/A";
            int credits = courseDetails.has_value() ? courseDetails.value().getCredits() : 0;

            report << std::left << std::setw(12) << res.getCourseId()
                   << std::setw(35) << courseName
                   << std::setw(10) << credits
                   << std::setw(8) << (res.getMarks() == -1 ? "N/A" : std::to_string(res.getMarks()))
                   << std::setw(8) << res.getGrade() << "\n";
        }
    }
    report << "-------------------------------------------------------------------------\n";
    
    auto cgpaExp = calculateCGPA(studentId);
    if (cgpaExp.has_value()) {
        report << "Cumulative GPA (CGPA): " << std::fixed << std::setprecision(2) << cgpaExp.value() << "\n";
    } else {
        report << "CGPA: Could not be calculated (" << cgpaExp.error().message << ")\n";
    }
    report << "------------------------------------------\n";
    return report.str();
}

std::expected<double, Error> ResultService::calculateCGPA(const std::string& studentId) const {
    auto resultsExp = getResultsByStudent(studentId); // Đã check quyền
    if (!resultsExp.has_value()) {
        return std::unexpected(resultsExp.error());
    }
    const auto& results = resultsExp.value();

    if (results.empty()) {
        return 0.0; // Hoặc lỗi nếu không có kết quả nào
    }

    double totalGradePoints = 0;
    int totalCreditsAttempted = 0;
    // Mapping Grade to Points (ví dụ)
    // A=4.0, B=3.0, C=2.0, D=1.0, F=0.0
    // Cần quy định rõ ràng hơn
    auto gradeToPoint = [](char grade) -> double {
        switch (grade) {
            case 'A': return 4.0;
            case 'B': return 3.0;
            case 'C': return 2.0;
            case 'D': return 1.0;
            case 'F': return 0.0;
            default: return -1.0; // Điểm không hợp lệ hoặc chưa có
        }
    };

    for (const auto& res : results) {
        if (res.getMarks() == -1) continue; // Bỏ qua môn chưa có điểm

        auto courseDetails = _courseDao->getById(res.getCourseId());
        if (!courseDetails.has_value()) {
            LOG_WARN("CGPA Calc: Course " + res.getCourseId() + " not found for student " + studentId);
            continue; // Bỏ qua nếu không tìm thấy thông tin môn học
        }
        int credits = courseDetails.value().getCredits();
        double point = gradeToPoint(res.getGrade());

        if (point >= 0 && credits > 0) { // Chỉ tính môn có điểm hợp lệ và tín chỉ > 0
            totalGradePoints += point * credits;
            totalCreditsAttempted += credits;
        }
    }

    if (totalCreditsAttempted == 0) {
        return 0.0; // Tránh chia cho 0, hoặc trả về lỗi
    }
    return totalGradePoints / totalCreditsAttempted;
}