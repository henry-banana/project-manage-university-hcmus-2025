#include "core/repo/csv/CsvStudentRepo.h"
#include "core/repo/csv/CsvTeacherRepo.h"
#include "core/repo/csv/CsvFacultyRepo.h"
#include "core/repo/csv/CsvLoginRepo.h"
#include "core/repo/csv/CsvCourseRepo.h"
#include "core/repo/csv/CsvFeeRepo.h"
#include "core/repo/csv/CsvSalaryRepo.h"
#include "core/repo/csv/CsvResultRepo.h"
#include "core/repo/csv/CsvEnrollmentRepo.h" // If needed for enrollment repo
// Include interfaces to allow shared_ptr<Interface>
#include "core/repo/interface/IStudentRepository.h"
#include "core/repo/interface/ITeacherRepository.h"
#include "core/repo/interface/IFacultyRepository.h"
#include "core/repo/interface/ICourseRepository.h"
#include "core/repo/interface/IFeeRepository.h"
#include "core/repo/interface/ISalaryRepository.h"
#include "core/repo/interface/IResultRepository.h"
#include "core/repo/interface/ILoginRepository.h"

#include "core/services/AuthService.cpp" // Include impl for now
#include "core/services/StudentService.cpp"
#include "core/services/TeacherService.cpp"
#include "core/services/FacultyService.cpp"
#include "core/services/CourseService.cpp"
#include "core/services/EnrollmentService.cpp"
#include "core/services/ResultService.cpp"
#include "core/services/FinanceService.cpp"
#include "core/services/AdminService.cpp"
// Include service interfaces
#include "core/services/AuthService.h"
#include "core/services/StudentService.h"
#include "core/services/TeacherService.h"
#include "core/services/FacultyService.h"
#include "core/services/CourseService.h"
#include "core/services/EnrollmentService.h"
#include "core/services/ResultService.h"
#include "core/services/FinanceService.h"
#include "core/services/AdminService.h"


#include "ui/ConsoleUI.h"
#include "utils/Logger.h"
#include "utils/PasswordInput.h" // For password input
#include "utils/ConsoleUtils.h" // For pauseExecution

#include <iostream>
#include <memory> // For std::make_shared

// Define paths for CSV files (adjust as needed)
const std::string DATA_DIR = "data/"; // Assuming a data subdirectory
const std::string STUDENT_CSV = DATA_DIR + "students.csv";
const std::string TEACHER_CSV = DATA_DIR + "teachers.csv";
const std::string FACULTY_CSV = DATA_DIR + "faculties.csv"; // Depts/Schools
const std::string COURSE_CSV = DATA_DIR + "courses.csv";
const std::string FEE_CSV = DATA_DIR + "fees.csv";
const std::string SALARY_CSV = DATA_DIR + "salaries.csv";
const std::string RESULT_CSV = DATA_DIR + "results.csv"; // Stores CourseResult items
const std::string ENROLLMENT_CSV = DATA_DIR + "enrollments.csv"; // student_id, course_id pairs
const std::string LOGIN_CSV = DATA_DIR + "logins.csv";

int main() {
    // Initialize Logger
    Logger::getInstance().setLogLevel(Logger::Level::INFO); // Set desired log level
    // Logger::getInstance().setLogFile(DATA_DIR + "application.log"); // Optional: custom log file path

    LOG_INFO("Application starting...");

    try {
        // 1. Create Repository Instances (using CSV implementations)
        // Use interfaces for pointers to allow easy switching later (e.g., to Database repos)
        auto studentRepo = std::make_shared<CsvStudentRepo>(STUDENT_CSV);
        auto teacherRepo = std::make_shared<CsvTeacherRepo>(TEACHER_CSV);
        auto facultyRepo = std::make_shared<CsvFacultyRepo>(FACULTY_CSV);
        // TODO: Create and include CsvCourseRepo, CsvFeeRepo, CsvSalaryRepo, CsvResultRepo, CsvEnrollmentRepo implementations
         auto loginRepo = std::make_shared<CsvLoginRepo>(LOGIN_CSV);
        //  // Placeholder repos until implemented:

        auto courseRepo = std::make_shared<CsvCourseRepo>(COURSE_CSV); // Assumes CsvCourseRepo exists
        auto feeRepo = std::make_shared<CsvFeeRepo>(FEE_CSV);       // Assumes CsvFeeRepo exists
        auto salaryRepo = std::make_shared<CsvSalaryRepo>(SALARY_CSV); // Assumes CsvSalaryRepo exists
        auto resultRepo = std::make_shared<CsvResultRepo>(RESULT_CSV); // Assumes CsvResultRepo exists

         // Enrollment might be part of CourseRepo or its own CsvEnrollmentRepo(ENROLLMENT_CSV)


        // 2. Create Service Instances, Injecting Repositories
        auto authService = std::make_shared<AuthService>(loginRepo);
        auto studentService = std::make_shared<StudentService>(studentRepo, facultyRepo, authService);
        auto teacherService = std::make_shared<TeacherService>(teacherRepo, facultyRepo, authService);
        auto facultyService = std::make_shared<FacultyService>(facultyRepo, studentRepo, teacherRepo, courseRepo); // Added dependency checks
        auto courseService = std::make_shared<CourseService>(courseRepo, facultyRepo);
        // Enrollment Service needs CourseRepo and StudentRepo
        auto enrollmentService = std::make_shared<EnrollmentService>(courseRepo, studentRepo);
         // Result Service needs ResultRepo, StudentRepo, CourseRepo, EnrollmentService
        auto resultService = std::make_shared<ResultService>(resultRepo, studentRepo, courseRepo, enrollmentService);
        // Finance Service needs FeeRepo, SalaryRepo, StudentRepo, TeacherRepo
        auto financeService = std::make_shared<FinanceService>(feeRepo, salaryRepo, studentRepo, teacherRepo);
         // Admin Service needs view access to various repos
        auto adminService = std::make_shared<AdminService>(studentRepo, teacherRepo, facultyRepo /*, courseRepo */);


        // 3. Create UI Instance, Injecting Services
        ConsoleUI consoleUI(
            authService,
            studentService,
            teacherService,
            facultyService,
            courseService,
            enrollmentService,
            resultService,
            financeService,
            adminService
        );

        // 4. Run the UI
        consoleUI.run();

    } catch (const std::exception& e) {
        LOG_CRITICAL("Unhandled exception caught in main: " + std::string(e.what()));
        std::cerr << "Critical Error: " << e.what() << std::endl;
        std::cerr << "Application will terminate." << std::endl;
        pauseExecution("Press Enter to exit...");
        return 1; // Indicate error exit
    } catch (...) {
        LOG_CRITICAL("Unknown unhandled exception caught in main.");
        std::cerr << "Unknown Critical Error. Application will terminate." << std::endl;
        pauseExecution("Press Enter to exit...");
        return 1; // Indicate error exit
    }


    LOG_INFO("Application finished cleanly.");
    return 0; // Success
}