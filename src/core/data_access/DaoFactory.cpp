#include "DaoFactory.h"
// Include các SQL DAO cụ thể
#include "sql/SqlStudentDao.h"
#include "sql/SqlTeacherDao.h"
#include "sql/SqlFacultyDao.h"
#include "sql/SqlCourseDao.h"
#include "sql/SqlLoginDao.h"
#include "sql/SqlEnrollmentDao.h"
#include "sql/SqlCourseResultDao.h"
#include "sql/SqlFeeRecordDao.h"
#include "sql/SqlSalaryRecordDao.h"

// Khởi tạo các con trỏ static
std::shared_ptr<IDatabaseAdapter> DaoFactory::_dbAdapterInstance = nullptr;
std::mutex DaoFactory::_dbAdapterMutex;

std::shared_ptr<IEntityParser<Student, DbQueryResultRow>> DaoFactory::_studentSqlParserInstance = nullptr;
std::shared_ptr<IEntityParser<Teacher, DbQueryResultRow>> DaoFactory::_teacherSqlParserInstance = nullptr;
std::shared_ptr<IEntityParser<Faculty, DbQueryResultRow>> DaoFactory::_facultySqlParserInstance = nullptr;
std::shared_ptr<IEntityParser<Course, DbQueryResultRow>> DaoFactory::_courseSqlParserInstance = nullptr;
std::shared_ptr<IEntityParser<LoginCredentials, DbQueryResultRow>> DaoFactory::_loginCredentialsSqlParserInstance = nullptr;
std::shared_ptr<IEntityParser<EnrollmentRecord, DbQueryResultRow>> DaoFactory::_enrollmentRecordSqlParserInstance = nullptr;
std::shared_ptr<IEntityParser<CourseResult, DbQueryResultRow>> DaoFactory::_courseResultSqlParserInstance = nullptr;
std::shared_ptr<IEntityParser<FeeRecord, DbQueryResultRow>> DaoFactory::_feeRecordSqlParserInstance = nullptr;
std::shared_ptr<IEntityParser<SalaryRecord, DbQueryResultRow>> DaoFactory::_salaryRecordSqlParserInstance = nullptr;
std::mutex DaoFactory::_parserMutex;


std::shared_ptr<IDatabaseAdapter> DaoFactory::getDatabaseAdapter(const AppConfig& config) {
    std::lock_guard<std::mutex> lock(_dbAdapterMutex);
    if (_dbAdapterInstance == nullptr) {
        if (config.dataSourceType == DataSourceType::SQL) {
            // (➕) Tự động tạo thư mục cho database nếu chưa có
            try {
                std::filesystem::path dbPath(config.sqlConnectionString);
                if (dbPath.has_parent_path()) {
                    if (!std::filesystem::exists(dbPath.parent_path())) {
                        if (std::filesystem::create_directories(dbPath.parent_path())) {
                            LOG_INFO("DaoFactory: Created database directory: " + dbPath.parent_path().string());
                        } else {
                            LOG_ERROR("DaoFactory: Failed to create database directory: " + dbPath.parent_path().string());
                            // Có thể throw hoặc tiếp tục và để connect thất bại
                        }
                    }
                }
            } catch (const std::filesystem::filesystem_error& e) {
                LOG_ERROR("DaoFactory: Filesystem error while ensuring database directory: " + std::string(e.what()));
                // Có thể throw hoặc tiếp tục
            }

            auto sqliteAdapter = std::make_shared<SQLiteAdapter>();
            // sqlConnectionString trong AppConfig là đường dẫn file DB
            auto connectResult = sqliteAdapter->connect(config.sqlConnectionString); 
            if (!connectResult.has_value() || !connectResult.value()) {
                std::string errMsg = connectResult.has_value() ? "Connection returned false" : connectResult.error().message;
                LOG_CRITICAL("DaoFactory: Failed to connect to SQLite database (" + config.sqlConnectionString + "): " + errMsg);
                throw std::runtime_error("DaoFactory: Failed to initialize database adapter. Reason: " + errMsg);
            }
            
            auto tableEnsureResult = sqliteAdapter->ensureTablesExist();
            if(!tableEnsureResult.has_value() || !tableEnsureResult.value()){
                 std::string errMsg = tableEnsureResult.has_value() ? "ensureTablesExist returned false" : tableEnsureResult.error().message;
                LOG_CRITICAL("DaoFactory: Failed to ensure SQLite tables exist: " + errMsg);
                throw std::runtime_error("DaoFactory: Failed to ensure database tables. Reason: " + errMsg);
            }
            _dbAdapterInstance = sqliteAdapter;
        } else {
            throw std::runtime_error("DaoFactory: SQL data source type selected, but no SQL configuration available or invalid type.");
        }
    }
    return _dbAdapterInstance;
}

// Triển khai các getXxxSqlParser()
std::shared_ptr<IEntityParser<Student, DbQueryResultRow>> DaoFactory::getStudentSqlParser() {
    std::lock_guard<std::mutex> lock(_parserMutex);
    if (!_studentSqlParserInstance) {
        _studentSqlParserInstance = std::make_shared<StudentSqlParser>();
    }
    return _studentSqlParserInstance;
}

std::shared_ptr<IEntityParser<Teacher, DbQueryResultRow>> DaoFactory::getTeacherSqlParser() {
    std::lock_guard<std::mutex> lock(_parserMutex);
    if (!_teacherSqlParserInstance) {
        _teacherSqlParserInstance = std::make_shared<TeacherSqlParser>();
    }
    return _teacherSqlParserInstance;
}

std::shared_ptr<IEntityParser<Faculty, DbQueryResultRow>> DaoFactory::getFacultySqlParser() {
    std::lock_guard<std::mutex> lock(_parserMutex);
    if (!_facultySqlParserInstance) {
        _facultySqlParserInstance = std::make_shared<FacultySqlParser>();
    }
    return _facultySqlParserInstance;
}

std::shared_ptr<IEntityParser<Course, DbQueryResultRow>> DaoFactory::getCourseSqlParser() {
    std::lock_guard<std::mutex> lock(_parserMutex);
    if (!_courseSqlParserInstance) {
        _courseSqlParserInstance = std::make_shared<CourseSqlParser>();
    }
    return _courseSqlParserInstance;
}

std::shared_ptr<IEntityParser<LoginCredentials, DbQueryResultRow>> DaoFactory::getLoginCredentialsSqlParser() {
    std::lock_guard<std::mutex> lock(_parserMutex);
    if (!_loginCredentialsSqlParserInstance) {
        _loginCredentialsSqlParserInstance = std::make_shared<LoginCredentialsSqlParser>();
    }
    return _loginCredentialsSqlParserInstance;
}

std::shared_ptr<IEntityParser<EnrollmentRecord, DbQueryResultRow>> DaoFactory::getEnrollmentRecordSqlParser() {
    std::lock_guard<std::mutex> lock(_parserMutex);
    if (!_enrollmentRecordSqlParserInstance) {
        _enrollmentRecordSqlParserInstance = std::make_shared<EnrollmentRecordSqlParser>();
    }
    return _enrollmentRecordSqlParserInstance;
}

std::shared_ptr<IEntityParser<CourseResult, DbQueryResultRow>> DaoFactory::getCourseResultSqlParser() {
    std::lock_guard<std::mutex> lock(_parserMutex);
    if (!_courseResultSqlParserInstance) {
        _courseResultSqlParserInstance = std::make_shared<CourseResultSqlParser>();
    }
    return _courseResultSqlParserInstance;
}

std::shared_ptr<IEntityParser<FeeRecord, DbQueryResultRow>> DaoFactory::getFeeRecordSqlParser() {
    std::lock_guard<std::mutex> lock(_parserMutex);
    if (!_feeRecordSqlParserInstance) {
        _feeRecordSqlParserInstance = std::make_shared<FeeRecordSqlParser>();
    }
    return _feeRecordSqlParserInstance;
}

std::shared_ptr<IEntityParser<SalaryRecord, DbQueryResultRow>> DaoFactory::getSalaryRecordSqlParser() {
    std::lock_guard<std::mutex> lock(_parserMutex);
    if (!_salaryRecordSqlParserInstance) {
        _salaryRecordSqlParserInstance = std::make_shared<SalaryRecordSqlParser>();
    }
    return _salaryRecordSqlParserInstance;
}


// Triển khai các create DAO methods
std::shared_ptr<IStudentDao> DaoFactory::createStudentDao(const AppConfig& config) {
    switch (config.dataSourceType) {
        case DataSourceType::SQL:
            return std::make_shared<SqlStudentDao>(getDatabaseAdapter(config), getStudentSqlParser());
        case DataSourceType::MOCK:
             return std::make_shared<MockStudentDao>();
        // case DataSourceType::CSV:
        //     // return std::make_shared<CsvStudentDao>(...); // Cần file handler và CSV parser
        default:
            LOG_ERROR("DaoFactory: Unsupported data source type for StudentDao: " + std::to_string(static_cast<int>(config.dataSourceType)));
            throw std::runtime_error("Unsupported data source type for StudentDao");
    }
}

std::shared_ptr<ITeacherDao> DaoFactory::createTeacherDao(const AppConfig& config) {
    switch (config.dataSourceType) {
        case DataSourceType::SQL:
            return std::make_shared<SqlTeacherDao>(getDatabaseAdapter(config), getTeacherSqlParser());
        case DataSourceType::MOCK:
            return std::make_shared<MockTeacherDao>();
        default:
            LOG_ERROR("DaoFactory: Unsupported data source type for TeacherDao: " + std::to_string(static_cast<int>(config.dataSourceType)));
            throw std::runtime_error("Unsupported data source type for TeacherDao");
    }
}

std::shared_ptr<IFacultyDao> DaoFactory::createFacultyDao(const AppConfig& config) {
    switch (config.dataSourceType) {
        case DataSourceType::SQL:
            return std::make_shared<SqlFacultyDao>(getDatabaseAdapter(config), getFacultySqlParser());
        case DataSourceType::MOCK:
            return std::make_shared<MockFacultyDao>();
        default:
            LOG_ERROR("DaoFactory: Unsupported data source type for FacultyDao: " + std::to_string(static_cast<int>(config.dataSourceType)));
            throw std::runtime_error("Unsupported data source type for FacultyDao");
    }
}

std::shared_ptr<ICourseDao> DaoFactory::createCourseDao(const AppConfig& config) {
    switch (config.dataSourceType) {
        case DataSourceType::SQL:
            return std::make_shared<SqlCourseDao>(getDatabaseAdapter(config), getCourseSqlParser());
        case DataSourceType::MOCK:
            return std::make_shared<MockCourseDao>();
        default:
            LOG_ERROR("DaoFactory: Unsupported data source type for CourseDao: " + std::to_string(static_cast<int>(config.dataSourceType)));
            throw std::runtime_error("Unsupported data source type for CourseDao");
    }
}

std::shared_ptr<ILoginDao> DaoFactory::createLoginDao(const AppConfig& config) {
    switch (config.dataSourceType) {
        case DataSourceType::SQL:
            return std::make_shared<SqlLoginDao>(getDatabaseAdapter(config), getLoginCredentialsSqlParser());
        case DataSourceType::MOCK:
            return std::make_shared<MockLoginDao>();
        default:
            LOG_ERROR("DaoFactory: Unsupported data source type for LoginDao: " + std::to_string(static_cast<int>(config.dataSourceType)));
            throw std::runtime_error("Unsupported data source type for LoginDao");
    }
}

std::shared_ptr<IEnrollmentDao> DaoFactory::createEnrollmentDao(const AppConfig& config) {
    switch (config.dataSourceType) {
        case DataSourceType::SQL:
            return std::make_shared<SqlEnrollmentDao>(getDatabaseAdapter(config), getEnrollmentRecordSqlParser());
        case DataSourceType::MOCK:
            return std::make_shared<MockEnrollmentDao>();
        default:
            LOG_ERROR("DaoFactory: Unsupported data source type for EnrollmentDao: " + std::to_string(static_cast<int>(config.dataSourceType)));
            throw std::runtime_error("Unsupported data source type for EnrollmentDao");
    }
}

std::shared_ptr<ICourseResultDao> DaoFactory::createCourseResultDao(const AppConfig& config) {
    switch (config.dataSourceType) {
        case DataSourceType::SQL:
            return std::make_shared<SqlCourseResultDao>(getDatabaseAdapter(config), getCourseResultSqlParser());
        case DataSourceType::MOCK:
            return std::make_shared<MockCourseResultDao>();
        default:
            LOG_ERROR("DaoFactory: Unsupported data source type for CourseResultDao: " + std::to_string(static_cast<int>(config.dataSourceType)));
            throw std::runtime_error("Unsupported data source type for CourseResultDao");
    }
}

std::shared_ptr<IFeeRecordDao> DaoFactory::createFeeRecordDao(const AppConfig& config) {
    switch (config.dataSourceType) {
        case DataSourceType::SQL:
            return std::make_shared<SqlFeeRecordDao>(getDatabaseAdapter(config), getFeeRecordSqlParser());
        case DataSourceType::MOCK:
            return std::make_shared<MockFeeRecordDao>();
        default:
            LOG_ERROR("DaoFactory: Unsupported data source type for FeeRecordDao: " + std::to_string(static_cast<int>(config.dataSourceType)));
            throw std::runtime_error("Unsupported data source type for FeeRecordDao");
    }
}

std::shared_ptr<ISalaryRecordDao> DaoFactory::createSalaryRecordDao(const AppConfig& config) {
    switch (config.dataSourceType) {
        case DataSourceType::SQL:
            return std::make_shared<SqlSalaryRecordDao>(getDatabaseAdapter(config), getSalaryRecordSqlParser());
        case DataSourceType::MOCK:
            return std::make_shared<MockSalaryRecordDao>();
        default:
            LOG_ERROR("DaoFactory: Unsupported data source type for SalaryRecordDao: " + std::to_string(static_cast<int>(config.dataSourceType)));
            throw std::runtime_error("Unsupported data source type for SalaryRecordDao");
    }
}

void DaoFactory::cleanup() {
    std::lock_guard<std::mutex> lock(_dbAdapterMutex);
    if (_dbAdapterInstance) {
        LOG_INFO("DaoFactory: Cleaning up database adapter.");
        // Destructor của IDatabaseAdapter sẽ được gọi khi _dbAdapterInstance.reset()
        // hoặc khi shared_ptr cuối cùng trỏ đến nó bị hủy.
        // Nếu IDatabaseAdapter::disconnect() có ghi log, nó vẫn hoạt động ở đây.
        _dbAdapterInstance.reset(); 
    }
    std::lock_guard<std::mutex> lock_parser(_parserMutex);
    _studentSqlParserInstance.reset();
    _teacherSqlParserInstance.reset();
    // ... reset all other parser instances
    _facultySqlParserInstance.reset();
    _courseSqlParserInstance.reset();
    _loginCredentialsSqlParserInstance.reset();
    _enrollmentRecordSqlParserInstance.reset();
    _courseResultSqlParserInstance.reset();
    _feeRecordSqlParserInstance.reset();
    _salaryRecordSqlParserInstance.reset();
    LOG_INFO("DaoFactory: Static resources cleaned up.");
}