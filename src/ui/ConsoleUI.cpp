#include "ConsoleUI.h"
#include "../utils/Logger.h"
#include "../utils/ConsoleUtils.h" 
#include "../common/UserRole.h"      
#include "../common/LoginStatus.h"   
#include <iostream>
#include <iomanip> 
#include <limits>  
#include <algorithm> 
#include <sstream> // For report generation
#include "../core/entities/User.h"
#ifdef _WIN32
#include <windows.h>
#endif



ConsoleUI::ConsoleUI(
    std::shared_ptr<IAuthService> authService,
    std::shared_ptr<IStudentService> studentService,
    std::shared_ptr<ITeacherService> teacherService,
    std::shared_ptr<IFacultyService> facultyService,
    std::shared_ptr<ICourseService> courseService,
    std::shared_ptr<IEnrollmentService> enrollmentService,
    std::shared_ptr<IResultService> resultService,
    std::shared_ptr<IFinanceService> financeService,
    std::shared_ptr<IAdminService> adminService
) : _authService(std::move(authService)),
    _studentService(std::move(studentService)),
    _teacherService(std::move(teacherService)),
    _facultyService(std::move(facultyService)),
    _courseService(std::move(courseService)),
    _enrollmentService(std::move(enrollmentService)),
    _resultService(std::move(resultService)),
    _financeService(std::move(financeService)),
    _adminService(std::move(adminService)),
    _isRunning(true),
    _currentState(UnauthenticatedState{}) {

    _prompter = std::make_unique<InputPrompter>();
    _displayer = std::make_unique<TableDisplayer>();
    _menuRenderer = std::make_unique<MenuRenderer>();

    if (!_authService || !_studentService || !_teacherService || !_facultyService ||
        !_courseService || !_enrollmentService || !_resultService || !_financeService || !_adminService) {
        LOG_CRITICAL("ConsoleUI: One or more services are null during construction.");
        throw std::runtime_error("ConsoleUI: Critical service dependency is missing.");
    }
    LOG_INFO("ConsoleUI initialized successfully.");
}

// --- Utility Implementations ---
void ConsoleUI::showErrorMessage(const std::string& message) {
    std::cerr << "\n### ERROR ###\n" << message << "\n#############" << std::endl;
}
void ConsoleUI::showErrorMessage(const Error& error) {
    std::cerr << "\n### ERROR [" << error.code << "] ###\n" << error.message << "\n#################" << std::endl;
}

void ConsoleUI::showSuccessMessage(const std::string& message) {
    std::cout << "\n*** SUCCESS ***\n" << message << "\n***************" << std::endl;
}

void ConsoleUI::clearAndPause(const std::string& message){
    pauseExecution(message); 
    clearScreen();
}

// --- Menu Processing Helper ---
void ConsoleUI::processMenu(const std::string& title, const std::vector<MenuItemDisplay>& items, 
                            const std::vector<std::function<void()>>& actions, bool isSubMenu) {
    if (items.empty()) { // (➕) Xử lý menu rỗng ngay từ đầu
        // clearScreen(); // Đã clear ở processCurrentState
        _menuRenderer->renderMenu(title, items); // Vẫn render để hiển thị "No options"
        std::cout << "No options available in this menu." << std::endl;
        if (isSubMenu) {
            // Nếu là submenu rỗng, hành động mặc định là quay lại state cha
            // (Cần logic để xác định state cha hoặc action[0] là back)
            // Tạm thời, chúng ta sẽ để handleState của menu cha quyết định khi processMenu này thoát
        } else {
            // Nếu menu chính rỗng (lỗi cấu hình), có thể thoát hoặc về state an toàn
            showErrorMessage("Critical Error: Main menu is empty.");
            doExitApplication();
        }
        clearAndPause("Press Enter to continue...");
        return;
    }


    if (items.size() != actions.size()) {
        showErrorMessage("Internal Error: Menu items and actions count mismatch for menu: " + title);
        LOG_ERROR("Menu items and actions count mismatch for menu: " + title);
        // Logic phục hồi state đã có ở trên, giữ nguyên
        if (isSubMenu) {
            if (std::holds_alternative<AdminStudentManagementState>(_currentState) ||
                /* ... các state con khác của Admin ... */
                std::holds_alternative<AdminAccountManagementState>(_currentState) ) {
                _currentState = AdminPanelState{};
            } else { 
                if (_authService->isAuthenticated()) {
                    UserRole role = _authService->getCurrentUserRole().value_or(UserRole::UNKNOWN);
                    if (role == UserRole::ADMIN) _currentState = AdminPanelState{};
                    else if (role == UserRole::STUDENT) _currentState = StudentPanelState{};
                    else if (role == UserRole::TEACHER) _currentState = TeacherPanelState{};
                    else _currentState = UnauthenticatedState{};
                } else {
                    _currentState = UnauthenticatedState{};
                }
            }
        } else { 
            _currentState = UnauthenticatedState{}; 
        }
        clearAndPause("Press Enter to acknowledge menu error...");
        return;
    }
    
    // (➖) Bỏ vòng lặp while (keepMenuOpen && _isRunning) ở đây
    // processMenu giờ chỉ chạy một lần cho mỗi lần gọi từ handleState

    // clearScreen(); // Đã clear ở processCurrentState hoặc đầu handleState
    _menuRenderer->renderMenu(title, items);
    
    std::string exitKeyText = "Exit/Back";
    int minChoice = 1000, maxChoice = -1; 
    bool hasZeroOption = false;

    // Tìm key "0" và xác định min/max choice
    for(size_t i = 0; i < items.size(); ++i) {
        const auto& item = items[i];
        if (item.key == "0") {
            exitKeyText = item.description;
            hasZeroOption = true;
            minChoice = std::min(minChoice, 0);
            maxChoice = std::max(maxChoice, 0);
        } else {
            try {
                int k = std::stoi(item.key);
                minChoice = std::min(minChoice, k);
                maxChoice = std::max(maxChoice, k);
            } catch(...) { /* Bỏ qua key không phải số, hoặc log lỗi nếu cần */ }
        }
    }

    // Nếu không có lựa chọn hợp lệ nào (ví dụ: tất cả key đều không phải số và không có "0")
    if (maxChoice == -1 && !hasZeroOption) {
        showErrorMessage("Internal Error: Menu has no valid numeric options. (" + title + ")");
        clearAndPause();
        // Quyết định state tiếp theo nếu menu lỗi (ví dụ: quay lại state cha hoặc Unauthenticated)
        // (Logic này có thể cần được điều chỉnh tùy theo cách bạn muốn xử lý lỗi menu)
        if (isSubMenu) { /* Logic quay lại state cha như ở trên */ } 
        else { _currentState = UnauthenticatedState{}; }
        return;
    }
    // Nếu chỉ có "0" hoặc không có lựa chọn nào khác "0"
    if (minChoice == 1000 && hasZeroOption) minChoice = 0; 
    else if (minChoice == 1000 && items.size() > 0) minChoice = std::stoi(items[0].key); // Lấy key đầu tiên nếu không có "0" và minChoice chưa được set


    int choice = _prompter->promptForInt("Enter your choice" + (hasZeroOption ? " (0 to " + exitKeyText + ")" : "") + ":", 
                                         minChoice , maxChoice);

    bool actionTaken = false;
    // Tìm action tương ứng với choice
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i].key == std::to_string(choice)) {
            if (actions[i]) {
                actions[i](); // Gọi hàm action. 
                              // Hàm action này (hoặc handleState gọi nó) sẽ chịu trách nhiệm set _currentState mới.
            } else {
                showErrorMessage("No action defined for menu item: " + items[i].description);
                clearAndPause(); // Cho người dùng đọc lỗi, state hiện tại sẽ được vẽ lại
            }
            actionTaken = true;
            break; 
        }
    }
    
    if (!actionTaken) { 
         showErrorMessage("Invalid choice. Please try again.");
         // Không thay đổi _currentState, vòng lặp run() -> processCurrentState() sẽ vẽ lại menu hiện tại
         clearAndPause(); 
    }
    // Kết thúc processMenu. Vòng lặp run() sẽ tiếp tục với _currentState (có thể đã được thay đổi bởi action).
}

// --- Main Application Loop and State Processing ---
void ConsoleUI::run() {
    LOG_INFO("ConsoleUI run loop started.");
    _isRunning = true;
    _currentState = UnauthenticatedState{};

    while (_isRunning) {
        processCurrentState(); 
    }
    LOG_INFO("ConsoleUI run loop finished.");
    std::cout << "\nExiting University Management System. Goodbye!\n\n";
    std::cout << "============================================================\n";
    std::cout << "University Management System - Application Shutting Down...\n";
    std::cout << "============================================================\n";
    #ifdef _WIN32
        Sleep(1000); // Windows sleep in milliseconds
    #else
        sleep(2); // Unix/Linux sleep in seconds
    #endif
    // Logger::releaseInstance();
}

void ConsoleUI::processCurrentState() {
    clearScreen(); 
    std::visit([this](auto&& state_arg) { 
        using T = std::decay_t<decltype(state_arg)>;
        if constexpr (std::is_same_v<T, UnauthenticatedState>)                this->handleState(state_arg);
        else if constexpr (std::is_same_v<T, LoginPromptState>)               this->handleState(state_arg);
        else if constexpr (std::is_same_v<T, StudentRegistrationPromptState>) this->handleState(state_arg);
        else if constexpr (std::is_same_v<T, ChangePasswordPromptState>)      this->handleState(state_arg);
        else if constexpr (std::is_same_v<T, AdminPanelState>)                this->handleState(state_arg);
        else if constexpr (std::is_same_v<T, StudentPanelState>)              this->handleState(state_arg);
        else if constexpr (std::is_same_v<T, TeacherPanelState>)              this->handleState(state_arg);
        else if constexpr (std::is_same_v<T, AdminStudentManagementState>)    this->handleState(state_arg);
        else if constexpr (std::is_same_v<T, AdminTeacherManagementState>)    this->handleState(state_arg);
        else if constexpr (std::is_same_v<T, AdminFacultyManagementState>)    this->handleState(state_arg);
        else if constexpr (std::is_same_v<T, AdminCourseManagementState>)     this->handleState(state_arg);
        else if constexpr (std::is_same_v<T, AdminAccountManagementState>)    this->handleState(state_arg);
    }, _currentState);
}

// --- State Handler Implementations ---

void ConsoleUI::handleState(UnauthenticatedState&) {
    drawHeader("UNIVERSITY MANAGEMENT SYSTEM");
    std::vector<MenuItemDisplay> items = {
        {"1", "Login"},
        {"2", "Register as Student"},
        {"0", "Exit Application"}
    };
    std::vector<std::function<void()>> actions = {
        [this]() { _currentState = LoginPromptState{}; },
        [this]() { _currentState = StudentRegistrationPromptState{}; },
        [this]() { doExitApplication(); } 
    };
    processMenu("MAIN MENU (GUEST)", items, actions, false);
}

void ConsoleUI::handleState(LoginPromptState&) {
    doLogin(); 
    if (_authService->isAuthenticated()) {
        auto role = _authService->getCurrentUserRole();
        if (role == UserRole::ADMIN) _currentState = AdminPanelState{};
        else if (role == UserRole::STUDENT) _currentState = StudentPanelState{};
        else if (role == UserRole::TEACHER) _currentState = TeacherPanelState{};
        else { 
            showErrorMessage("Your account status (" + LoginStatusUtils::toString(_authService->getCurrentUser().value()->getStatus()) + ") does not permit login at this time.");
            doLogout(); 
            clearAndPause();
        }
    } else { 
        _currentState = UnauthenticatedState{};
    }
}

void ConsoleUI::handleState(StudentRegistrationPromptState&) {
    doStudentRegistration(); 
    _currentState = UnauthenticatedState{}; 
}

void ConsoleUI::handleState(ChangePasswordPromptState&) {
    doChangePassword(); 
    if (_authService->isAuthenticated()) {
        auto role = _authService->getCurrentUserRole();
        if (role == UserRole::ADMIN) _currentState = AdminPanelState{};
        else if (role == UserRole::STUDENT) _currentState = StudentPanelState{};
        else if (role == UserRole::TEACHER) _currentState = TeacherPanelState{};
        else _currentState = UnauthenticatedState{}; 
    } else {
        _currentState = UnauthenticatedState{}; 
    }
}

void ConsoleUI::handleState(AdminPanelState&) {
    if (!_authService->isAuthenticated() || _authService->getCurrentUserRole() != UserRole::ADMIN) {
        showErrorMessage("Access Denied. Not an Admin.");
        doLogout(); clearAndPause(); return;
    }
    drawHeader("ADMIN PANEL - User: " + _authService->getCurrentUser().value()->getFirstName());
    std::vector<MenuItemDisplay> items = {
        {"1", "Student Management"},
        {"2", "Teacher Management"},
        {"3", "Faculty Management"},
        {"4", "Course Management"},
        {"5", "User Account Utilities"},
        {"8", "Change My Password"},
        {"9", "Logout"},
        {"0", "Exit Application"}
    };
    std::vector<std::function<void()>> actions = {
        [this]() { _currentState = AdminStudentManagementState{}; },
        [this]() { _currentState = AdminTeacherManagementState{}; },
        [this]() { _currentState = AdminFacultyManagementState{}; },
        [this]() { _currentState = AdminCourseManagementState{}; },
        [this]() { _currentState = AdminAccountManagementState{}; },
        [this]() { _currentState = ChangePasswordPromptState{}; },
        [this]() { doLogout(); },
        [this]() { doExitApplication(); }
    };
    processMenu("ADMIN PANEL", items, actions, false);
}

void ConsoleUI::handleState(StudentPanelState&) {
    if (!_authService->isAuthenticated() || _authService->getCurrentUserRole() != UserRole::STUDENT) {
        showErrorMessage("Access Denied. Not a Student.");
        doLogout(); clearAndPause(); return;
    }
    drawHeader("STUDENT PANEL - User: " + _authService->getCurrentUser().value()->getFullName());
     std::vector<MenuItemDisplay> items = {
        {"1", "View My Details"},
        {"2", "View My Academic Results"},
        {"3", "Enroll in a Course"},
        {"4", "Drop an Enrolled Course"},
        {"5", "View My Fee Record"},
        {"6", "Make Fee Payment"},
        {"8", "Change My Password"},
        {"9", "Logout"},
        {"0", "Exit Application"}
    };
    std::vector<std::function<void()>> actions = {
        [this]() { this->doStudentViewDetails(); }, 
        [this]() { this->doStudentViewResults(); },
        [this]() { this->doStudentEnrollCourse(); },
        [this]() { this->doStudentDropCourse(); },
        [this]() { this->doStudentViewFeeRecord(); },
        [this]() { this->doStudentMakeFeePayment(); },
        [this]() { _currentState = ChangePasswordPromptState{}; },
        [this]() { doLogout(); },
        [this]() { doExitApplication(); }
    };
    processMenu("STUDENT PANEL", items, actions, false);
}

void ConsoleUI::handleState(TeacherPanelState&) {
    if (!_authService->isAuthenticated() || _authService->getCurrentUserRole() != UserRole::TEACHER) {
        showErrorMessage("Access Denied. Not a Teacher.");
        doLogout(); clearAndPause(); return;
    }
    drawHeader("TEACHER PANEL - User: " + _authService->getCurrentUser().value()->getFullName());
    std::vector<MenuItemDisplay> items = {
        {"1", "View My Details"},
        {"2", "Enter/Update Marks for a Course"},
        {"3", "View My Salary Record"},
        {"4", "View Enrolled Students (by Course)"},
        {"8", "Change My Password"},
        {"9", "Logout"},
        {"0", "Exit Application"}
    };
     std::vector<std::function<void()>> actions = {
        [this]() { this->doTeacherViewDetails(); },
        [this]() { this->doTeacherEnterMarks(); },
        [this]() { this->doTeacherViewSalary(); },
        [this]() { this->doTeacherViewEnrolledStudents(); },
        [this]() { _currentState = ChangePasswordPromptState{}; },
        [this]() { doLogout(); },
        [this]() { doExitApplication(); }
    };
    processMenu("TEACHER PANEL", items, actions, false);
}


// --- State Handlers for Admin Sub-Menus ---
void ConsoleUI::handleState(AdminStudentManagementState&) {
    std::vector<MenuItemDisplay> items = {
        {"1", "Approve Pending Student Registrations"},
        {"2", "View Students by Status"},
        {"3", "Add New Student (Admin)"},
        {"4", "Update Student Details (Admin)"},
        {"5", "Remove Student Account (Admin)"},
        {"6", "View All Students"},
        {"7", "Find Student by ID"},
        {"0", "Back to Admin Panel"}
    };
    std::vector<std::function<void()>> actions = {
        [this]() { this->doAdminApproveRegistration(); },
        [this]() { this->doAdminViewStudentsByStatus(); },
        [this]() { this->doAdminAddStudent(); },
        [this]() { this->doAdminUpdateStudent(); },
        [this]() { this->doAdminRemoveStudent(); },
        [this]() { this->doAdminViewAllStudents(); },
        [this]() { this->doAdminFindStudentById(); },
        [this]() { _currentState = AdminPanelState{}; } 
    };
    processMenu("ADMIN - STUDENT MANAGEMENT", items, actions, true);
}

void ConsoleUI::handleState(AdminTeacherManagementState&) {
     std::vector<MenuItemDisplay> items = {
        {"1", "Add New Teacher"},
        {"2", "View Teacher Details"},
        {"3", "Update Teacher Details"},
        {"4", "Remove Teacher Account"},
        {"5", "View All Teachers"},
        {"6", "View Teachers by Faculty"},
        {"0", "Back to Admin Panel"}
    };
    std::vector<std::function<void()>> actions = {
        [this]() { this->doAdminAddTeacher(); },
        [this]() { this->doAdminFindTeacherById(); },
        [this]() { this->doAdminUpdateTeacher(); },
        [this]() { this->doAdminRemoveTeacher(); },
        [this]() { this->doAdminViewAllTeachers();},
        [this]() {
            std::string facultyId = _prompter->promptForString("Enter Faculty ID to list teachers:");
            auto result = _teacherService->getTeachersByFaculty(facultyId);
            if(result.has_value()) displayTeachersList(result.value());
            else showErrorMessage(result.error());
            clearAndPause();
        },
        [this]() { _currentState = AdminPanelState{}; } 
    };
    processMenu("ADMIN - TEACHER MANAGEMENT", items, actions, true);
}

void ConsoleUI::handleState(AdminFacultyManagementState&) {
     std::vector<MenuItemDisplay> items = {
        {"1", "Add New Faculty"},
        {"2", "View/Find Faculty Details"},
        {"3", "Update Faculty Name"},
        {"4", "Remove Faculty"},
        {"5", "View All Faculties"},
        {"0", "Back to Admin Panel"}
    };
    std::vector<std::function<void()>> actions = {
        [this]() { this->doAdminAddFaculty(); },
        [this]() { this->doAdminFindFaculty(); },
        [this]() { this->doAdminUpdateFaculty(); },
        [this]() { this->doAdminRemoveFaculty(); },
        [this]() { this->doAdminViewAllFaculties(); },
        [this]() { _currentState = AdminPanelState{}; }
    };
    processMenu("ADMIN - FACULTY MANAGEMENT", items, actions, true);
}

void ConsoleUI::handleState(AdminCourseManagementState&) {
     std::vector<MenuItemDisplay> items = {
        {"1", "Add New Course"},
        {"2", "View/Find Course Details"},
        {"3", "Update Course Details"},
        {"4", "Remove Course"},
        {"5", "View All Courses"},
        {"6", "View Courses by Faculty"},
        {"0", "Back to Admin Panel"}
    };
    std::vector<std::function<void()>> actions = {
        [this]() { this->doAdminAddCourse(); },
        [this]() { this->doAdminFindCourseById(); },
        [this]() { this->doAdminUpdateCourse(); },
        [this]() { this->doAdminRemoveCourse(); },
        [this]() { this->doAdminViewAllCourses(); },
        [this]() { 
            std::string facultyId = _prompter->promptForString("Enter Faculty ID to list courses:");
            auto result = _courseService->getCoursesByFaculty(facultyId);
            if(result.has_value()) displayCoursesList(result.value());
            else showErrorMessage(result.error());
            clearAndPause();
        },
        [this]() { _currentState = AdminPanelState{}; }
    };
    processMenu("ADMIN - COURSE MANAGEMENT", items, actions, true);
}

void ConsoleUI::handleState(AdminAccountManagementState&) {
    std::vector<MenuItemDisplay> items = {
        {"1", "Reset User Password"},
        {"2", "Disable User Account"},
        {"3", "Enable/Re-activate User Account"},
        {"0", "Back to Admin Panel"}
    };
    std::vector<std::function<void()>> actions = {
        [this]() { this->doAdminResetUserPassword(); },
        [this]() { this->doAdminDisableUserAccount(); },
        [this]() { this->doAdminEnableUserAccount(); },
        [this]() { _currentState = AdminPanelState{}; } 
    };
    processMenu("ADMIN - USER ACCOUNT UTILITIES", items, actions, true);
}


// --- Action Implementations (Logic nghiệp vụ cụ thể) ---
void ConsoleUI::doExitApplication() {
    _isRunning = false;
    LOG_INFO("Exit Application action called.");
}

void ConsoleUI::doLogout(){
    _authService->logout();
    showSuccessMessage("You have been logged out.");
    _currentState = UnauthenticatedState{}; 
    // Không cần clearAndPause ở đây, vì processCurrentState sẽ vẽ lại cho UnauthenticatedState
}

void ConsoleUI::doLogin() { 
    drawHeader("USER LOGIN");
    std::string userIdOrEmail = _prompter->promptForString("Enter User ID or Email:");
    std::string password = _prompter->promptForPassword("Enter Password:");

    auto loginResult = _authService->login(userIdOrEmail, password);
    if (loginResult.has_value()) {
        showSuccessMessage("Login successful! Welcome " + loginResult.value()->getFullName());
        // Chuyển state sẽ được handleState(LoginPromptState&) làm
    } else {
        showErrorMessage(loginResult.error());
    }
    clearAndPause(); 
}

void ConsoleUI::doStudentRegistration() {
    drawHeader("STUDENT REGISTRATION");
    StudentRegistrationData regData = promptForStudentRegistrationData();
    std::string password = _prompter->promptForPassword("Enter Password for new account:");
    std::string confirmPassword = _prompter->promptForPassword("Confirm Password:");

    if (password != confirmPassword) {
        showErrorMessage("Passwords do not match.");
        clearAndPause();
        return;
    }
    
    auto regResult = _authService->registerStudent(regData, password);
    if (regResult.has_value() && regResult.value()) {
        showSuccessMessage("Registration successful! Your application is pending approval by an Administrator.");
    } else {
        showErrorMessage(regResult.error());
    }
    clearAndPause();
}

void ConsoleUI::doChangePassword(){
    if(!_authService->isAuthenticated()){
        showErrorMessage("You must be logged in to change password.");
        clearAndPause();
        return;
    }
    drawHeader("CHANGE PASSWORD");
    std::string oldPassword = _prompter->promptForPassword("Enter Current Password:");
    std::string newPassword = _prompter->promptForPassword("Enter New Password:");
    std::string confirmNewPassword = _prompter->promptForPassword("Confirm New Password:");

    if(newPassword != confirmNewPassword){
        showErrorMessage("New passwords do not match.");
        clearAndPause();
        return;
    }

    auto currentUserId = _authService->getCurrentUserId();
    if (!currentUserId.has_value()){
        showErrorMessage("Internal Error: Cannot identify current user."); 
        clearAndPause();
        return;
    }

    auto changePassResult = _authService->changePassword(currentUserId.value(), oldPassword, newPassword);
    if(changePassResult.has_value() && changePassResult.value()){
        showSuccessMessage("Password changed successfully.");
    } else {
        showErrorMessage(changePassResult.error());
    }
    clearAndPause();
}

// --- Admin Actions ---
void ConsoleUI::doAdminApproveRegistration() {
    drawHeader("ADMIN - APPROVE STUDENT REGISTRATIONS");
    auto pendingStudents = _adminService->getStudentsByStatus(LoginStatus::PENDING_APPROVAL);
    if (!pendingStudents.has_value()) {
        showErrorMessage(pendingStudents.error());
        clearAndPause();
        return;
    }
    if (pendingStudents.value().empty()) {
        showSuccessMessage("No students are currently pending approval.");
        clearAndPause();
        return;
    }
    
    std::cout << "Students Pending Approval:" << std::endl;
    displayStudentsList(pendingStudents.value());
    
    std::string studentId = _prompter->promptForString("\nEnter ID of student to approve (or type 'cancel'):");
    if (studentId == "cancel" || studentId.empty()) {clearAndPause("Approval cancelled."); return;}

    auto approveResult = _adminService->approveStudentRegistration(studentId);
    if (approveResult.has_value() && approveResult.value()) {
        showSuccessMessage("Student " + studentId + " approved successfully.");
    } else {
        showErrorMessage(approveResult.error());
    }
    clearAndPause();
}

void ConsoleUI::doAdminViewStudentsByStatus() {
    drawHeader("ADMIN - VIEW STUDENTS BY STATUS");
    std::cout << "Select status to view:" << std::endl;
    std::vector<MenuItemDisplay> statusItems = {
        {"1", "Active (" + LoginStatusUtils::toString(LoginStatus::ACTIVE) + ")"},
        {"2", "Pending Approval (" + LoginStatusUtils::toString(LoginStatus::PENDING_APPROVAL) + ")"},
        {"3", "Disabled (" + LoginStatusUtils::toString(LoginStatus::DISABLED) + ")"},
        {"0", "Back"}
    };
    _menuRenderer->renderMenu("Select Status", statusItems);
    int choice = _prompter->promptForInt("Choice:", 0, 3);
    
    LoginStatus statusToView;
    switch(choice){
        case 1: statusToView = LoginStatus::ACTIVE; break;
        case 2: statusToView = LoginStatus::PENDING_APPROVAL; break;
        case 3: statusToView = LoginStatus::DISABLED; break;
        case 0: clearAndPause("Returning..."); return;
        default: showErrorMessage("Invalid status choice."); clearAndPause(); return;
    }

    auto students = _adminService->getStudentsByStatus(statusToView);
    if(students.has_value()){
        std::cout << "\nStudents with status: " << LoginStatusUtils::toString(statusToView) << std::endl;
        displayStudentsList(students.value());
    } else {
        showErrorMessage(students.error());
    }
    clearAndPause();
}


void ConsoleUI::doAdminAddStudent() {
    NewStudentDataByAdmin data = promptForNewStudentDataByAdmin(); 
    
    auto addResult = _adminService->addStudentByAdmin(data);
    if (addResult.has_value()) {
        showSuccessMessage("Student " + addResult.value().getId() + " added successfully by admin.");
        displayStudentDetails(addResult.value());
    } else {
        showErrorMessage(addResult.error());
    }
    clearAndPause();
}

void ConsoleUI::doAdminUpdateStudent() {
    drawHeader("ADMIN - UPDATE STUDENT DETAILS");
    std::string studentId = _prompter->promptForString("Enter ID of student to update:");
    auto studentExp = _studentService->getStudentDetails(studentId); 
    if(!studentExp.has_value()){
        showErrorMessage(studentExp.error());
        clearAndPause();
        return;
    }
    std::cout << "\nCurrent details for student " << studentId << ":" << std::endl;
    displayStudentDetails(studentExp.value());

    StudentUpdateData updateData;
    updateData.studentIdToUpdate = studentId;
    std::cout << "\nEnter new details (press Enter to keep current value):" << std::endl;
    
    std::string temp;
    temp = _prompter->promptForString("New First Name [" + studentExp.value().getFirstName() + "]:", true);
    if(!temp.empty()) updateData.firstName = temp;

    temp = _prompter->promptForString("New Last Name [" + studentExp.value().getLastName() + "]:", true);
    if(!temp.empty()) updateData.lastName = temp;

    temp = _prompter->promptForString("New Email [" + studentExp.value().getEmail() + "]:", true);
    if(!temp.empty()) updateData.email = temp;
    
    temp = _prompter->promptForString("New Faculty ID [" + studentExp.value().getFacultyId() + "]:", true);
    if(!temp.empty()) updateData.facultyId = temp;
    
    if(_prompter->promptForYesNo("Update birthday [" + studentExp.value().getBirthday().toString_ddmmyyyy() + "]?")){
        Birthday bday;
        bday.set(_prompter->promptForInt("  New Day:",1,31), _prompter->promptForInt("  New Month:",1,12), _prompter->promptForInt("  New Year:",1950,2010));
        updateData.birthday = bday;
    }
     temp = _prompter->promptForString("New Address [" + studentExp.value().getAddress() + "]:", true);
    if(!temp.empty()) updateData.address = temp;
    temp = _prompter->promptForString("New Citizen ID [" + studentExp.value().getCitizenId() + "]:", true);
    if(!temp.empty()) updateData.citizenId = temp;
    temp = _prompter->promptForString("New Phone Number [" + studentExp.value().getPhoneNumber() + "]:", true);
    if(!temp.empty()) updateData.phoneNumber = temp;

    auto result = _studentService->updateStudentDetails(updateData); 
    if(result.has_value() && result.value()){
        showSuccessMessage("Student details updated successfully.");
    } else if (result.has_value() && !result.value()){ 
        showSuccessMessage("No changes applied to student details.");
    }
    else {
        showErrorMessage(result.error());
    }
    clearAndPause();
}

void ConsoleUI::doAdminRemoveStudent() {
    drawHeader("ADMIN - REMOVE STUDENT ACCOUNT");
    std::string studentId = _prompter->promptForString("Enter ID of student to remove:");
    if(_prompter->promptForYesNo("Are you sure you want to PERMANENTLY remove student " + studentId + "?\nThis will delete ALL related data (login, enrollments, results, fees). This action is IRREVERSIBLE.")){
        auto result = _adminService->removeStudentAccount(studentId);
        if(result.has_value() && result.value()){
            showSuccessMessage("Student account " + studentId + " removed successfully.");
        } else {
            showErrorMessage(result.error());
        }
    } else {
        std::cout << "Student removal cancelled." << std::endl;
    }
    clearAndPause();
}

void ConsoleUI::doAdminViewAllStudents() {
    drawHeader("ADMIN - ALL STUDENTS");
    auto result = _studentService->getAllStudents(); 
    if(result.has_value()) displayStudentsList(result.value());
    else showErrorMessage(result.error());
    clearAndPause();
}

void ConsoleUI::doAdminFindStudentById() {
    drawHeader("ADMIN - FIND STUDENT BY ID");
    std::string id = _prompter->promptForString("Enter Student ID to find:");
    auto result = _studentService->getStudentDetails(id); 
    if(result.has_value()) displayStudentDetails(result.value());
    else showErrorMessage(result.error());
    clearAndPause();
}


void ConsoleUI::doAdminAddTeacher() {
    NewTeacherDataByAdmin data = promptForNewTeacherDataByAdmin(); 
    
    auto addResult = _adminService->addTeacherByAdmin(data);
    if (addResult.has_value()) {
        showSuccessMessage("Teacher " + addResult.value().getId() + " added successfully.");
        displayTeacherDetails(addResult.value());
    } else {
        showErrorMessage(addResult.error());
    }
    clearAndPause();
}

void ConsoleUI::doAdminUpdateTeacher() {
    drawHeader("ADMIN - UPDATE TEACHER DETAILS");
    std::string teacherId = _prompter->promptForString("Enter ID of teacher to update:");
    auto teacherExp = _teacherService->getTeacherDetails(teacherId);
    if(!teacherExp.has_value()){
        showErrorMessage(teacherExp.error());
        clearAndPause();
        return;
    }
    std::cout << "\nCurrent details for teacher " << teacherId << ":" << std::endl;
    displayTeacherDetails(teacherExp.value());

    TeacherUpdateData updateData;
    updateData.teacherIdToUpdate = teacherId;
    std::cout << "\nEnter new details (press Enter to keep current value):" << std::endl;
    std::string temp;
    
    temp = _prompter->promptForString("New First Name [" + teacherExp.value().getFirstName() + "]:", true);
    if(!temp.empty()) updateData.firstName = temp;
    temp = _prompter->promptForString("New Last Name [" + teacherExp.value().getLastName() + "]:", true);
    if(!temp.empty()) updateData.lastName = temp;
    temp = _prompter->promptForString("New Email [" + teacherExp.value().getEmail() + "]:", true);
    if(!temp.empty()) updateData.email = temp;
    temp = _prompter->promptForString("New Faculty ID [" + teacherExp.value().getFacultyId() + "]:", true);
    if(!temp.empty()) updateData.facultyId = temp;
    temp = _prompter->promptForString("New Qualification [" + teacherExp.value().getQualification() + "]:", true);
    if(!temp.empty()) updateData.qualification = temp;
    temp = _prompter->promptForString("New Specialization [" + teacherExp.value().getSpecializationSubjects() + "]:", true);
    if(!temp.empty()) updateData.specializationSubjects = temp;
    temp = _prompter->promptForString("New Designation [" + teacherExp.value().getDesignation() + "]:", true);
    if(!temp.empty()) updateData.designation = temp;
    
    std::string expYearsStr = _prompter->promptForString("New Experience Years [" + std::to_string(teacherExp.value().getExperienceYears()) + "]:", true);
    if(!expYearsStr.empty()){
        try { updateData.experienceYears = std::stoi(expYearsStr); } 
        catch(const std::exception&){ showErrorMessage("Invalid number for experience years."); }
    }
    // ... các trường khác: birthday, address, citizenId, phone ...

    auto result = _teacherService->updateTeacherDetails(updateData);
    if(result.has_value() && result.value()){
        showSuccessMessage("Teacher details updated successfully.");
    } else if (result.has_value() && !result.value()){
        showSuccessMessage("No changes applied to teacher details.");
    } else {
        showErrorMessage(result.error());
    }
    clearAndPause();
}

void ConsoleUI::doAdminRemoveTeacher() {
    drawHeader("ADMIN - REMOVE TEACHER ACCOUNT");
    std::string teacherId = _prompter->promptForString("Enter ID of teacher to remove:");
    if(_prompter->promptForYesNo("Are you sure you want to PERMANENTLY remove teacher " + teacherId + "?\nThis will delete ALL related data (login, salary). This action is IRREVERSIBLE.")){
        auto result = _adminService->removeTeacherAccount(teacherId);
        if(result.has_value() && result.value()){
            showSuccessMessage("Teacher account " + teacherId + " removed successfully.");
        } else {
            showErrorMessage(result.error());
        }
    } else {
        std::cout << "Teacher removal cancelled." << std::endl;
    }
    clearAndPause();
}

void ConsoleUI::doAdminViewAllTeachers() {
    drawHeader("ADMIN - ALL TEACHERS");
    auto result = _teacherService->getAllTeachers(); 
    if(result.has_value()) displayTeachersList(result.value());
    else showErrorMessage(result.error());
    clearAndPause();
}
void ConsoleUI::doAdminFindTeacherById() {
    drawHeader("ADMIN - FIND TEACHER BY ID");
    std::string id = _prompter->promptForString("Enter Teacher ID to find:");
    auto result = _teacherService->getTeacherDetails(id); 
    if(result.has_value()) displayTeacherDetails(result.value());
    else showErrorMessage(result.error());
    clearAndPause();
}

void ConsoleUI::doAdminAddFaculty() {
    drawHeader("ADMIN - ADD NEW FACULTY");
    std::string id = _prompter->promptForString("Enter Faculty ID (e.g., IT, EE):");
    std::string name = _prompter->promptForString("Enter Faculty Name:");
    
    auto addResult = _facultyService->addFaculty(id, name);
    if (addResult.has_value()) {
        showSuccessMessage("Faculty '" + addResult.value().getName() + "' (ID: " + addResult.value().getId() + ") added successfully.");
        displayFacultyDetails(addResult.value());
    } else {
        showErrorMessage(addResult.error());
    }
    clearAndPause();
}

void ConsoleUI::doAdminUpdateFaculty() {
    drawHeader("ADMIN - UPDATE FACULTY NAME");
    std::string facultyId = _prompter->promptForString("Enter ID of faculty to update:");
    auto facultyExp = _facultyService->getFacultyById(facultyId);
    if(!facultyExp.has_value()){
        showErrorMessage(facultyExp.error());
        clearAndPause();
        return;
    }
    std::cout << "Current name for ID " << facultyId << ": " << facultyExp.value().getName() << std::endl;
    std::string newName = _prompter->promptForString("Enter new name for faculty:");

    auto result = _facultyService->updateFaculty(facultyId, newName);
    if(result.has_value() && result.value()){
        showSuccessMessage("Faculty name updated successfully.");
    } else {
        showErrorMessage(result.error());
    }
    clearAndPause();
}

void ConsoleUI::doAdminRemoveFaculty() {
    drawHeader("ADMIN - REMOVE FACULTY");
    std::string facultyId = _prompter->promptForString("Enter ID of faculty to remove:");
     if(_prompter->promptForYesNo("Are you sure you want to remove faculty " + facultyId + "?\nAssociated students, teachers, and courses will have their faculty ID set to NULL (or prevent removal if DB restricts).")){
        auto result = _facultyService->removeFaculty(facultyId);
        if(result.has_value() && result.value()){
            showSuccessMessage("Faculty " + facultyId + " removed successfully.");
        } else {
            showErrorMessage(result.error());
        }
    } else {
        std::cout << "Faculty removal cancelled." << std::endl;
    }
    clearAndPause();
}

void ConsoleUI::doAdminViewAllFaculties() {
    drawHeader("ADMIN - ALL FACULTIES");
    auto result = _facultyService->getAllFaculties();
    if(result.has_value()){
        displayFacultiesList(result.value());
    } else {
        showErrorMessage(result.error());
    }
    clearAndPause();
}

void ConsoleUI::doAdminFindFaculty() {
    drawHeader("ADMIN - FIND FACULTY");
    std::string idOrName = _prompter->promptForString("Enter Faculty ID or Name to find:");
    auto resultById = _facultyService->getFacultyById(idOrName);
    if (resultById.has_value()) {
        displayFacultyDetails(resultById.value());
    } else {
        auto resultByName = _facultyService->getFacultyByName(idOrName);
        if (resultByName.has_value()) {
            displayFacultyDetails(resultByName.value());
        } else {
            showErrorMessage("Faculty not found by ID or Name: " + idOrName + "\n(Searched by ID error: " + resultById.error().message + ")\n(Searched by Name error: " + resultByName.error().message + ")");
        }
    }
    clearAndPause();
}

void ConsoleUI::doAdminAddCourse() {
    drawHeader("ADMIN - ADD NEW COURSE");
    std::string id = _prompter->promptForString("Enter Course ID (e.g., CS101):");
    std::string name = _prompter->promptForString("Enter Course Name:");
    int credits = _prompter->promptForInt("Enter Credits (1-10):", 1, 10);
    
    auto facultiesExp = _facultyService->getAllFaculties();
    if (facultiesExp.has_value() && !facultiesExp.value().empty()) {
        displayFacultiesList(facultiesExp.value());
    } else if (!facultiesExp.has_value()) {
        showErrorMessage(facultiesExp.error());
    } else {
        showErrorMessage("No faculties available. Please add a faculty first to assign the course.");
    }
    std::string facultyId = _prompter->promptForString("Enter Faculty ID for this course:");

    auto addResult = _courseService->addCourse(id, name, credits, facultyId);
    if(addResult.has_value()){
        showSuccessMessage("Course '" + addResult.value().getName() + "' (ID: "+addResult.value().getId()+") added successfully.");
        displayCourseDetails(addResult.value());
    } else {
        showErrorMessage(addResult.error());
    }
    clearAndPause();
}

void ConsoleUI::doAdminUpdateCourse() {
    drawHeader("ADMIN - UPDATE COURSE DETAILS");
    std::string courseId = _prompter->promptForString("Enter ID of course to update:");
    auto courseExp = _courseService->getCourseById(courseId);
    if(!courseExp.has_value()){
        showErrorMessage(courseExp.error());
        clearAndPause();
        return;
    }
    std::cout << "\nCurrent details for course " << courseId << ":" << std::endl;
    displayCourseDetails(courseExp.value());

    std::cout << "\nEnter new details (press Enter to keep current value):" << std::endl;
    std::string newName = _prompter->promptForString("New Name ["+courseExp.value().getName()+"]:", true);
    if(newName.empty()) newName = courseExp.value().getName();

    std::string creditsStr = _prompter->promptForString("New Credits ["+std::to_string(courseExp.value().getCredits())+"]:", true);
    int newCredits = courseExp.value().getCredits();
    if(!creditsStr.empty()){
        try { newCredits = std::stoi(creditsStr); } 
        catch(const std::exception&){ showErrorMessage("Invalid credits format. Keeping old value."); }
    }
    
    auto facultiesExp = _facultyService->getAllFaculties();
     if (facultiesExp.has_value() && !facultiesExp.value().empty()) {
        displayFacultiesList(facultiesExp.value());
    }
    std::string newFacultyId = _prompter->promptForString("New Faculty ID ["+courseExp.value().getFacultyId()+"]:", true);
    if(newFacultyId.empty()) newFacultyId = courseExp.value().getFacultyId();

    auto result = _courseService->updateCourse(courseId, newName, newCredits, newFacultyId);
    if(result.has_value() && result.value()){
        showSuccessMessage("Course details updated successfully.");
    } else {
        showErrorMessage(result.error());
    }
    clearAndPause();
}

void ConsoleUI::doAdminRemoveCourse() {
    drawHeader("ADMIN - REMOVE COURSE");
    std::string courseId = _prompter->promptForString("Enter ID of course to remove:");
    if(_prompter->promptForYesNo("Are you sure you want to remove course " + courseId + "?\nThis may affect enrollments and results (data will be unlinked or deleted based on DB schema).")){
        auto result = _courseService->removeCourse(courseId);
        if(result.has_value() && result.value()){
            showSuccessMessage("Course " + courseId + " removed successfully.");
        } else {
            showErrorMessage(result.error());
        }
    } else {
        std::cout << "Course removal cancelled." << std::endl;
    }
    clearAndPause();
}

void ConsoleUI::doAdminViewAllCourses() {
    drawHeader("ADMIN - ALL COURSES");
    auto result = _courseService->getAllCourses();
    if(result.has_value()){
        displayCoursesList(result.value());
    } else {
        showErrorMessage(result.error());
    }
    clearAndPause();
}

void ConsoleUI::doAdminFindCourseById() {
    drawHeader("ADMIN - FIND COURSE BY ID");
    std::string id = _prompter->promptForString("Enter Course ID to find:");
    auto result = _courseService->getCourseById(id); 
    if(result.has_value()) displayCourseDetails(result.value());
    else showErrorMessage(result.error());
    clearAndPause();
}


void ConsoleUI::doAdminResetUserPassword(){
    drawHeader("ADMIN - RESET USER PASSWORD");
    std::string userId = _prompter->promptForString("Enter User ID (Student or Teacher) to reset password:");
    std::string newPassword = _prompter->promptForPassword("Enter new password for user " + userId + ":");
    std::string confirmPassword = _prompter->promptForPassword("Confirm new password:");
    if(newPassword != confirmPassword){
        showErrorMessage("Passwords do not match.");
        clearAndPause();
        return;
    }
    auto result = _adminService->resetUserPassword(userId, newPassword);
    if(result.has_value() && result.value()){
        showSuccessMessage("Password for user " + userId + " has been reset.");
    } else {
        showErrorMessage(result.error());
    }
    clearAndPause();
}
void ConsoleUI::doAdminDisableUserAccount(){
    drawHeader("ADMIN - DISABLE USER ACCOUNT");
    std::string userId = _prompter->promptForString("Enter User ID to disable:");
    if(_prompter->promptForYesNo("Are you sure you want to disable account " + userId + "?")){
        auto result = _adminService->disableUserAccount(userId);
        if(result.has_value() && result.value()){
            showSuccessMessage("User account " + userId + " disabled.");
        } else {
            showErrorMessage(result.error());
        }
    } else {
        std::cout << "Account disabling cancelled." << std::endl;
    }
    clearAndPause();
}
void ConsoleUI::doAdminEnableUserAccount(){
    drawHeader("ADMIN - ENABLE USER ACCOUNT");
    std::string userId = _prompter->promptForString("Enter User ID to enable/re-activate:");
     if(_prompter->promptForYesNo("Are you sure you want to enable account " + userId + "?")){
        auto result = _adminService->enableUserAccount(userId);
        if(result.has_value() && result.value()){
            showSuccessMessage("User account " + userId + " enabled/activated.");
        } else {
            showErrorMessage(result.error());
        }
    } else {
        std::cout << "Account enabling cancelled." << std::endl;
    }
    clearAndPause();
}

// --- Student Actions ---
void ConsoleUI::doStudentViewDetails() {
    if(!_authService->isAuthenticated() || !_authService->getCurrentUserId().has_value()) {
        showErrorMessage("Not logged in."); clearAndPause(); return;
    }
    std::string studentId = _authService->getCurrentUserId().value();
    auto result = _studentService->getStudentDetails(studentId);
    if(result.has_value()) {
        drawHeader("MY STUDENT DETAILS");
        displayStudentDetails(result.value());
    } else {
        showErrorMessage(result.error());
    }
    clearAndPause();
}

void ConsoleUI::doStudentViewResults() {
    if(!_authService->isAuthenticated() || !_authService->getCurrentUserId().has_value()) {
        showErrorMessage("Not logged in."); clearAndPause(); return;
    }
    std::string studentId = _authService->getCurrentUserId().value();
    drawHeader("MY ACADEMIC RESULTS");
    auto reportResult = _resultService->generateStudentResultReport(studentId);
    if(reportResult.has_value()){
        std::cout << reportResult.value() << std::endl;
    } else {
        showErrorMessage(reportResult.error());
    }
    clearAndPause();
}
void ConsoleUI::doStudentEnrollCourse() {
    if(!_authService->isAuthenticated() || !_authService->getCurrentUserId().has_value()) {
        showErrorMessage("Not logged in."); clearAndPause(); return;
    }
    std::string studentId = _authService->getCurrentUserId().value();
    drawHeader("ENROLL IN COURSE");
    
    auto coursesExp = _courseService->getAllCourses();
    if(!coursesExp.has_value()){
        showErrorMessage(coursesExp.error());
        clearAndPause(); return;
    }
    if(coursesExp.value().empty()){
        showErrorMessage("No courses available to enroll at the moment.");
        clearAndPause(); return;
    }
    std::cout << "Available courses:" << std::endl;
    displayCoursesList(coursesExp.value());
    std::string courseId = _prompter->promptForString("Enter Course ID to enroll:");

    auto enrollResult = _enrollmentService->enrollStudentInCourse(studentId, courseId);
    if(enrollResult.has_value() && enrollResult.value()){
        showSuccessMessage("Successfully enrolled in course " + courseId);
    } else {
        showErrorMessage(enrollResult.error());
    }
    clearAndPause();
}
void ConsoleUI::doStudentDropCourse() {
     if(!_authService->isAuthenticated() || !_authService->getCurrentUserId().has_value()) {
        showErrorMessage("Not logged in."); clearAndPause(); return;
    }
    std::string studentId = _authService->getCurrentUserId().value();
    drawHeader("DROP COURSE");
    auto enrolledCoursesExp = _enrollmentService->getEnrolledCoursesByStudent(studentId);
    if(!enrolledCoursesExp.has_value()){
        showErrorMessage(enrolledCoursesExp.error());
        clearAndPause(); return;
    }
    if(enrolledCoursesExp.value().empty()){
        showErrorMessage("You are not enrolled in any courses to drop.");
        clearAndPause(); return;
    }
    std::cout << "Your enrolled courses:" << std::endl;
    displayCoursesList(enrolledCoursesExp.value());
    std::string courseId = _prompter->promptForString("Enter Course ID to drop:");

    if(_prompter->promptForYesNo("Are you sure you want to drop course " + courseId + "?")){
        auto dropResult = _enrollmentService->dropCourseForStudent(studentId, courseId);
        if(dropResult.has_value() && dropResult.value()){
            showSuccessMessage("Successfully dropped course " + courseId);
        } else {
            showErrorMessage(dropResult.error());
        }
    } else {
        std::cout << "Course drop cancelled." << std::endl;
    }
    clearAndPause();
}
void ConsoleUI::doStudentViewFeeRecord() {
    if(!_authService->isAuthenticated() || !_authService->getCurrentUserId().has_value()) {
        showErrorMessage("Not logged in."); clearAndPause(); return;
    }
    std::string studentId = _authService->getCurrentUserId().value();
    drawHeader("MY FEE RECORD");
    auto feeExp = _financeService->getStudentFeeRecord(studentId);
    if(feeExp.has_value()){
        displayFeeRecordDetails(feeExp.value());
    } else {
        showErrorMessage(feeExp.error());
    }
    clearAndPause();
}
void ConsoleUI::doStudentMakeFeePayment() {
    if(!_authService->isAuthenticated() || !_authService->getCurrentUserId().has_value()) {
        showErrorMessage("Not logged in."); clearAndPause(); return;
    }
    std::string studentId = _authService->getCurrentUserId().value();
    drawHeader("MAKE FEE PAYMENT");
    auto feeExp = _financeService->getStudentFeeRecord(studentId);
    if(!feeExp.has_value()){
        showErrorMessage(feeExp.error());
        clearAndPause(); return;
    }
    displayFeeRecordDetails(feeExp.value());
    if(feeExp.value().isFullyPaid()){
        showSuccessMessage("Your fees are fully paid. No payment needed.");
        clearAndPause(); return;
    }
    long amount = _prompter->promptForLong("Enter amount to pay (1 - " + std::to_string(feeExp.value().getDueFee()) + "):", 1, feeExp.value().getDueFee());
    
    if(_prompter->promptForYesNo("Confirm payment of " + std::to_string(amount) + " VND?")){
        auto paymentResult = _financeService->makeFeePayment(studentId, amount);
        if(paymentResult.has_value() && paymentResult.value()){
            showSuccessMessage("Payment of " + std::to_string(amount) + " successful.");
            auto receiptExp = _financeService->generateFeeReceipt(studentId, amount);
            if(receiptExp.has_value()) {
                std::cout << "\n--- PAYMENT RECEIPT ---" << std::endl;
                std::cout << receiptExp.value() << std::endl;
            }
        } else {
            showErrorMessage(paymentResult.error());
        }
    } else {
        std::cout << "Payment cancelled." << std::endl;
    }
    clearAndPause();
}

// --- Teacher Actions ---
void ConsoleUI::doTeacherViewDetails() { 
    if(!_authService->isAuthenticated() || !_authService->getCurrentUserId().has_value() || _authService->getCurrentUserRole() != UserRole::TEACHER) {
        showErrorMessage("Access Denied or Not Logged In."); clearAndPause(); return;
    }
    std::string teacherId = _authService->getCurrentUserId().value();
    auto result = _teacherService->getTeacherDetails(teacherId);
    if(result.has_value()) {
        drawHeader("MY TEACHER DETAILS");
        displayTeacherDetails(result.value());
    } else {
        showErrorMessage(result.error());
    }
    clearAndPause();
}
void ConsoleUI::doTeacherEnterMarks() { 
    drawHeader("TEACHER - ENTER/UPDATE MARKS");
    // Giáo viên cần chọn môn học của mình trước
    // TODO: Cần logic để lấy danh sách môn học mà giáo viên này dạy
    // Tạm thời, cho phép nhập Course ID trực tiếp
    std::string courseId = _prompter->promptForString("Enter Course ID to enter/update marks for:");
    auto courseExp = _courseService->getCourseById(courseId);
    if(!courseExp.has_value()){
        showErrorMessage("Course not found: " + courseExp.error().message);
        clearAndPause(); return;
    }
    std::cout << "Selected course: " << courseExp.value().getName() << " (" << courseId << ")" << std::endl;

    std::string studentId = _prompter->promptForString("Enter Student ID:");
    int marks = _prompter->promptForInt("Enter Marks (-1 for N/A, 0-100):", -1, 100);

    auto result = _resultService->enterMarks(studentId, courseId, marks);
    if(result.has_value() && result.value()){
        showSuccessMessage("Marks for student " + studentId + " in course " + courseId + " updated to " + (marks == -1 ? "N/A" : std::to_string(marks)) + ".");
    } else {
        showErrorMessage(result.error());
    }
    clearAndPause();
}
void ConsoleUI::doTeacherViewSalary() { 
    if(!_authService->isAuthenticated() || !_authService->getCurrentUserId().has_value() || _authService->getCurrentUserRole() != UserRole::TEACHER) {
        showErrorMessage("Access Denied or Not Logged In."); clearAndPause(); return;
    }
    std::string teacherId = _authService->getCurrentUserId().value();
    drawHeader("MY SALARY RECORD");
    auto result = _financeService->getTeacherSalaryRecord(teacherId);
    if(result.has_value()){
        displaySalaryRecordDetails(result.value());
        if(_prompter->promptForYesNo("Generate salary certificate?")){
            auto certExp = _financeService->generateSalaryCertificate(teacherId);
            if(certExp.has_value()) std::cout << "\n--- SALARY CERTIFICATE ---\n" << certExp.value() << std::endl;
            else showErrorMessage(certExp.error());
        }
    } else {
        showErrorMessage(result.error());
    }
    clearAndPause();
}
void ConsoleUI::doTeacherViewEnrolledStudents() { 
    drawHeader("TEACHER - VIEW ENROLLED STUDENTS BY COURSE");
    // TODO: Lấy danh sách các môn GV dạy
    // Tạm thời, cho GV nhập Course ID
    auto coursesExp = _courseService->getAllCourses();
    if(!coursesExp.has_value()){ showErrorMessage(coursesExp.error()); clearAndPause(); return;}
    if(coursesExp.value().empty()){showErrorMessage("No courses available in the system."); clearAndPause(); return;}
    
    displayCoursesList(coursesExp.value());
    std::string courseId = _prompter->promptForString("Enter Course ID to view enrolled students:");

    auto studentsExp = _enrollmentService->getEnrolledStudentsByCourse(courseId);
    if(studentsExp.has_value()){
        std::cout << "\nStudents enrolled in " << courseId << ":" << std::endl;
        displayStudentsList(studentsExp.value());
    } else {
        showErrorMessage(studentsExp.error());
    }
    clearAndPause();
}

// --- Display Helpers Implementation ---
void ConsoleUI::displayStudentDetails(const Student& student) {
    std::vector<std::string> content;
    content.push_back("Student ID   : " + student.getId());
    content.push_back("Full Name    : " + student.getFullName());
    content.push_back("Email        : " + (student.getEmail().empty() ? "N/A" : student.getEmail()));
    content.push_back("Phone        : " + (student.getPhoneNumber().empty() ? "N/A" : student.getPhoneNumber()));
    content.push_back("Birthday     : " + (student.getBirthday().isSet() ? student.getBirthday().toString_ddmmyyyy() : "N/A"));
    content.push_back("Address      : " + (student.getAddress().empty() ? "N/A" : student.getAddress()));
    content.push_back("Citizen ID   : " + (student.getCitizenId().empty() ? "N/A" : student.getCitizenId()));
    
    std::string facultyName = student.getFacultyId();
    auto facultyExp = _facultyService->getFacultyById(student.getFacultyId());
    if(facultyExp.has_value()) facultyName = facultyExp.value().getName() + " (" + student.getFacultyId() + ")";
    content.push_back("Faculty      : " + facultyName);

    content.push_back("Role         : " + toStringUserRole(student.getRole()));
    content.push_back("Status       : " + LoginStatusUtils::toString(student.getStatus()));
    drawBox(content, '=', 70);
}

void ConsoleUI::displayStudentsList(const std::vector<Student>& students) {
    if (students.empty()) {
        std::cout << "No students to display." << std::endl;
        return;
    }
    std::vector<std::string> headers = {"ID", "Full Name", "Email", "Faculty ID", "Status"};
    std::vector<std::vector<std::string>> rows;
    std::vector<int> widths = {10, 25, 30, 12, 18};
    for (const auto& s : students) {
        rows.push_back({
            s.getId(),
            s.getFullName(),
            s.getEmail(),
            s.getFacultyId(),
            LoginStatusUtils::toString(s.getStatus())
        });
    }
    _displayer->displayTable(headers, rows, widths);
}

void ConsoleUI::displayTeacherDetails(const Teacher& teacher) {
    std::vector<std::string> content;
    content.push_back("Teacher ID   : " + teacher.getId());
    content.push_back("Full Name    : " + teacher.getFullName());
    content.push_back("Email        : " + (teacher.getEmail().empty() ? "N/A" : teacher.getEmail()));
    content.push_back("Phone        : " + (teacher.getPhoneNumber().empty() ? "N/A" : teacher.getPhoneNumber()));
    content.push_back("Birthday     : " + (teacher.getBirthday().isSet() ? teacher.getBirthday().toString_ddmmyyyy() : "N/A"));
    content.push_back("Address      : " + (teacher.getAddress().empty() ? "N/A" : teacher.getAddress()));
    content.push_back("Citizen ID   : " + (teacher.getCitizenId().empty() ? "N/A" : teacher.getCitizenId()));
    
    std::string facultyName = teacher.getFacultyId();
    auto facultyExp = _facultyService->getFacultyById(teacher.getFacultyId());
    if(facultyExp.has_value()) facultyName = facultyExp.value().getName() + " (" + teacher.getFacultyId() + ")";
    content.push_back("Faculty      : " + facultyName);

    content.push_back("Qualification: " + (teacher.getQualification().empty() ? "N/A" : teacher.getQualification()));
    content.push_back("Specialization: " + (teacher.getSpecializationSubjects().empty() ? "N/A" : teacher.getSpecializationSubjects()));
    content.push_back("Designation  : " + (teacher.getDesignation().empty() ? "N/A" : teacher.getDesignation()));
    content.push_back("Experience   : " + std::to_string(teacher.getExperienceYears()) + " years");
    content.push_back("Role         : " + toStringUserRole(teacher.getRole()));
    content.push_back("Status       : " + LoginStatusUtils::toString(teacher.getStatus()));
    drawBox(content, '=', 70);
}
void ConsoleUI::displayTeachersList(const std::vector<Teacher>& teachers){
     if (teachers.empty()) { std::cout << "No teachers to display." << std::endl; return; }
    std::vector<std::string> headers = {"ID", "Full Name", "Email", "Faculty ID", "Designation", "Status"};
    std::vector<std::vector<std::string>> rows;
     std::vector<int> widths = {10, 25, 30, 12, 20, 18};
    for (const auto& t : teachers) {
        rows.push_back({t.getId(), t.getFullName(), t.getEmail(), t.getFacultyId(), t.getDesignation(), LoginStatusUtils::toString(t.getStatus())});
    }
    _displayer->displayTable(headers, rows, widths);
}
void ConsoleUI::displayFacultyDetails(const Faculty& faculty){
    std::vector<std::string> content = {
        "Faculty ID   : " + faculty.getId(),
        "Faculty Name : " + faculty.getName()
    };
    drawBox(content, '=', 50);
}
void ConsoleUI::displayFacultiesList(const std::vector<Faculty>& faculties){
    if (faculties.empty()) { std::cout << "No faculties to display." << std::endl; return; }
    std::vector<std::string> headers = {"ID", "Name"};
    std::vector<std::vector<std::string>> rows;
    std::vector<int> widths = {10, 40};
    for (const auto& f : faculties) {
        rows.push_back({f.getId(), f.getName()});
    }
    _displayer->displayTable(headers, rows, widths);
}
void ConsoleUI::displayCourseDetails(const Course& course){
     std::vector<std::string> content = {
        "Course ID    : " + course.getId(),
        "Course Name  : " + course.getName(),
        "Credits      : " + std::to_string(course.getCredits())
    };
    std::string facultyName = course.getFacultyId();
    auto facultyExp = _facultyService->getFacultyById(course.getFacultyId());
    if(facultyExp.has_value()) facultyName = facultyExp.value().getName() + " (" + course.getFacultyId() + ")";
    content.push_back("Faculty      : " + facultyName);
    drawBox(content, '=', 60);
}
void ConsoleUI::displayCoursesList(const std::vector<Course>& courses){
    if (courses.empty()) { std::cout << "No courses to display." << std::endl; return; }
    std::vector<std::string> headers = {"ID", "Name", "Credits", "Faculty ID"};
    std::vector<std::vector<std::string>> rows;
    std::vector<int> widths = {10, 35, 10, 12};
    for (const auto& c : courses) {
        rows.push_back({c.getId(), c.getName(), std::to_string(c.getCredits()), c.getFacultyId()});
    }
    _displayer->displayTable(headers, rows, widths);
}

void ConsoleUI::displayCourseResultEntry(const CourseResult& result, const std::string& courseName) {
    std::cout << "Course: " << std::left << std::setw(30) << (!courseName.empty() ? courseName : result.getCourseId())
              << " Marks: " << std::right << std::setw(3) << (result.getMarks() == -1 ? "N/A" : std::to_string(result.getMarks()))
              << " Grade: " << result.getGrade() << std::endl;
}

void ConsoleUI::displayCourseResultsList(const std::vector<CourseResult>& results) {
     if (results.empty()) {
        std::cout << "No results to display." << std::endl;
        return;
    }
    std::vector<std::string> headers = {"Course ID", "Course Name", "Credits", "Marks", "Grade"};
    std::vector<std::vector<std::string>> rows;
    std::vector<int> widths = {12, 35, 10, 8, 8};
    for (const auto& res : results) {
        std::string cName = "N/A";
        int credits = 0;
        auto courseRes = _courseService->getCourseById(res.getCourseId());
        if(courseRes.has_value()){
            cName = courseRes.value().getName();
            credits = courseRes.value().getCredits();
        }
        rows.push_back({
            res.getCourseId(),
            cName,
            std::to_string(credits),
            res.getMarks() == -1 ? "N/A" : std::to_string(res.getMarks()),
            std::string(1, res.getGrade())
        });
    }
    _displayer->displayTable(headers, rows, widths);
}
void ConsoleUI::displayFeeRecordDetails(const FeeRecord& record){
    std::vector<std::string> content = {
        "Student ID : " + record.getStudentId(),
        "Total Fee  : " + std::to_string(record.getTotalFee()) + " VND",
        "Paid Fee   : " + std::to_string(record.getPaidFee()) + " VND",
        "Due Fee    : " + std::to_string(record.getDueFee()) + " VND",
        "Status     : " + std::string(record.isFullyPaid() ? "Fully Paid" : "Payment Due")
    };
    drawBox(content,'=', 50);
}
void ConsoleUI::displaySalaryRecordDetails(const SalaryRecord& record){
     std::vector<std::string> content = {
        "Teacher ID        : " + record.getTeacherId(),
        "Basic Monthly Pay : " + std::to_string(record.getBasicMonthlyPay()) + " VND"
    };
    drawBox(content, '=', 50);
}

// --- Input Helper Implementations ---
StudentRegistrationData ConsoleUI::promptForStudentRegistrationData() {
    StudentRegistrationData data;
    // drawHeader("NEW STUDENT INFORMATION"); // Header được vẽ ở hàm gọi
    data.email = _prompter->promptForEmail("Enter Email:");
    data.firstName = _prompter->promptForString("Enter First Name:");
    data.lastName = _prompter->promptForString("Enter Last Name:");
    
    auto facultiesExp = _facultyService->getAllFaculties(); 
    if (facultiesExp.has_value() && !facultiesExp.value().empty()) {
        std::cout << "\nAvailable Faculties:" << std::endl;
        displayFacultiesList(facultiesExp.value());
        data.facultyId = _prompter->promptForString("Enter Faculty ID from the list above:");
    } else {
        if (!facultiesExp.has_value()) showErrorMessage(facultiesExp.error());
        else showErrorMessage("No faculties available to select from. Please ensure faculties are added by Admin.");
        data.facultyId = _prompter->promptForString("Enter Faculty ID (will be validated):");
    }

    std::cout << "\nEnter Birthday:" << std::endl;
    data.birthDay = _prompter->promptForInt("  Day (1-31):", 1, 31);
    data.birthMonth = _prompter->promptForInt("  Month (1-12):", 1, 12);
    data.birthYear = _prompter->promptForInt("  Year (e.g., 1990-2006):", 1950, 2010); 
    data.address = _prompter->promptForString("Enter Address (optional):", true);
    data.citizenId = _prompter->promptForString("Enter Citizen ID (9 or 12 digits):");
    data.phoneNumber = _prompter->promptForString("Enter Phone Number (optional, VN format e.g., 09xxxxxxxx):", true);
    return data;
}

NewStudentDataByAdmin ConsoleUI::promptForNewStudentDataByAdmin(){
    NewStudentDataByAdmin data;
    // Hàm promptForStudentRegistrationData đã có drawHeader riêng cho phần thông tin SV
    data.studentInfo = promptForStudentRegistrationData(); 
    data.initialPassword = _prompter->promptForPassword("Enter Initial Password for Student:");
    return data;
}

NewTeacherDataByAdmin ConsoleUI::promptForNewTeacherDataByAdmin() {
    NewTeacherDataByAdmin data;
    // drawHeader("ADMIN - ADD NEW TEACHER"); // Header được vẽ ở hàm gọi
    data.id = _prompter->promptForString("Enter Teacher ID (e.g., T001):");
    data.email = _prompter->promptForEmail("Enter Email:");
    data.firstName = _prompter->promptForString("Enter First Name:");
    data.lastName = _prompter->promptForString("Enter Last Name:");
    
    auto facultiesExp = _facultyService->getAllFaculties();
    if (facultiesExp.has_value() && !facultiesExp.value().empty()) {
        std::cout << "\nAvailable Faculties:" << std::endl;
        displayFacultiesList(facultiesExp.value());
        data.facultyId = _prompter->promptForString("Enter Faculty ID from the list above:");
    } else {
        if (!facultiesExp.has_value()) showErrorMessage(facultiesExp.error());
        else showErrorMessage("No faculties available. Please ensure faculties are added first.");
        data.facultyId = _prompter->promptForString("Enter Faculty ID (will be validated):");
    }

    std::cout << "\nEnter Birthday:" << std::endl;
    data.birthDay = _prompter->promptForInt("  Day (1-31):", 1, 31);
    data.birthMonth = _prompter->promptForInt("  Month (1-12):", 1, 12);
    data.birthYear = _prompter->promptForInt("  Year (e.g., 1970-1995):", 1950, 2000); 
    data.address = _prompter->promptForString("Enter Address (optional):", true);
    data.citizenId = _prompter->promptForString("Enter Citizen ID (9 or 12 digits):");
    data.phoneNumber = _prompter->promptForString("Enter Phone Number (optional, VN format e.g., 09xxxxxxxx):", true);
    data.qualification = _prompter->promptForString("Enter Qualification (e.g., PhD, MSc):");
    data.specializationSubjects = _prompter->promptForString("Enter Specialization Subjects (comma-separated, optional):", true);
    data.designation = _prompter->promptForString("Enter Designation (e.g., Lecturer, Professor):");
    data.experienceYears = _prompter->promptForInt("Enter Experience Years (0-50):", 0, 50);
    data.initialPassword = _prompter->promptForPassword("Enter Initial Password for Teacher:");
    return data;
}