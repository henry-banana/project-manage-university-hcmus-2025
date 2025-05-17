#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include <memory>
#include <string>
#include <vector>
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
// Menu Enums
#include "MenuOption.h"
// Entity Classes (để hiển thị hoặc nhận input cho các struct như StudentRegistrationData)
#include "../core/entities/Student.h"
// ... các entities khác nếu cần

// Forward declarations cho UI helpers
class InputPrompter;
class TableDisplayer;
class MenuRenderer;

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

    // UI Helpers (sẽ được tạo trong constructor hoặc inject)
    std::unique_ptr<InputPrompter> _inputPrompter;
    std::unique_ptr<TableDisplayer> _tableDisplayer;
    std::unique_ptr<MenuRenderer> _menuRenderer;

    // Menu Display Methods (sử dụng _menuRenderer)
    void displayMainMenu();
    // ... các hàm display menu khác

    // Action Handlers
    void handleMainMenuChoice(MainMenuOption choice);
    void handleAdminLoginAndActions();
    void handleUserLoginAndActions();
    void handleStudentRegistration();
    // ... các hàm handle menu con

    // Helper for student registration data input
    StudentRegistrationData promptForStudentRegistrationData();

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
        // std::unique_ptr<InputPrompter> prompter, // có thể inject helpers
        // std::unique_ptr<TableDisplayer> displayer,
        // std::unique_ptr<MenuRenderer> renderer
    );

    void run(); // Main application loop
};

#endif // CONSOLEUI_H