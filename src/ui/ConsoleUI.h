#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include <memory>
#include <string>
#include <vector>
#include <optional>
#include "../core/services/AuthService.h"
#include "../core/services/StudentService.h"
#include "../core/services/TeacherService.h"
#include "../core/services/FacultyService.h" // Department/School Service
#include "../core/services/CourseService.h"
#include "../core/services/EnrollmentService.h"
#include "../core/services/ResultService.h"
#include "../core/services/FinanceService.h"
#include "../core/services/AdminService.h"
#include "../core/entities/User.h" // For UserRole
#include "../core/entities/Student.h"
#include "../core/entities/Teacher.h"
#include "../core/entities/Faculty.h"
#include "../core/entities/Course.h"
#include "MenuOption.h" // User's MenuOption enum

// Lớp quản lý giao diện console
class ConsoleUI {
private:
    std::shared_ptr<IAuthService> _authService;
    std::shared_ptr<IStudentService> _studentService;
    std::shared_ptr<ITeacherService> _teacherService;
    std::shared_ptr<IFacultyService> _facultyService; // Dept/School service
    std::shared_ptr<ICourseService> _courseService;
    std::shared_ptr<IEnrollmentService> _enrollmentService;
    std::shared_ptr<IResultService> _resultService;
    std::shared_ptr<IFinanceService> _financeService;
    std::shared_ptr<IAdminService> _adminService;

    // Trạng thái đăng nhập hiện tại
    // Internal state (not needed if using AuthService state)
    // std::optional<UserRole> _currentUserRole;
    // std::string _currentUserId;
    
    // Hiển thị các menu
    void displayMainMenu();
    void displayStudentMenu();
    void displayTeacherMenu(); // Renamed from FacultyMenu
    void displayAdminTopMenu();
    void displayAdminStudentMenu();
    void displayAdminTeacherMenu(); // Renamed from FacultyMenu
    void displayAdminFacultyMenu(); // Menu for managing Faculties (Departments)
    void displayAdminCourseMenu();  // Menu for managing Courses
    void displayFinanceLoginMenu(); // Choose Student/Teacher login for Finance
    void displayFinanceStudentMenu();
    void displayFinanceTeacherMenu();

    // --- Private Action Handler Methods ---
    void handleMainMenuChoice(MainMenuOption choice);
    void handleStudentActions();
    void handleTeacherActions();
    void handleAdminActions();
    void handleAdminStudentActions();
    void handleAdminTeacherActions();
    void handleAdminFacultyActions();
    void handleAdminCourseActions();
    void handleFinanceActions();
    void handleFinanceStudentActions(const std::string& studentId);
    void handleFinanceTeacherActions(const std::string& teacherId);

    // --- Login/Logout Flow ---
    bool performLogin(const std::string& prompt, UserRole expectedRole = UserRole::STUDENT /* Use a sentinel?*/, bool allowAny = false); // Performs login prompt and validation
    void performLogout();

    // --- Data Display Helpers ---
    void showWelcomeMessage();
    void showSectionHeader(const std::string& title);
    void showStudentDetails(const std::string& studentId);
    void showTeacherDetails(const std::string& teacherId);
    void showFacultyDetails(const std::string& facultyId); // Show Dept/School details
    void showCourseDetails(const std::string& courseId);
    void showStudentResultReport(const std::string& studentId);
    void showFeeReceipt(const std::string& studentId);
    void showSalaryCertificate(const std::string& teacherId);

    // --- List Display Helpers ---
    void showStudentList(const std::vector<Student>& students);
    void showTeacherList(const std::vector<Teacher>& teachers);
    void showFacultyList(const std::vector<Faculty>& faculties); // Show Dept/School list
    void showCourseList(const std::vector<Course>& courses);
    // void showHodList(const std::vector<Teacher>& hods); // Can use showTeacherList

protected:

public:
    // Constructor nhận các service qua Dependency Injection
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

    // Vòng lặp chính của ứng dụng
    void run();
};

#endif // CONSOLEUI_H