#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include <memory> // For std::shared_ptr/unique_ptr
#include "../core/services/IAuthService.h"
#include "../core/services/IStudentService.h"
#include "../core/services/IFacultyService.h"
#include "../core/services/IFinanceService.h"
#include "../core/services/IAdminService.h"
#include "../core/repositories/interfaces/ILoginRepository.h" // For UserType
#include "MenuOption.h" // Enum cho các lựa chọn menu

// Lớp quản lý giao diện console
class ConsoleUI {
private:
    // Các service được inject
    std::shared_ptr<IAuthService> _authService;
    std::shared_ptr<IStudentService> _studentService;
    std::shared_ptr<IFacultyService> _facultyService;
    std::shared_ptr<IFinanceService> _financeService;
    std::shared_ptr<IAdminService> _adminService;

    // Trạng thái đăng nhập hiện tại
    std::optional<UserType> _currentUserType;
    std::string _currentUserId; // Lưu ID người dùng đã đăng nhập

    // Hiển thị các menu
    void displayMainMenu();
    void displayLoginPrompt(const std::string& role); // Role: "Finance", "Admin", "Student/Faculty"
    void displayStudentMenu();
    void displayFacultyMenu();
    void displayAdminMenu();
    void displayFinanceMenu(UserType userType); // Menu finance khác nhau cho S và F

    // Các hàm xử lý luồng con
    void handleLogin(const std::string& role);
    void handleStudentActions();
    void handleFacultyActions();
    void handleAdminActions();
    void handleFinanceActions();

    // Các hàm hiển thị thông tin cụ thể
    void showWelcomeHeader();
    void showSectionHeader(const std::string& title);
    void showStudentDetails(const std::string& studentId);
    void showFacultyDetails(const std::string& facultyId);
    void showStudentResultReport(const std::string& studentId);
    void showFeeReceipt(const std::string& studentId);
    void showSalaryCertificate(const std::string& facultyId);
    void showStudentList(const std::vector<Student>& students);
    void showFacultyList(const std::vector<Faculty>& faculty);
    void showDepartmentList(const std::vector<Department>& departments);
    void showHodList(const std::vector<Faculty>& hods);

    // Hàm tiện ích UI
    void clearScreen();
    void pauseExecution(); // Tương đương getch()
    int getMenuChoice(int minOption, int maxOption);
    std::string promptForString(const std::string& prompt);
    long promptForLong(const std::string& prompt);
    bool promptForYesNo(const std::string& prompt);

protected:

public:
    // Constructor nhận các service qua Dependency Injection
    ConsoleUI(
        std::shared_ptr<IAuthService> authService,
        std::shared_ptr<IStudentService> studentService,
        std::shared_ptr<IFacultyService> facultyService,
        std::shared_ptr<IFinanceService> financeService,
        std::shared_ptr<IAdminService> adminService
    );

    // Vòng lặp chính của ứng dụng
    void run();
};

#endif // CONSOLEUI_H