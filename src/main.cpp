/**
 * @file main.cpp
 * @brief Điểm khởi đầu của ứng dụng Hệ thống Quản lý Trường Đại học
 * 
 * File này chứa hàm main() là điểm khởi đầu của ứng dụng. Nó thực hiện:
 * 1. Đọc cấu hình từ file cấu hình
 * 2. Thiết lập hệ thống ghi log
 * 3. Khởi tạo các thành phần của ứng dụng (DAOs, Services)
 * 4. Khởi chạy giao diện người dùng
 * 5. Xử lý các ngoại lệ toàn cục
 */
#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>
#include <filesystem>

#include "common/AppConfig.h"
#include "common/ErrorType.h"
#include "common/UserRole.h"
#include "common/LoginStatus.h"

#include "utils/ConfigLoader.h"
#include "utils/Logger.h"

#include "core/data_access/DaoFactory.h"
#include "core/validators/impl/GeneralInputValidator.h"
#include "core/services/SessionContext.h"

#include "core/services/impl/AuthService.h"
#include "core/services/impl/StudentService.h"
#include "core/services/impl/TeacherService.h"
#include "core/services/impl/FacultyService.h"
#include "core/services/impl/CourseService.h"
#include "core/services/impl/EnrollmentService.h"
#include "core/services/impl/ResultService.h"
#include "core/services/impl/FinanceService.h"
#include "core/services/impl/AdminService.h"

#include "ui/ConsoleUI.h"

/**
 * @brief Hàm chính khởi chạy ứng dụng
 * 
 * @param argc Số lượng tham số dòng lệnh
 * @param argv Mảng các tham số dòng lệnh
 * @return 0 nếu ứng dụng kết thúc bình thường, khác 0 nếu có lỗi
 */
int main(int argc, char* argv[]) {
    std::string configFilePath = "config/app_config.ini";
    if (argc > 1) {
        configFilePath = argv[1];
    }    
    AppConfig appConfig; 

    ConfigLoader configLoader(configFilePath);
    auto configLoadResult = configLoader.loadConfig();

    if (configLoadResult.has_value()) {
        appConfig = configLoadResult.value();
    } else {
        std::cerr << "WARNING: Could not load application configuration from '" << configFilePath 
                  << "'. Error: " << configLoadResult.error().message << "\n";
        appConfig.dataSourceType = DataSourceType::SQL; 
        appConfig.logLevel = Logger::Level::DEBUG;      
        appConfig.logFilePath = "logs/app_default_error.log"; 
        if (appConfig.dataSourceType == DataSourceType::SQL) {
            appConfig.sqlConnectionString = "database/university_default.db"; 
        }
        std::cerr << "Using emergency default configuration. Check log for details if created.\n";
    }

    try {
        // Cấu hình hệ thống log
        std::filesystem::path configuredLogPath(appConfig.logFilePath);
        std::filesystem::path logDirToCreate; 

        if (configuredLogPath.has_parent_path() && !configuredLogPath.parent_path().empty() && configuredLogPath.parent_path().string() != ".") {
            logDirToCreate = configuredLogPath.parent_path();
        } else {
            logDirToCreate = std::filesystem::current_path() / "logs";
        }
        std::string baseName = configuredLogPath.stem().string();
        if (baseName.empty() && !configuredLogPath.filename().empty() && configuredLogPath.extension().empty()){
            baseName = configuredLogPath.filename().string();
        } else if (baseName.empty()){
            baseName = "application"; 
        }
        std::string extension = configuredLogPath.extension().string();
        if (extension.empty()) {
            extension = ".log"; 
        }

        if (!std::filesystem::exists(logDirToCreate)) {
            std::cout << "[Main Info] Attempting to create log directory: " << logDirToCreate.string() << "\n";
            if (!std::filesystem::create_directories(logDirToCreate)) {
                 std::cerr << "WARNING: Could not create log directory: " << logDirToCreate.string() << "\n";
                 logDirToCreate = std::filesystem::current_path(); 
            }
        }
        
        Logger::getInstance().configure(appConfig.logLevel, logDirToCreate, baseName, extension);

    } catch (const std::exception& e) {
        std::cerr << "FATAL: Failed during logger configuration phase: " << e.what() << "\n";
        return 1; 
    }
    
    LOG_INFO("============================================================");
    LOG_INFO("University Management System - Application Starting...");
    LOG_INFO("Configuration loaded. Data Source: " + 
             std::string(appConfig.dataSourceType == DataSourceType::SQL ? "SQL" : 
                        (appConfig.dataSourceType == DataSourceType::MOCK ? "Mock" : "CSV (if implemented)")) +
             ". Log Level: " + Logger::getInstance().levelToString(appConfig.logLevel));
    LOG_INFO("============================================================");

    try {
        // Khởi tạo các thành phần cốt lõi
        LOG_DEBUG("Initializing core components...");
        auto sessionContext = std::make_shared<SessionContext>();        
        LOG_DEBUG("SessionContext initialized.");
        auto generalInputValidator = std::make_shared<GeneralInputValidator>();        
        LOG_DEBUG("GeneralInputValidator initialized.");
        
        // Khởi tạo các DAO thông qua Factory
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

        // Khởi tạo các Service
        LOG_DEBUG("Initializing Services...");
        auto authService = std::make_shared<AuthService>(
            loginDao, 
            studentDao, 
            facultyDao,   
            teacherDao, 
            generalInputValidator, 
            sessionContext
        );   

        auto studentService = std::make_shared<StudentService>(
            studentDao, teacherDao, facultyDao, generalInputValidator, sessionContext
        );
        
        auto teacherService = std::make_shared<TeacherService>(teacherDao, studentDao, facultyDao, generalInputValidator, sessionContext);
        auto facultyService = std::make_shared<FacultyService>(facultyDao, studentDao, teacherDao, courseDao, generalInputValidator, sessionContext);
        auto courseService = std::make_shared<CourseService>(courseDao, facultyDao, enrollmentDao, courseResultDao, generalInputValidator, sessionContext);
        auto enrollmentService = std::make_shared<EnrollmentService>(enrollmentDao, studentDao, courseDao, generalInputValidator, sessionContext);
        auto resultService = std::make_shared<ResultService>(courseResultDao, facultyDao, studentDao, courseDao, enrollmentDao, generalInputValidator, sessionContext);
        auto financeService = std::make_shared<FinanceService>(feeRecordDao, salaryRecordDao, studentDao, teacherDao, facultyDao, generalInputValidator, sessionContext);
        auto adminService = std::make_shared<AdminService>(
            studentDao, teacherDao, facultyDao,loginDao, feeRecordDao, salaryRecordDao, enrollmentDao, courseResultDao, generalInputValidator, sessionContext
        );        
        LOG_INFO("Services initialized successfully.");

        // Khởi tạo giao diện người dùng
        LOG_DEBUG("Initializing ConsoleUI...");
        ConsoleUI consoleUI(
            authService, studentService, teacherService, facultyService,
            courseService, enrollmentService, resultService, financeService, adminService
        );        
        LOG_INFO("ConsoleUI initialized. Starting UI run loop...");
        
        // Chạy vòng lặp chính của ứng dụng
        consoleUI.run();    

    } catch (const std::runtime_error& e) {
        LOG_CRITICAL("Runtime error during application initialization or execution: " + std::string(e.what()));
        std::cerr << "CRITICAL RUNTIME ERROR: " << e.what() << "\n";
        Logger::getInstance().critical("Application terminated due to runtime_error: " + std::string(e.what())); 
        Logger::releaseInstance(); 
        return 1;    
    } catch (const std::exception& e) {
        LOG_CRITICAL("Unhandled C++ standard exception in main: " + std::string(e.what()));
        std::cerr << "CRITICAL UNHANDLED EXCEPTION: " << e.what() << "\n";
        Logger::getInstance().critical("Application terminated due to std::exception: " + std::string(e.what()));
        Logger::releaseInstance();
        return 1;    
    } catch (...) {
        LOG_CRITICAL("Unknown unhandled exception in main!");
        std::cerr << "CRITICAL UNKNOWN EXCEPTION!\n";
        Logger::getInstance().critical("Application terminated due to unknown exception.");
        Logger::releaseInstance();
        return 1;
    }    
    
    LOG_INFO("============================================================");
    LOG_INFO("University Management System - Application Shutting Down...");
    LOG_INFO("============================================================");
    
    // DaoFactory::cleanup(); // Không cần nếu dùng shared_ptr đúng cách
    // Logger::releaseInstance(); 
    return 0;
}