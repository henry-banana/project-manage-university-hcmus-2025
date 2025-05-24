/**
 * @file ConsoleUI.h
 * @brief Định nghĩa lớp giao diện người dùng dòng lệnh
 * 
 * Lớp ConsoleUI triển khai giao diện người dùng dựa trên console, quản lý
 * các menu, trạng thái, và tương tác giữa người dùng và các dịch vụ của
 * hệ thống quản lý trường đại học.
 */
#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <variant> // (➕) Cho UIState

// Service Interfaces
#include "../core/services/interface/IAuthService.h"
#include "../core/services/interface/IStudentService.h"
#include "../core/services/interface/ITeacherService.h"
#include "../core/services/interface/IFacultyService.h"
#include "../core/services/interface/ICourseService.h"
#include "../core/services/interface/IEnrollmentService.h"
#include "../core/services/interface/IResultService.h"
#include "../core/services/interface/IFinanceService.h"
#include "../core/services/interface/IAdminService.h"

// UI Helpers
#include "view_helpers/InputPrompter.h"
#include "view_helpers/TableDisplayer.h"
#include "view_helpers/MenuRenderer.h" // Đã có MenuItemDisplay struct

// Entities (cần cho việc hiển thị và nhận input)
#include "../core/entities/Student.h"
#include "../core/entities/Teacher.h"
#include "../core/entities/Faculty.h"
#include "../core/entities/Course.h"
#include "../core/entities/CourseResult.h"
#include "../core/entities/FeeRecord.h"
#include "../core/entities/SalaryRecord.h"

/**
 * @brief Trạng thái khi chưa đăng nhập (hiển thị Main Menu cho khách)
 */
struct UnauthenticatedState {}; 

/**
 * @brief Trạng thái đang ở màn hình nhập login
 */
struct LoginPromptState {};    

/**
 * @brief Trạng thái đang ở màn hình đăng ký sinh viên
 */
struct StudentRegistrationPromptState {}; 

/**
 * @brief Trạng thái đang ở màn hình đổi mật khẩu
 */
struct ChangePasswordPromptState {}; 

/**
 * @brief Trạng thái đang ở menu chính của Admin
 */
struct AdminPanelState {};     

/**
 * @brief Trạng thái đang ở menu chính của Student
 */
struct StudentPanelState {};   

/**
 * @brief Trạng thái đang ở menu chính của Teacher
 */
struct TeacherPanelState {};   

/**
 * @brief Trạng thái đang ở menu quản lý sinh viên của Admin
 */
struct AdminStudentManagementState {};

/**
 * @brief Trạng thái đang ở menu quản lý giảng viên của Admin
 */
struct AdminTeacherManagementState {};

/**
 * @brief Trạng thái đang ở menu quản lý khoa của Admin
 */
struct AdminFacultyManagementState {};

/**
 * @brief Trạng thái đang ở menu quản lý khóa học của Admin
 */
struct AdminCourseManagementState {};

/**
 * @brief Trạng thái đang ở menu quản lý tài khoản của Admin
 */
struct AdminAccountManagementState {};

// (➕) UIState là một variant của tất cả các trạng thái có thể có
/**
 * @brief UIState là một variant của tất cả các trạng thái có thể có
 * 
 * Sử dụng std::variant để quản lý trạng thái UI trong một biến duy nhất
 * giúp quản lý chu trình sống của ứng dụng dễ dàng hơn.
 */
using UIState = std::variant<
    UnauthenticatedState,
    LoginPromptState,
    StudentRegistrationPromptState,
    ChangePasswordPromptState,
    AdminPanelState,
    StudentPanelState,
    TeacherPanelState,
    AdminStudentManagementState,
    AdminTeacherManagementState,
    AdminFacultyManagementState,
    AdminCourseManagementState,
    AdminAccountManagementState
    // Thêm các state khác nếu cần
>;

/**
 * @class ConsoleUI
 * @brief Lớp quản lý giao diện người dùng dựa trên console
 * 
 * ConsoleUI là lớp chính quản lý tương tác với người dùng thông qua console.
 * Nó sử dụng mô hình máy trạng thái (state machine) để chuyển đổi giữa các màn hình
 * và xử lý các hành động người dùng tương ứng với từng trạng thái.
 */
class ConsoleUI {
private:
    // Services
    std::shared_ptr<IAuthService> _authService; /**< Dịch vụ xác thực người dùng */
    std::shared_ptr<IStudentService> _studentService; /**< Dịch vụ quản lý sinh viên */
    std::shared_ptr<ITeacherService> _teacherService; /**< Dịch vụ quản lý giảng viên */
    std::shared_ptr<IFacultyService> _facultyService; /**< Dịch vụ quản lý khoa */
    std::shared_ptr<ICourseService> _courseService; /**< Dịch vụ quản lý khóa học */
    std::shared_ptr<IEnrollmentService> _enrollmentService; /**< Dịch vụ đăng ký khóa học */
    std::shared_ptr<IResultService> _resultService; /**< Dịch vụ quản lý kết quả học tập */
    std::shared_ptr<IFinanceService> _financeService; /**< Dịch vụ quản lý tài chính */
    std::shared_ptr<IAdminService> _adminService; /**< Dịch vụ quản trị hệ thống */

    // UI Helpers
    std::unique_ptr<InputPrompter> _prompter; /**< Tiện ích nhập liệu từ người dùng */
    std::unique_ptr<TableDisplayer> _displayer; /**< Tiện ích hiển thị dữ liệu dạng bảng */
    std::unique_ptr<MenuRenderer> _menuRenderer; /**< Tiện ích hiển thị menu */

    bool _isRunning; /**< Cờ báo hiệu ứng dụng đang chạy */
    UIState _currentState; /**< Trạng thái UI hiện tại */

    /**
     * @brief Xử lý trạng thái hiện tại của UI
     * 
     * Phương thức này sử dụng std::visit để gọi hàm xử lý tương ứng với trạng thái hiện tại.
     */
    void processCurrentState();
    
    /**
     * @brief Xử lý trạng thái UnauthenticatedState - màn hình chính khi chưa đăng nhập
     * @param state Tham chiếu đến đối tượng trạng thái
     */
    void handleState(UnauthenticatedState& state);
    
    /**
     * @brief Xử lý trạng thái LoginPromptState - màn hình đăng nhập
     * @param state Tham chiếu đến đối tượng trạng thái
     */
    void handleState(LoginPromptState& state);
    
    /**
     * @brief Xử lý trạng thái StudentRegistrationPromptState - màn hình đăng ký sinh viên
     * @param state Tham chiếu đến đối tượng trạng thái
     */
    void handleState(StudentRegistrationPromptState& state);
    
    /**
     * @brief Xử lý trạng thái ChangePasswordPromptState - màn hình đổi mật khẩu
     * @param state Tham chiếu đến đối tượng trạng thái
     */
    void handleState(ChangePasswordPromptState& state);
    
    /**
     * @brief Xử lý trạng thái AdminPanelState - menu chính của Admin
     * @param state Tham chiếu đến đối tượng trạng thái
     */
    void handleState(AdminPanelState& state);
    
    /**
     * @brief Xử lý trạng thái StudentPanelState - menu chính của Sinh viên
     * @param state Tham chiếu đến đối tượng trạng thái
     */
    void handleState(StudentPanelState& state);
    
    /**
     * @brief Xử lý trạng thái TeacherPanelState - menu chính của Giảng viên
     * @param state Tham chiếu đến đối tượng trạng thái
     */
    void handleState(TeacherPanelState& state);
    
    /**
     * @brief Xử lý trạng thái AdminStudentManagementState - menu quản lý sinh viên
     * @param state Tham chiếu đến đối tượng trạng thái
     */
    void handleState(AdminStudentManagementState& state);
    
    /**
     * @brief Xử lý trạng thái AdminTeacherManagementState - menu quản lý giảng viên
     * @param state Tham chiếu đến đối tượng trạng thái
     */
    void handleState(AdminTeacherManagementState& state);
    
    /**
     * @brief Xử lý trạng thái AdminFacultyManagementState - menu quản lý khoa
     * @param state Tham chiếu đến đối tượng trạng thái
     */
    void handleState(AdminFacultyManagementState& state);
    
    /**
     * @brief Xử lý trạng thái AdminCourseManagementState - menu quản lý khóa học
     * @param state Tham chiếu đến đối tượng trạng thái
     */
    void handleState(AdminCourseManagementState& state);
    
    /**
     * @brief Xử lý trạng thái AdminAccountManagementState - menu quản lý tài khoản
     * @param state Tham chiếu đến đối tượng trạng thái
     */
    void handleState(AdminAccountManagementState& state);


    /**
     * @brief Hiển thị và xử lý menu
     * 
     * @param title Tiêu đề của menu
     * @param items Danh sách các mục menu
     * @param actions Danh sách các hàm xử lý tương ứng với từng mục
     * @param isSubMenu Cờ đánh dấu đây là menu con hay không
     */
    void processMenu(const std::string& title, const std::vector<MenuItemDisplay>& items, 
                     const std::vector<std::function<void()>>& actions, bool isSubMenu = false);

    // --- Action Implementations (logic nghiệp vụ cụ thể, được gọi từ State Handlers) ---
    /**
     * @brief Xử lý đăng nhập
     */
    void doLogin();
    
    /**
     * @brief Xử lý đăng ký sinh viên mới
     */
    void doStudentRegistration();
    
    /**
     * @brief Xử lý đổi mật khẩu
     */
    void doChangePassword();
    
    /**
     * @brief Xử lý đăng xuất
     */
    void doLogout();
    
    /**
     * @brief Thoát khỏi ứng dụng
     */
    void doExitApplication();

    // --- Admin Actions ---
    /**
     * @brief Xử lý phê duyệt đăng ký sinh viên
     */
    void doAdminApproveRegistration();
    
    /**
     * @brief Xem danh sách sinh viên theo trạng thái
     */
    void doAdminViewStudentsByStatus();
    
    /**
     * @brief Thêm sinh viên mới vào hệ thống
     */
    void doAdminAddStudent();
    
    /**
     * @brief Cập nhật thông tin sinh viên
     */
    void doAdminUpdateStudent();
    
    /**
     * @brief Xóa sinh viên khỏi hệ thống
     */
    void doAdminRemoveStudent();
    
    /**
     * @brief Xem danh sách tất cả sinh viên
     */
    void doAdminViewAllStudents();
    
    /**
     * @brief Tìm kiếm sinh viên theo ID
     */
    void doAdminFindStudentById();

    /**
     * @brief Thêm giảng viên mới vào hệ thống
     */
    void doAdminAddTeacher();
    
    /**
     * @brief Cập nhật thông tin giảng viên
     */
    void doAdminUpdateTeacher();
    
    /**
     * @brief Xóa giảng viên khỏi hệ thống
     */
    void doAdminRemoveTeacher();
    
    /**
     * @brief Xem danh sách tất cả giảng viên
     */
    void doAdminViewAllTeachers();
    
    /**
     * @brief Tìm kiếm giảng viên theo ID
     */
    void doAdminFindTeacherById();

    /**
     * @brief Thêm khoa mới vào hệ thống
     */
    void doAdminAddFaculty();
    
    /**
     * @brief Cập nhật thông tin khoa
     */
    void doAdminUpdateFaculty();
    
    /**
     * @brief Xóa khoa khỏi hệ thống
     */
    void doAdminRemoveFaculty();
    
    /**
     * @brief Xem danh sách tất cả các khoa
     */
    void doAdminViewAllFaculties();
    
    /**
     * @brief Tìm kiếm khoa theo ID hoặc tên
     */
    void doAdminFindFaculty();
    
    /**
     * @brief Thêm khóa học mới vào hệ thống
     */
    void doAdminAddCourse();
    
    /**
     * @brief Cập nhật thông tin khóa học
     */
    void doAdminUpdateCourse();
    
    /**
     * @brief Xóa khóa học khỏi hệ thống
     */
    void doAdminRemoveCourse();
    
    /**
     * @brief Xem danh sách tất cả khóa học
     */
    void doAdminViewAllCourses();
    
    /**
     * @brief Tìm kiếm khóa học theo ID
     */
    void doAdminFindCourseById();

    /**
     * @brief Đặt lại mật khẩu cho người dùng
     */
    void doAdminResetUserPassword();
    
    /**
     * @brief Vô hiệu hóa tài khoản người dùng
     */
    void doAdminDisableUserAccount();
    
    /**
     * @brief Kích hoạt tài khoản người dùng
     */
    void doAdminEnableUserAccount();

    // --- Student Actions ---
    /**
     * @brief Xem thông tin chi tiết của sinh viên đang đăng nhập
     */
    void doStudentViewDetails();
    
    /**
     * @brief Xem kết quả học tập của sinh viên đang đăng nhập
     */
    void doStudentViewResults();
    
    /**
     * @brief Đăng ký khóa học cho sinh viên đang đăng nhập
     */
    void doStudentEnrollCourse();
    
    /**
     * @brief Hủy đăng ký khóa học của sinh viên đang đăng nhập
     */
    void doStudentDropCourse();
    
    /**
     * @brief Xem hồ sơ học phí của sinh viên đang đăng nhập
     */
    void doStudentViewFeeRecord();
    
    /**
     * @brief Thực hiện thanh toán học phí
     */
    void doStudentMakeFeePayment();

    // --- Teacher Actions ---
    /**
     * @brief Xem thông tin chi tiết của giảng viên đang đăng nhập
     */
    void doTeacherViewDetails();
    
    /**
     * @brief Xem danh sách khóa học mà giảng viên đang phụ trách
     */
    void doTeacherViewCourses();
    
    /**
     * @brief Nhập điểm cho sinh viên trong khóa học do giảng viên phụ trách
     */
    void doTeacherEnterMarks();
    
    /**
     * @brief Xem thông tin lương của giảng viên đang đăng nhập
     */
    void doTeacherViewSalary();
    
    /**
     * @brief Xem danh sách sinh viên đã đăng ký vào khóa học do giảng viên phụ trách
     */
    void doTeacherViewEnrolledStudents();


    // --- Data Display Helpers ---
    /**
     * @brief Hiển thị thông tin chi tiết của sinh viên
     * @param student Đối tượng sinh viên cần hiển thị
     */
    void displayStudentDetails(const Student& student);
    
    /**
     * @brief Hiển thị danh sách sinh viên dưới dạng bảng
     * @param students Danh sách các đối tượng sinh viên
     */
    void displayStudentsList(const std::vector<Student>& students);
    
    /**
     * @brief Hiển thị thông tin chi tiết của giảng viên
     * @param teacher Đối tượng giảng viên cần hiển thị
     */
    void displayTeacherDetails(const Teacher& teacher);
    
    /**
     * @brief Hiển thị danh sách giảng viên dưới dạng bảng
     * @param teachers Danh sách các đối tượng giảng viên
     */
    void displayTeachersList(const std::vector<Teacher>& teachers);
    
    /**
     * @brief Hiển thị thông tin chi tiết của khoa
     * @param faculty Đối tượng khoa cần hiển thị
     */
    void displayFacultyDetails(const Faculty& faculty);
    
    /**
     * @brief Hiển thị danh sách khoa dưới dạng bảng
     * @param faculties Danh sách các đối tượng khoa
     */
    void displayFacultiesList(const std::vector<Faculty>& faculties);
    
    /**
     * @brief Hiển thị thông tin chi tiết của khóa học
     * @param course Đối tượng khóa học cần hiển thị
     */
    void displayCourseDetails(const Course& course);
    
    /**
     * @brief Hiển thị danh sách khóa học dưới dạng bảng
     * @param courses Danh sách các đối tượng khóa học
     */
    void displayCoursesList(const std::vector<Course>& courses);
    
    /**
     * @brief Hiển thị thông tin kết quả học tập của một sinh viên trong một khóa học
     * @param result Đối tượng kết quả học tập
     * @param courseName Tên khóa học (tùy chọn)
     */
    void displayCourseResultEntry(const CourseResult& result, const std::string& courseName = "");
    
    /**
     * @brief Hiển thị danh sách kết quả học tập dưới dạng bảng
     * @param results Danh sách các đối tượng kết quả học tập
     */
    void displayCourseResultsList(const std::vector<CourseResult>& results);
    
    /**
     * @brief Hiển thị thông tin chi tiết về học phí
     * @param record Đối tượng hồ sơ học phí
     */
    void displayFeeRecordDetails(const FeeRecord& record);
    
    /**
     * @brief Hiển thị thông tin chi tiết về lương
     * @param record Đối tượng hồ sơ lương
     */
    void displaySalaryRecordDetails(const SalaryRecord& record);

    // --- Input Helpers ---
    /**
     * @brief Nhắc người dùng nhập dữ liệu đăng ký sinh viên
     * @return Dữ liệu đăng ký sinh viên đã nhập
     */
    StudentRegistrationData promptForStudentRegistrationData();
    
    /**
     * @brief Nhắc người dùng nhập dữ liệu sinh viên mới (do admin thêm)
     * @return Dữ liệu sinh viên mới đã nhập
     */
    NewStudentDataByAdmin promptForNewStudentDataByAdmin();
    
    /**
     * @brief Nhắc người dùng nhập dữ liệu giảng viên mới (do admin thêm)
     * @return Dữ liệu giảng viên mới đã nhập
     */
    NewTeacherDataByAdmin promptForNewTeacherDataByAdmin();
    
    // --- Utility ---
    /**
     * @brief Hiển thị thông báo lỗi đơn giản
     * @param message Nội dung thông báo lỗi
     */
    void showErrorMessage(const std::string& message);
    
    /**
     * @brief Hiển thị đối tượng lỗi
     * @param error Đối tượng lỗi cần hiển thị
     */
    void showErrorMessage(const Error& error);
    
    /**
     * @brief Hiển thị thông báo thành công
     * @param message Nội dung thông báo
     */
    void showSuccessMessage(const std::string& message);
    
    /**
     * @brief Xóa màn hình và tạm dừng chờ người dùng nhấn Enter
     * @param message Thông báo hiển thị trước khi tạm dừng
     */
    void clearAndPause(const std::string& message = "\nPress Enter to continue...");

public:
    /**
     * @brief Constructor với đầy đủ các dependency
     * 
     * @param authService Dịch vụ xác thực
     * @param studentService Dịch vụ quản lý sinh viên
     * @param teacherService Dịch vụ quản lý giảng viên
     * @param facultyService Dịch vụ quản lý khoa
     * @param courseService Dịch vụ quản lý khóa học
     * @param enrollmentService Dịch vụ đăng ký khóa học
     * @param resultService Dịch vụ quản lý kết quả học tập
     * @param financeService Dịch vụ quản lý tài chính
     * @param adminService Dịch vụ quản trị hệ thống
     */
    ConsoleUI(
        std::shared_ptr<IAuthService> authService,
        std::shared_ptr<IStudentService> studentService,
        std::shared_ptr<ITeacherService> teacherService,
        std::shared_ptr<IFacultyService> facultyService,
        std::shared_ptr<ICourseService> courseService,
        std::shared_ptr<IEnrollmentService> enrollmentService,
        std::shared_ptr<IResultService> resultService,
        std::shared_ptr<IFinanceService> financeService,
        std::shared_ptr<IAdminService> adminService
    );

    /**
     * @brief Chạy vòng lặp chính của giao diện người dùng
     * 
     * Phương thức này khởi động vòng lặp chính của ứng dụng, xử lý các trạng thái UI
     * và tương tác với người dùng cho đến khi người dùng chọn thoát.
     */
    void run(); 
};

#endif // CONSOLEUI_H