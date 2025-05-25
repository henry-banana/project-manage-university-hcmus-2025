#include <iostream>
#include <string>
#include <memory>
#include <stdexcept> // For std::runtime_error

// Common includes
#include "common/AppConfig.h"
#include "common/ErrorType.h" // Dù không dùng trực tiếp ở main, nhưng cần cho các thành phần khác
#include "common/UserRole.h"
#include "common/LoginStatus.h"

// Utilities
#include "utils/ConfigLoader.h"
#include "utils/Logger.h"

// Core components
#include "core/data_access/DaoFactory.h"
#include "core/validators/impl/GeneralInputValidator.h" // Triển khai cụ thể
#include "core/services/SessionContext.h"

// Service implementations
#include "core/services/impl/AuthService.h"
#include "core/services/impl/StudentService.h"
#include "core/services/impl/TeacherService.h"
#include "core/services/impl/FacultyService.h"
#include "core/services/impl/CourseService.h"
#include "core/services/impl/EnrollmentService.h"
#include "core/services/impl/ResultService.h"
#include "core/services/impl/FinanceService.h"
#include "core/services/impl/AdminService.h"

// UI
#include "ui/ConsoleUI.h"


int main(int argc, char* argv[]) {
    // Xử lý tham số dòng lệnh cơ bản (ví dụ: đường dẫn file config)
    std::string configFilePath = "config/app_config.ini"; // Mặc định
    if (argc > 1) {
        configFilePath = argv[1];
    }

    // 1. Load AppConfig
    AppConfig appConfig; // Vẫn khởi tạo default
    ConfigLoader configLoader(configFilePath);
    auto configLoadResult = configLoader.loadConfig();

    if (configLoadResult.has_value()) {
        appConfig = configLoadResult.value(); // (➕) Dùng .value()
    } else {
        // Không load được config, dùng default hoặc báo lỗi và thoát
        std::cerr << "WARNING: Could not load application configuration from '" << configFilePath 
                  << "'. Error: " << configLoadResult.error().message << std::endl;
        
        appConfig.dataSourceType = DataSourceType::MOCK; // Mặc định an toàn
        appConfig.logLevel = Logger::Level::DEBUG;       // Log nhiều hơn khi lỗi config
        appConfig.logFilePath = "logs/app_default_error.log";
        if (appConfig.dataSourceType == DataSourceType::SQL) {
            // Cần một đường dẫn mặc định hợp lý hoặc yêu cầu người dùng cung cấp
            appConfig.sqlConnectionString = "database/university.db"; 
        }
        std::cerr << "Using emergency default configuration with "
                  << (appConfig.dataSourceType == DataSourceType::MOCK ? "Mock DAOs." : 
                     (appConfig.dataSourceType == DataSourceType::SQL ? "SQL DAOs (default DB path)." : "Unknown DAOs.")) 
                  << std::endl;
    }

    // 2. Configure Logger
    // Logger được tạo là singleton, getInstance() sẽ tự khởi tạo nếu cần
    try {
        Logger::getInstance().configure(appConfig.logLevel, appConfig.logFilePath.string());
    } catch (const std::exception& e) {
        std::cerr << "FATAL: Failed to configure logger: " << e.what() << std::endl;
        // Không thể ghi log nếu logger lỗi, nên chỉ in ra cerr
        return 1; // Thoát nếu logger không thể khởi tạo
    }
    
    // std::cin.get(); // Đảm bảo cin không bị lỗi khi đọc dòng đầu tiên
    
    LOG_INFO("============================================================");
    LOG_INFO("University Management System - Application Starting...");
    LOG_INFO("Configuration loaded. Data Source: " + 
             std::string(appConfig.dataSourceType == DataSourceType::SQL ? "SQL" : 
                        std::string(appConfig.dataSourceType == DataSourceType::MOCK ? "Mock" : "CSV (if implemented)")) +
             ". Log Level: " + Logger::getInstance().levelToString(appConfig.logLevel)); // Lấy level hiện tại từ logger
    LOG_INFO("============================================================");


    try {
        // 3. Khởi tạo các thành phần cốt lõi và dependencies
        LOG_DEBUG("Initializing core components...");

        auto sessionContext = std::make_shared<SessionContext>();
        LOG_DEBUG("SessionContext initialized.");

        auto generalInputValidator = std::make_shared<GeneralInputValidator>();
        LOG_DEBUG("GeneralInputValidator initialized.");

        // Tạo các DAO thông qua DaoFactory
        // DaoFactory sẽ tự quản lý DatabaseAdapter nếu dùng SQL
        LOG_DEBUG("Initializing DAOs via DaoFactory...");
        auto studentDao = DaoFactory::createStudentDao(appConfig);
        auto teacherDao = DaoFactory::createTeacherDao(appConfig);
        auto facultyDao = DaoFactory::createFacultyDao(appConfig);
        auto courseDao = DaoFactory::createCourseDao(appConfig);
        auto loginDao = DaoFactory::createLoginDao(appConfig);
        auto enrollmentDao = DaoFactory::createEnrollmentDao(appConfig);
        auto courseResultDao = DaoFactory::createCourseResultDao(appConfig);
        auto feeRecordDao = DaoFactory::createFeeRecordDao(appConfig);
        auto salaryRecordDao = DaoFactory::createSalaryRecordDao(appConfig);
        LOG_INFO("DAOs initialized successfully.");

        // Tạo các Service, inject dependencies
        LOG_DEBUG("Initializing Services...");
        auto authService = std::make_shared<AuthService>(
            loginDao, 
            studentDao, 
            teacherDao, // AuthService có dùng teacherDao để check email khi student đăng ký
            generalInputValidator, 
            sessionContext
        );

        auto studentService = std::make_shared<StudentService>(
            studentDao, 
            teacherDao,     // (➕) Truyền teacherDao vào đây
            facultyDao, 
            generalInputValidator, 
            sessionContext
        );

        auto teacherService = std::make_shared<TeacherService>(
            teacherDao, 
            studentDao,     // TeacherService dùng studentDao để check email trùng khi admin/teacher cập nhật email teacher
            facultyDao, 
            generalInputValidator, 
            sessionContext
        );

        auto facultyService = std::make_shared<FacultyService>(
            facultyDao, 
            studentDao,     // FacultyService dùng để check ràng buộc khi xóa faculty
            teacherDao,     // FacultyService dùng để check ràng buộc khi xóa faculty
            courseDao,      // FacultyService dùng để check ràng buộc khi xóa faculty
            generalInputValidator, 
            sessionContext
        );

        auto courseService = std::make_shared<CourseService>(
            courseDao, 
            facultyDao, 
            enrollmentDao,  // CourseService dùng để check ràng buộc khi xóa course
            courseResultDao,// CourseService dùng để check ràng buộc khi xóa course
            generalInputValidator, 
            sessionContext
        );

        auto enrollmentService = std::make_shared<EnrollmentService>(
            enrollmentDao, 
            studentDao, 
            courseDao, 
            generalInputValidator, 
            sessionContext
        );

        auto resultService = std::make_shared<ResultService>(
            courseResultDao, 
            facultyDao,     // (➕) Truyền facultyDao vào đây (theo constructor của bạn)
            studentDao, 
            courseDao, 
            enrollmentDao, 
            generalInputValidator, 
            sessionContext
        );

        auto financeService = std::make_shared<FinanceService>(
            feeRecordDao, 
            salaryRecordDao, 
            studentDao, 
            teacherDao,
            facultyDao,     // (➕) Truyền facultyDao vào đây (theo constructor của bạn)
            generalInputValidator, 
            sessionContext
        );

        auto adminService = std::make_shared<AdminService>(
            studentDao, 
            teacherDao, 
            loginDao, 
            feeRecordDao, 
            salaryRecordDao, 
            enrollmentDao, 
            courseResultDao, 
            generalInputValidator, 
            sessionContext
        );
        LOG_INFO("Services initialized successfully.");

        // 4. Khởi tạo ConsoleUI
        LOG_DEBUG("Initializing ConsoleUI...");
        ConsoleUI consoleUI(
            authService, studentService, teacherService, facultyService,
            courseService, enrollmentService, resultService, financeService, adminService
        );
        LOG_INFO("ConsoleUI initialized. Starting UI run loop...");

        // 5. Chạy ứng dụng UI
        consoleUI.run();

    } catch (const std::runtime_error& e) {
        LOG_CRITICAL("Runtime error during application initialization or execution: " + std::string(e.what()));
        std::cerr << "CRITICAL RUNTIME ERROR: " << e.what() << std::endl;
        Logger::getInstance().critical("Application terminated due to runtime_error: " + std::string(e.what())); // Log lần cuối
        Logger::releaseInstance(); // Đảm bảo logger được giải phóng
        return 1;
    } catch (const std::exception& e) {
        LOG_CRITICAL("Unhandled C++ standard exception in main: " + std::string(e.what()));
        std::cerr << "CRITICAL UNHANDLED EXCEPTION: " << e.what() << std::endl;
        Logger::getInstance().critical("Application terminated due to std::exception: " + std::string(e.what()));
        Logger::releaseInstance();
        return 1;
    } catch (...) {
        LOG_CRITICAL("Unknown unhandled exception in main!");
        std::cerr << "CRITICAL UNKNOWN EXCEPTION!" << std::endl;
        Logger::getInstance().critical("Application terminated due to unknown exception.");
        Logger::releaseInstance();
        return 1;
    }

    LOG_INFO("============================================================");
    LOG_INFO("University Management System - Application Shutting Down...");
    LOG_INFO("============================================================");
    
    DaoFactory::cleanup(); // Nếu bạn đã triển khai hàm này để đóng DB adapter
    // Hiện tại, DB adapter (SQLiteAdapter) sẽ tự đóng kết nối trong destructor của nó
    // khi _dbAdapterInstance trong DaoFactory được giải phóng (khi chương trình kết thúc).

    Logger::releaseInstance(); // Dọn dẹp Logger singleton
    return 0;
}