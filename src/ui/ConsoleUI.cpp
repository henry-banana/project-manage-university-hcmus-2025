#include "ConsoleUI.h"
#include "../utils/ConsoleUtils.h"   // For clearScreen, pauseExecution, etc.
#include "../utils/PasswordInput.h" // For getMaskedPassword
#include "../utils/Logger.h"
#include <iostream>
#include <limits> // For numeric_limits
#include <stdexcept> // For constructor validation
#include <format>

// --- Constructor ---
ConsoleUI::ConsoleUI(
    std::shared_ptr<IAuthService> authService,
    std::shared_ptr<IStudentService> studentService,
    std::shared_ptr<ITeacherService> teacherService,
    std::shared_ptr<IFacultyService> facultyService,
    std::shared_ptr<ICourseService> courseService,
    std::shared_ptr<IEnrollmentService> enrollmentService,
    std::shared_ptr<IResultService> resultService,
    std::shared_ptr<IFinanceService> financeService,
    std::shared_ptr<IAdminService> adminService)
    : _authService(authService),
      _studentService(studentService),
      _teacherService(teacherService),
      _facultyService(facultyService),
      _courseService(courseService),
      _enrollmentService(enrollmentService),
      _resultService(resultService),
      _financeService(financeService),
      _adminService(adminService)
       {
    // Validate that all injected services are non-null
    if (!_authService || !_studentService || !_teacherService || !_facultyService ||
        !_courseService || !_enrollmentService || !_resultService || !_financeService ||
        !_adminService) {
        throw std::invalid_argument("All services must be provided to ConsoleUI.");
    }
}

// --- Main Application Loop ---
void ConsoleUI::run() {
    showWelcomeMessage();
    pauseExecution();

    bool running = true;
    while (running) {
        clearScreen();
        displayMainMenu();
        int choice = getMenuChoice(static_cast<int>(MainMenuOption::EXIT), static_cast<int>(MainMenuOption::STUDENT_FACULTY_LOGIN));
        MainMenuOption option = static_cast<MainMenuOption>(choice);

        if (option == MainMenuOption::EXIT) {
            running = false;
        } else {
            handleMainMenuChoice(option);
             if (running) { // Pause only if not exiting immediately
                pauseExecution();
             }
        }
    }
     clearScreen();
     std::cout << "\nExiting University Management System. Goodbye!\n" << std::endl;
     LOG_INFO("Application exited normally.");
}

// --- Menu Display Methods ---

void ConsoleUI::showWelcomeMessage() {
    clearScreen();
    std::cout << "\n\n\n";
    drawHeader("WELCOME TO UNIVERSITY MANAGEMENT SYSTEM", '=');
    std::cout << std::format("\n\n\t\t CHAROTAR UNIVERSITY OF SCIENCE AND TECHNOLOGY (CHARUSAT)\n"
                            "\t\t         FACULTY OF ENGINEERING AND TECHNOLOGY, CHANGA") << std::endl;
    std::cout << "\n\n\n";
}


void ConsoleUI::displayMainMenu() {
    showSectionHeader("Main Menu");
    std::cout << std::format(
        "1. Finance Department Access\n"
        "2. Admin Department Access\n"
        "3. Student/Teacher Login\n"
        "-----------------------------\n"
        "0. Exit Application"
    ) << std::endl;
}

void ConsoleUI::displayStudentMenu() {
     showSectionHeader("Student Menu");
     std::cout << std::format(
        "1. View Result Report\n"
        "2. View Personal Details\n"
        "3. View Enrolled Courses\n"
        "4. Change Password\n"
        "-----------------------------\n"
        "0. Logout"
     ) << std::endl;
     // Add more options: Enroll/Drop courses (via Enrollment Service)
}

void ConsoleUI::displayTeacherMenu() {
     showSectionHeader("Teacher Menu");
     std::cout << std::format(
        "1. View Personal Details\n"
        "2. Enter Marks for a Course\n"
        "3. Change Password\n"
        "-----------------------------\n"
        "0. Logout"
     ) << std::endl;
     // Add options: View assigned courses, Enter marks for courses
}

void ConsoleUI::displayAdminTopMenu() {
    showSectionHeader("Admin Main Menu");
    std::cout << std::format(
        "1. Manage Students\n"
        "2. Manage Teachers\n"
        "3. Manage Faculties (Departments/Schools)\n"
        "4. Manage Courses\n"
        "-----------------------------\n"
        "0. Logout"
    ) << std::endl;
    // Add more admin tasks: view logs, system settings?
}

void ConsoleUI::displayAdminStudentMenu() {
     showSectionHeader("Admin - Manage Students");
     std::cout << std::format(
        "1. Add New Student\n"
        "2. Find Student by ID\n"
        "3. List Students by Faculty\n"
        "4. List All Students\n"
        "5. Update Student Details\n"
        "6. Remove Student\n"
        "-----------------------------\n"
        "0. Back to Admin Main Menu"
     ) << std::endl;
}

void ConsoleUI::displayAdminTeacherMenu() {
      showSectionHeader("Admin - Manage Teachers");
      std::cout << std::format(
        "1. Add New Teacher\n"
        "2. Find Teacher by ID\n"
        "3. List Teachers by Faculty\n"
        "4. List All HODs\n"
        "5. List All Teachers\n"
        "6. Update Teacher Details\n"
        "7. Remove Teacher\n"
        "-----------------------------\n"
        "0. Back to Admin Main Menu"
      ) << std::endl;
}
void ConsoleUI::displayAdminFacultyMenu() {
      showSectionHeader("Admin - Manage Faculties (Depts/Schools)");
      std::cout << std::format(
        "1. Add New Faculty\n"
        "2. Find Faculty by ID\n"
        "3. List All Faculties\n"
        "4. Update Faculty Details\n"
        "5. Remove Faculty\n"
        "-----------------------------\n"
        "0. Back to Admin Main Menu"
      ) << std::endl;
}
void ConsoleUI::displayAdminCourseMenu() {
      showSectionHeader("Admin - Manage Courses");
      std::cout << std::format(
        "1. Add New Course\n"
        "2. Find Course by ID\n"
        "3. List Courses by Faculty\n"
        "4. List All Courses\n"
        "5. Update Course Details\n"
        "6. Remove Course\n"
        "-----------------------------\n"
        "0. Back to Admin Main Menu"
      ) << std::endl;
      // Careful! Checks needed.
}


void ConsoleUI::displayFinanceLoginMenu() {
     showSectionHeader("Finance Department - User Login");
     std::cout << std::format(
        "Select user type to access their financial records:\n"
        "1. Student (for Fee Operations)\n"
        "2. Teacher (for Salary Operations)\n"
        "-----------------------------\n"
        "0. Back to Main Menu"
     ) << std::endl;
}

void ConsoleUI::displayFinanceStudentMenu() {
    showSectionHeader("Finance - Student Fee Operations");
    std::cout << std::format(
        "1. View Fee Record\n"
        "2. Make Fee Payment\n"
        "3. Generate Fee Receipt\n"
        "4. Set/Update Total Fee\n"
        "-----------------------------\n"
        "0. Back to Finance Login"
    ) << std::endl;
}

void ConsoleUI::displayFinanceTeacherMenu() {
    showSectionHeader("Finance - Teacher Salary Operations");
    std::cout << std::format(
        "1. View Salary Record\n"
        "2. Generate Salary Certificate\n"
        "3. Set/Update Basic Salary\n"
        "4. View Calculated Annual Pay\n"
        "-----------------------------\n"
        "0. Back to Finance Login"
    ) << std::endl;
}

// --- Action Handler Methods ---

void ConsoleUI::handleMainMenuChoice(MainMenuOption choice) {
    switch (choice) {
        case MainMenuOption::FINANCE:
            // Finance access requires its own login? Or assumes admin logged in?
            // Let's assume Finance staff needs to access records of S/T
            handleFinanceActions();
            break;
        case MainMenuOption::ADMIN:
             if (performLogin("Admin Login", UserRole::ADMIN)) {
                 handleAdminActions();
                 performLogout(); // Logout after finishing admin tasks
             }
            break;
        case MainMenuOption::STUDENT_FACULTY_LOGIN:
            {
                // Use allowAny=true, the role is determined by the login repo
                if (performLogin("Student/Teacher Login", UserRole::STUDENT, true)) {
                     auto role = _authService->getCurrentUserRole();
                     if (role == UserRole::STUDENT) {
                         handleStudentActions();
                     } else if (role == UserRole::TEACHER) {
                         handleTeacherActions();
                     } else {
                         std::cout << "Login successful, but role is not Student or Teacher." << std::endl;
                         LOG_ERROR("User logged in via Student/Teacher portal has unexpected role: " + _authService->getCurrentUserId().value_or("N/A"));
                     }
                     performLogout(); // Logout when done
                 }
            }
            break;
        case MainMenuOption::EXIT:
            // Handled in run loop
            break;
        default:
            std::cout << "Invalid main menu option." << std::endl;
            LOG_WARN("Invalid main menu option selected: " + std::to_string(static_cast<int>(choice)));
            break;
    }
}

bool ConsoleUI::performLogin(const std::string& prompt, UserRole expectedRole, bool allowAny) {
     showSectionHeader(prompt);
     std::string userId = promptForString("Enter User ID: ");
     std::string password = getMaskedPassword("Enter Password: "); // Use masked input

     auto roleOpt = _authService->login(userId, password);

     if (roleOpt) {
         if (allowAny || roleOpt.value() == expectedRole) {
             std::cout << "\nLogin Successful. Welcome " << userId << "!" << std::endl;
             return true;
         } else {
             std::cout << "\nLogin Failed: Access denied for this role." << std::endl;
             LOG_WARN("Login attempt for user " + userId + " failed: Role mismatch.");
             _authService->logout(); // Log out immediately if role doesn't match expected
             return false;
         }
     } else {
         std::cout << "\nLogin Failed: Invalid User ID or Password." << std::endl;
         return false;
     }
}

void ConsoleUI::performLogout() {
    _authService->logout();
    std::cout << "\nYou have been logged out." << std::endl;
}


void ConsoleUI::handleStudentActions() {
     std::string studentId = _authService->getCurrentUserId().value_or("");
     if (studentId.empty()) {
         LOG_ERROR("Cannot handle student actions: No student ID available after login.");
         return;
     }

     bool studentMenuRunning = true;
     while(studentMenuRunning) {
         clearScreen();
         displayStudentMenu();
         int choice = getMenuChoice(0, 4); // Adjust max based on options

         switch(choice) {
             case 1: // View Result
                 showStudentResultReport(studentId);
                 pauseExecution();
                 break;
             case 2: // View Details
                 showStudentDetails(studentId);
                  pauseExecution();
                 break;
            case 3: // View Enrolled Courses
                {
                    auto courses = _enrollmentService->getEnrolledCourses(studentId);
                    showCourseList(courses);
                    pauseExecution();
                }
                break;
             case 4: // Change Password
                {
                    std::string oldPass = getMaskedPassword("Enter OLD Password: ");
                    std::string newPass = getMaskedPassword("Enter NEW Password: ");
                    std::string confirmPass = getMaskedPassword("Confirm NEW Password: ");
                    if (newPass != confirmPass) {
                        std::cout << "New passwords do not match. Password not changed." << std::endl;
                    } else if (_authService->changePassword(oldPass, newPass)) {
                        std::cout << "Password changed successfully." << std::endl;
                    } else {
                        std::cout << "Password change failed. Check old password and complexity rules." << std::endl;
                    }
                    pauseExecution();
                }
                 break;
             case 0: // Logout
                 studentMenuRunning = false;
                 break;
             default:
                 std::cout << "Invalid choice." << std::endl;
                  pauseExecution();
                 break;
         }
     }
}

void ConsoleUI::handleTeacherActions() {
    std::string teacherId = _authService->getCurrentUserId().value_or("");
    if (teacherId.empty()) {
         LOG_ERROR("Cannot handle teacher actions: No teacher ID available after login.");
         return;
     }

     bool teacherMenuRunning = true;
     while(teacherMenuRunning) {
         clearScreen();
         displayTeacherMenu();
         int choice = getMenuChoice(0, 3); // Adjust max option

         switch(choice) {
             case 1: // View Details
                 showTeacherDetails(teacherId);
                 pauseExecution();
                 break;
            case 2: // Enter Marks
                {
                    std::string courseId = promptForString("Enter Course ID to enter marks for: ");
                    std::string studentId = promptForString("Enter Student ID: ");
                    int marks = static_cast<int>(promptForLong("Enter Marks (0-100, -1 if ungraded): "));

                    if (_resultService->enterMarks(studentId, courseId, marks)) {
                        std::cout << "Marks entered successfully." << std::endl;
                    } else {
                        std::cout << "Failed to enter marks. Check if student is enrolled and marks are valid." << std::endl;
                    }
                     pauseExecution();
                }
                break;
            case 3: // Change Password
                {
                    std::string oldPass = getMaskedPassword("Enter OLD Password: ");
                    std::string newPass = getMaskedPassword("Enter NEW Password: ");
                    std::string confirmPass = getMaskedPassword("Confirm NEW Password: ");
                     if (newPass != confirmPass) {
                         std::cout << "New passwords do not match. Password not changed." << std::endl;
                     } else if (_authService->changePassword(oldPass, newPass)) {
                         std::cout << "Password changed successfully." << std::endl;
                     } else {
                         std::cout << "Password change failed. Check old password and complexity rules." << std::endl;
                     }
                     pauseExecution();
                }
                 break;

             case 0: // Logout
                 teacherMenuRunning = false;
                 break;
             default:
                  std::cout << "Invalid choice." << std::endl;
                  pauseExecution();
                 break;
         }
     }
}


void ConsoleUI::handleAdminActions() {
     bool adminMenuRunning = true;
     while(adminMenuRunning) {
         clearScreen();
         displayAdminTopMenu();
         int choice = getMenuChoice(0, 4); // Adjust max options

         switch(choice) {
             case 1: handleAdminStudentActions(); break;
             case 2: handleAdminTeacherActions(); break;
             case 3: handleAdminFacultyActions(); break;
             case 4: handleAdminCourseActions(); break;
             case 0: adminMenuRunning = false; break;
             default: std::cout << "Invalid choice." << std::endl; pauseExecution(); break;
         }
     }
}

// Implement handleAdminStudentActions, handleAdminTeacherActions, etc.
// These will display their respective submenus and call the appropriate *Service* methods
// Example:
void ConsoleUI::handleAdminStudentActions() {
     bool running = true;
     while(running) {
         clearScreen();
         displayAdminStudentMenu();
         int choice = getMenuChoice(0, 6);
         switch(choice) {
             case 1: /* Add Student Logic */
                {
                     std::cout << "-- Add New Student --\n";
                     std::string id = promptForString("Enter Student ID: ");
                     std::string fname = promptForString("Enter First Name: ");
                     std::string lname = promptForString("Enter Last Name: ");
                     std::string cid = promptForString("Enter Citizen ID: ");
                     std::string facId = promptForString("Enter Faculty ID: ");
                     // Add prompts for email, phone, address, birthday...
                     std::string email = promptForString("Enter Email: ");
                     std::string phone = promptForString("Enter Phone: ");
                     std::string address = promptForString("Enter Address: ");
                     std::string bday = promptForString("Enter Birthday (dd/mm/yyyy): ");
                     std::string password = getMaskedPassword("Enter Initial Password: ");

                     Student newStudent(
                        id, fname, lname, cid, facId, email, phone, address
                     );

                     if (!newStudent.birthday().setBirthday(bday)) {
                         std::cout << "Warning: Invalid birthday format. Birthday not set." << std::endl;
                     }

                     if (_studentService->addStudent(newStudent, password)) {
                         std::cout << "Student added successfully." << std::endl;
                     } else {
                         std::cout << "Failed to add student. Check logs for details." << std::endl;
                     }
                }
                 break;
             case 2: /* Find Student */
                 {
                     std::string id = promptForString("Enter Student ID to find: ");
                     showStudentDetails(id);
                 }
                 break;
             case 3: /* List by Faculty */
                 {
                     std::string facId = promptForString("Enter Faculty ID: ");
                     auto students = _adminService->getStudentsByFaculty(facId); // Use AdminService view
                     showStudentList(students);
                 }
                 break;
             case 4: /* List All */
                  showStudentList(_adminService->getAllStudents());
                  break;
             case 5: /* Update Student */
                   std::cout << "Update student: Find student by ID first, then provide new details (Not fully implemented).\n";
                   // Requires getting existing student, prompting for changes, then calling updateStudentDetails
                   break;
             case 6: /* Remove Student */
                 {
                     std::string id = promptForString("Enter Student ID to remove: ");
                     if (promptForYesNo("Are you sure you want to remove student " + id + "? This is irreversible!")) {
                        if (_studentService->removeStudent(id)) { // Use StudentService for removal logic
                             std::cout << "Student removed successfully." << std::endl;
                        } else {
                             std::cout << "Failed to remove student. Check logs." << std::endl;
                        }
                     } else {
                          std::cout << "Removal cancelled." << std::endl;
                     }
                 }
                 break;
             case 0: running = false; break;
             default: std::cout << "Invalid choice." << std::endl; break;
         }
          if (running) pauseExecution();
     }
}
// TODO: Implement handleAdminTeacherActions, handleAdminFacultyActions, handleAdminCourseActions similarly


void ConsoleUI::handleFinanceActions() {
     bool financeMenuRunning = true;
     while(financeMenuRunning) {
         clearScreen();
         displayFinanceLoginMenu();
         int choice = getMenuChoice(0, 2);

         switch(choice) {
             case 1: // Access Student Records
                {
                     std::string studentId = promptForString("Enter Student ID to access Finance Records: ");
                     // Validate student exists before proceeding
                     if (_studentService->getStudentDetails(studentId)) {
                         handleFinanceStudentActions(studentId);
                     } else {
                         std::cout << "Student ID not found." << std::endl;
                         pauseExecution();
                     }
                }
                 break;
             case 2: // Access Teacher Records
                 {
                      std::string teacherId = promptForString("Enter Teacher ID to access Finance Records: ");
                      if (_teacherService->getTeacherDetails(teacherId)) {
                          handleFinanceTeacherActions(teacherId);
                      } else {
                          std::cout << "Teacher ID not found." << std::endl;
                          pauseExecution();
                      }
                 }
                 break;
             case 0: // Back to Main Menu
                 financeMenuRunning = false;
                 break;
             default:
                 std::cout << "Invalid choice." << std::endl;
                 pauseExecution();
                 break;
         }
     }
}

void ConsoleUI::handleFinanceStudentActions(const std::string& studentId) {
      bool running = true;
      while(running) {
          clearScreen();
          showSectionHeader("Finance - Student: " + studentId);
          displayFinanceStudentMenu();
          int choice = getMenuChoice(0, 4);

          switch(choice) {
              case 1: // View Record
                    {
                         auto recordOpt = _financeService->getStudentFeeRecord(studentId);
                         if(recordOpt) recordOpt.value().display();
                         else std::cout << "No fee record found for student " << studentId << std::endl;
                    }
                  break;
              case 2: // Make Payment
                  {
                      long amount = promptForLong("Enter amount to pay: ");
                      if (_financeService->makeFeePayment(studentId, amount)) {
                           std::cout << "Payment processed successfully." << std::endl;
                      } else {
                           std::cout << "Payment failed. Check amount or logs." << std::endl;
                      }
                  }
                  break;
              case 3: // Generate Receipt
                    {
                        auto receiptOpt = _financeService->generateFeeReceipt(studentId);
                        if(receiptOpt) std::cout << receiptOpt.value() << std::endl;
                        else std::cout << "Could not generate receipt (record not found?)." << std::endl;
                    }
                  break;
               case 4: // Set Total Fee
                    {
                        long totalFee = promptForLong("Enter new total fee: ");
                         if (_financeService->setStudentTotalFee(studentId, totalFee)) {
                              std::cout << "Total fee updated successfully." << std::endl;
                         } else {
                              std::cout << "Failed to update total fee." << std::endl;
                         }
                    }
                    break;
              case 0: running = false; break; // Back to Finance Login
              default: std::cout << "Invalid choice." << std::endl; break;
          }
           if (running) pauseExecution();
      }
}


void ConsoleUI::handleFinanceTeacherActions(const std::string& teacherId) {
      bool running = true;
       while(running) {
           clearScreen();
           showSectionHeader("Finance - Teacher: " + teacherId);
           displayFinanceTeacherMenu();
           int choice = getMenuChoice(0, 4);

           switch(choice) {
               case 1: // View Record
                    {
                         auto recordOpt = _financeService->getTeacherSalaryRecord(teacherId);
                         if(recordOpt) recordOpt.value().display();
                         else std::cout << "No salary record found for teacher " << teacherId << std::endl;
                    }
                   break;
               case 2: // Generate Certificate
                    {
                         auto certOpt = _financeService->generateSalaryCertificate(teacherId);
                         if(certOpt) std::cout << certOpt.value() << std::endl;
                         else std::cout << "Could not generate certificate (record not found?)." << std::endl;
                    }
                   break;
               case 3: // Set Basic Salary
                   {
                       long basicPay = promptForLong("Enter new basic monthly salary: ");
                       if (_financeService->setTeacherBasicSalary(teacherId, basicPay)) {
                           std::cout << "Basic salary updated successfully." << std::endl;
                       } else {
                           std::cout << "Failed to update basic salary." << std::endl;
                       }
                   }
                   break;
                case 4: // View Calculated Annual Pay
                    {
                         auto annualPayOpt = _financeService->calculateTeacherAnnualPay(teacherId);
                         if (annualPayOpt) {
                              std::cout << "Calculated Total Annual Pay (Approx): " << annualPayOpt.value() << " Rs." << std::endl;
                         } else {
                              std::cout << "Could not calculate annual pay (salary record not found?)." << std::endl;
                         }
                    }
                    break;
               case 0: running = false; break; // Back to Finance Login
               default: std::cout << "Invalid choice." << std::endl; break;
           }
            if (running) pauseExecution();
       }
}

// --- Data Display Helpers ---

void ConsoleUI::showSectionHeader(const std::string& title) {
    drawHeader(title, '-');
    std::cout << std::endl;
}

void ConsoleUI::showStudentDetails(const std::string& studentId) {
    auto studentOpt = _studentService->getStudentDetails(studentId);
    if (studentOpt) {
        studentOpt.value().display(); // Use the entity's display method
    } else {
        std::cout << std::format("Student with ID '{}' not found.", studentId) << std::endl;
    }
}
void ConsoleUI::showTeacherDetails(const std::string& teacherId) {
    auto teacherOpt = _teacherService->getTeacherDetails(teacherId);
    if (teacherOpt) {
        teacherOpt.value().display();
    } else {
        std::cout << std::format("Teacher with ID '{}' not found.", teacherId) << std::endl;
    }
}
void ConsoleUI::showFacultyDetails(const std::string& facultyId) {
     auto facultyOpt = _facultyService->getFacultyById(facultyId);
     if (facultyOpt) {
         facultyOpt.value().display();
     } else {
         std::cout << std::format("Faculty/Department with ID '{}' not found.", facultyId) << std::endl;
     }
}
void ConsoleUI::showCourseDetails(const std::string& courseId) {
      auto courseOpt = _courseService->getCourseById(courseId);
      if (courseOpt) {
          courseOpt.value().display();
      } else {
          std::cout << std::format("Course with ID '{}' not found.", courseId) << std::endl;
      }
}

void ConsoleUI::showStudentResultReport(const std::string& studentId) {
     std::string report = _resultService->generateResultReport(studentId);
     std::cout << report << std::endl;
}

void ConsoleUI::showFeeReceipt(const std::string& studentId) {
     auto receiptOpt = _financeService->generateFeeReceipt(studentId);
     if (receiptOpt) {
         std::cout << receiptOpt.value() << std::endl;
     } else {
         std::cout << std::format("Could not generate fee receipt for student {}.", studentId) << std::endl;
     }
}

void ConsoleUI::showSalaryCertificate(const std::string& teacherId) {
      auto certOpt = _financeService->generateSalaryCertificate(teacherId);
      if (certOpt) {
          std::cout << certOpt.value() << std::endl;
      } else {
          std::cout << std::format("Could not generate salary certificate for teacher {}.", teacherId) << std::endl;
      }
}

// --- List Display Helpers ---

void ConsoleUI::showStudentList(const std::vector<Student>& students) {
     showSectionHeader("Student List");
     if (students.empty()) {
         std::cout << std::format("No students found.") << std::endl;
         return;
     }
     
     // Header and separator
     std::cout << std::format("{:<15}{:<30}{:<15}{:<30}", "ID", "Name", "Faculty ID", "Email") << std::endl;
     std::cout << std::string(90, '-') << std::endl;
     
     // Data rows
     for (const auto& s : students) {
         std::cout << std::format("{:<15}{:<30}{:<15}{:<30}",
             s.id(),
             s.fullName().substr(0, 28),
             s.facultyId(),
             s.email().substr(0, 28)) << std::endl;
     }
     
     std::cout << std::string(90, '-') << std::endl;
}

void ConsoleUI::showTeacherList(const std::vector<Teacher>& teachers) {
     showSectionHeader("Teacher List");
     if (teachers.empty()) {
         std::cout << std::format("No teachers found.") << std::endl;
         return;
     }
     
     // Header and separator
     std::cout << std::format("{:<15}{:<30}{:<15}{:<20}", "ID", "Name", "Faculty ID", "Designation") << std::endl;
     std::cout << std::string(80, '-') << std::endl;
     
     // Data rows
     for (const auto& t : teachers) {
         std::cout << std::format("{:<15}{:<30}{:<15}{:<20}",
             t.id(),
             t.fullName().substr(0, 28),
             t.facultyId(),
             t.designation().substr(0, 18)) << std::endl;
     }
     
     std::cout << std::string(80, '-') << std::endl;
}

 void ConsoleUI::showFacultyList(const std::vector<Faculty>& faculties) {
     showSectionHeader("Faculty (Department/School) List");
     if (faculties.empty()) {
         std::cout << std::format("No faculties found.") << std::endl;
         return;
     }
     
     // Header and separator
     std::cout << std::format("{:<15}{:<40}", "ID", "Name") << std::endl;
     std::cout << std::string(55, '-') << std::endl;
     
     // Data rows
     for (const auto& f : faculties) {
         std::cout << std::format("{:<15}{:<40}", 
             f.id(), 
             f.name().substr(0, 38)) << std::endl;
     }
     
     std::cout << std::string(55, '-') << std::endl;
 }

 void ConsoleUI::showCourseList(const std::vector<Course>& courses) {
     showSectionHeader("Course List");
     if (courses.empty()) {
         std::cout << std::format("No courses found.") << std::endl;
         return;
     }
     
     // Header and separator
     std::cout << std::format("{:<12}{:<40}{:<10}{:<15}", "ID", "Name", "Credits", "Faculty ID") << std::endl;
     std::cout << std::string(77, '-') << std::endl;
     
     // Data rows
     for (const auto& c : courses) {
         std::cout << std::format("{:<12}{:<40}{:<10}{:<15}",
             c.id(),
             c.name().substr(0, 38),
             c.credits(),
             c.facultyId()) << std::endl;
     }
     
     std::cout << std::string(77, '-') << std::endl;
 }