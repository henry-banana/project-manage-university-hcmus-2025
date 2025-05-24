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

// (➕) Định nghĩa các State cho UI
struct UnauthenticatedState {}; // Trạng thái khi chưa đăng nhập (hiển thị Main Menu cho khách)
struct LoginPromptState {};    // Trạng thái đang ở màn hình nhập login
struct StudentRegistrationPromptState {}; // Trạng thái đang ở màn hình đăng ký SV
struct ChangePasswordPromptState {}; // Trạng thái đang ở màn hình đổi mật khẩu

struct AdminPanelState {};     // Trạng thái đang ở menu chính của Admin
struct StudentPanelState {};   // Trạng thái đang ở menu chính của Student
struct TeacherPanelState {};   // Trạng thái đang ở menu chính của Teacher
// Các state cho menu con của Admin (ví dụ)
struct AdminStudentManagementState {};
struct AdminTeacherManagementState {};
struct AdminFacultyManagementState {};
struct AdminCourseManagementState {};
struct AdminAccountManagementState {};
// Các state cho menu con của Student/Teacher nếu cần quản lý phức tạp hơn

// (➕) UIState là một variant của tất cả các state có thể có
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

class ConsoleUI {
private:
    // Services
    std::shared_ptr<IAuthService> _authService;
    std::shared_ptr<IStudentService> _studentService;
    std::shared_ptr<ITeacherService> _teacherService;
    std::shared_ptr<IFacultyService> _facultyService;
    std::shared_ptr<ICourseService> _courseService;
    std::shared_ptr<IEnrollmentService> _enrollmentService;
    std::shared_ptr<IResultService> _resultService;
    std::shared_ptr<IFinanceService> _financeService;
    std::shared_ptr<IAdminService> _adminService;

    // UI Helpers
    std::unique_ptr<InputPrompter> _prompter;
    std::unique_ptr<TableDisplayer> _displayer;
    std::unique_ptr<MenuRenderer> _menuRenderer;

    bool _isRunning;
    UIState _currentState; // (➕) Trạng thái UI hiện tại

    // --- State Handlers (thay thế cho các show...Menu chính và logic switch trong run()) ---
    void processCurrentState();
    void handleState(UnauthenticatedState&);
    void handleState(LoginPromptState&);
    void handleState(StudentRegistrationPromptState&);
    void handleState(ChangePasswordPromptState&);
    void handleState(AdminPanelState&);
    void handleState(StudentPanelState&);
    void handleState(TeacherPanelState&);
    // Handlers cho các menu con của Admin
    void handleState(AdminStudentManagementState&);
    void handleState(AdminTeacherManagementState&);
    void handleState(AdminFacultyManagementState&);
    void handleState(AdminCourseManagementState&);
    void handleState(AdminAccountManagementState&);


    // --- Menu Processing Helper (vẫn hữu ích cho các menu con) ---
    void processMenu(const std::string& title, const std::vector<MenuItemDisplay>& items, 
                     const std::vector<std::function<void()>>& actions, bool isSubMenu = false);

    // --- Action Implementations (logic nghiệp vụ cụ thể, được gọi từ State Handlers) ---
    // Các hàm này giờ đây có thể không cần trả về bool để điều khiển vòng lặp menu nữa,
    // mà việc chuyển state sẽ do handleState tương ứng thực hiện.
    void doLogin();
    void doStudentRegistration();
    void doChangePassword();
    void doLogout();
    void doExitApplication();

    // Admin Actions
    void doAdminApproveRegistration();
    void doAdminViewStudentsByStatus();
    void doAdminAddStudent();
    void doAdminUpdateStudent();
    void doAdminRemoveStudent();
    void doAdminViewAllStudents(); // (➕) Tách riêng
    void doAdminFindStudentById(); // (➕) Tách riêng

    void doAdminAddTeacher();
    void doAdminUpdateTeacher();
    void doAdminRemoveTeacher();
    void doAdminViewAllTeachers();
    void doAdminFindTeacherById();

    void doAdminAddFaculty();
    void doAdminUpdateFaculty();
    void doAdminRemoveFaculty();
    void doAdminViewAllFaculties();
    void doAdminFindFaculty();
    
    void doAdminAddCourse();
    void doAdminUpdateCourse();
    void doAdminRemoveCourse();
    void doAdminViewAllCourses();
    void doAdminFindCourseById();

    void doAdminResetUserPassword();
    void doAdminDisableUserAccount();
    void doAdminEnableUserAccount();

    // Student Actions
    void doStudentViewDetails();
    void doStudentViewResults();
    void doStudentEnrollCourse();
    void doStudentDropCourse();
    void doStudentViewFeeRecord();
    void doStudentMakeFeePayment();

    // Teacher Actions
    void doTeacherViewDetails();
    void doTeacherEnterMarks();
    void doTeacherViewSalary();
    void doTeacherViewEnrolledStudents();


    // --- Data Display Helpers ---
    void displayStudentDetails(const Student& student);
    void displayStudentsList(const std::vector<Student>& students);
    void displayTeacherDetails(const Teacher& teacher);
    void displayTeachersList(const std::vector<Teacher>& teachers);
    void displayFacultyDetails(const Faculty& faculty);
    void displayFacultiesList(const std::vector<Faculty>& faculties);
    void displayCourseDetails(const Course& course);
    void displayCoursesList(const std::vector<Course>& courses);
    void displayCourseResultEntry(const CourseResult& result, const std::string& courseName = "");
    void displayCourseResultsList(const std::vector<CourseResult>& results);
    void displayFeeRecordDetails(const FeeRecord& record);
    void displaySalaryRecordDetails(const SalaryRecord& record);

    // --- Input Helpers ---
    StudentRegistrationData promptForStudentRegistrationData();
    NewStudentDataByAdmin promptForNewStudentDataByAdmin();
    NewTeacherDataByAdmin promptForNewTeacherDataByAdmin();
    
    // --- Utility ---
    void showErrorMessage(const std::string& message); // Overload for simple messages
    void showErrorMessage(const Error& error);
    void showSuccessMessage(const std::string& message);
    void clearAndPause(const std::string& message = "\nPress Enter to continue...");

public:
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

    void run(); 
};

#endif // CONSOLEUI_H