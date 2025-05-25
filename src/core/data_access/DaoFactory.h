#ifndef DAOFACTORY_H
#define DAOFACTORY_H

#include <memory>
#include <stdexcept> // For std::runtime_error
#include "../../common/AppConfig.h"
// Bao gồm tất cả các interface DAO
#include "interface/IStudentDao.h"
#include "interface/ITeacherDao.h"
#include "interface/IFacultyDao.h"
#include "interface/ICourseDao.h"
#include "interface/IEnrollmentDao.h"
#include "interface/ICourseResultDao.h"
#include "interface/IFeeRecordDao.h"
#include "interface/ISalaryRecordDao.h"
#include "interface/ILoginDao.h"

// Interface cho DB Adapter
#include "../database_adapter/interface/IDatabaseAdapter.h"
// Adapter cụ thể (SQLite)
#include "../database_adapter/sql/SQLiteAdapter.h" // (➕) Cần để khởi tạo

// Interface cho Parsers
#include "../parsing/interface/IEntityParser.h"

// Các Entity (cần cho template của parser)
#include "../../core/entities/Student.h"
#include "../../core/entities/Teacher.h"
#include "../../core/entities/Faculty.h"
#include "../../core/entities/Course.h"
#include "../../core/entities/CourseResult.h"
#include "../../core/entities/FeeRecord.h"
#include "../../core/entities/SalaryRecord.h"
// EnrollmentRecord và LoginCredentials đã được include trong IEnrollmentDao.h và ILoginDao.h

// Các Parser SQL cụ thể (➕)
#include "../parsing/impl_sql_parser/StudentSqlParser.h"
#include "../parsing/impl_sql_parser/TeacherSqlParser.h"
#include "../parsing/impl_sql_parser/FacultySqlParser.h"
#include "../parsing/impl_sql_parser/CourseSqlParser.h"
#include "../parsing/impl_sql_parser/LoginCredentialsSqlParser.h"
#include "../parsing/impl_sql_parser/EnrollmentRecordSqlParser.h"
#include "../parsing/impl_sql_parser/CourseResultSqlParser.h"
#include "../parsing/impl_sql_parser/FeeRecordSqlParser.h"
#include "../parsing/impl_sql_parser/SalaryRecordSqlParser.h"

// Các Mock DAO (➕)
#include "mock/MockStudentDao.h"
#include "mock/MockTeacherDao.h"
#include "mock/MockFacultyDao.h"
#include "mock/MockCourseDao.h"
#include "mock/MockLoginDao.h"
#include "mock/MockEnrollmentDao.h"
#include "mock/MockCourseResultDao.h"
#include "mock/MockFeeRecordDao.h"
#include "mock/MockSalaryRecordDao.h"

// File Handler không cần thiết cho DaoFactory tạo SQL DAO hoặc Mock DAO nữa.

class DaoFactory {
private:
    // Giữ một instance của IDatabaseAdapter nếu dùng SQL.
    // Được tạo một lần khi DaoFactory được yêu cầu tạo SQL DAO lần đầu.
    static std::shared_ptr<IDatabaseAdapter> _dbAdapterInstance;
    static std::mutex _dbAdapterMutex;

    // Tương tự, giữ các instance của parser để tái sử dụng.
    static std::shared_ptr<IEntityParser<Student, DbQueryResultRow>> _studentSqlParserInstance;
    static std::shared_ptr<IEntityParser<Teacher, DbQueryResultRow>> _teacherSqlParserInstance;
    static std::shared_ptr<IEntityParser<Faculty, DbQueryResultRow>> _facultySqlParserInstance;
    static std::shared_ptr<IEntityParser<Course, DbQueryResultRow>> _courseSqlParserInstance;
    static std::shared_ptr<IEntityParser<LoginCredentials, DbQueryResultRow>> _loginCredentialsSqlParserInstance;
    static std::shared_ptr<IEntityParser<EnrollmentRecord, DbQueryResultRow>> _enrollmentRecordSqlParserInstance;
    static std::shared_ptr<IEntityParser<CourseResult, DbQueryResultRow>> _courseResultSqlParserInstance;
    static std::shared_ptr<IEntityParser<FeeRecord, DbQueryResultRow>> _feeRecordSqlParserInstance;
    static std::shared_ptr<IEntityParser<SalaryRecord, DbQueryResultRow>> _salaryRecordSqlParserInstance;
    static std::mutex _parserMutex;


    // Helper để lấy (hoặc tạo nếu chưa có) IDatabaseAdapter
    static std::shared_ptr<IDatabaseAdapter> getDatabaseAdapter(const AppConfig& config);

    // Helper để lấy (hoặc tạo nếu chưa có) các SQL Parser
    template<typename TEntity>
    static std::shared_ptr<IEntityParser<TEntity, DbQueryResultRow>> getSqlEntityParser();
    // Chuyên biệt hóa template cho từng entity
    static std::shared_ptr<IEntityParser<Student, DbQueryResultRow>> getStudentSqlParser();
    static std::shared_ptr<IEntityParser<Teacher, DbQueryResultRow>> getTeacherSqlParser();
    static std::shared_ptr<IEntityParser<Faculty, DbQueryResultRow>> getFacultySqlParser();
    static std::shared_ptr<IEntityParser<Course, DbQueryResultRow>> getCourseSqlParser();
    static std::shared_ptr<IEntityParser<LoginCredentials, DbQueryResultRow>> getLoginCredentialsSqlParser();
    static std::shared_ptr<IEntityParser<EnrollmentRecord, DbQueryResultRow>> getEnrollmentRecordSqlParser();
    static std::shared_ptr<IEntityParser<CourseResult, DbQueryResultRow>> getCourseResultSqlParser();
    static std::shared_ptr<IEntityParser<FeeRecord, DbQueryResultRow>> getFeeRecordSqlParser();
    static std::shared_ptr<IEntityParser<SalaryRecord, DbQueryResultRow>> getSalaryRecordSqlParser();


public:
    // Các factory method
    static std::shared_ptr<IStudentDao> createStudentDao(const AppConfig& config);
    static std::shared_ptr<ITeacherDao> createTeacherDao(const AppConfig& config);
    static std::shared_ptr<IFacultyDao> createFacultyDao(const AppConfig& config);
    static std::shared_ptr<ICourseDao> createCourseDao(const AppConfig& config);
    static std::shared_ptr<IEnrollmentDao> createEnrollmentDao(const AppConfig& config);
    static std::shared_ptr<ICourseResultDao> createCourseResultDao(const AppConfig& config);
    static std::shared_ptr<IFeeRecordDao> createFeeRecordDao(const AppConfig& config);
    static std::shared_ptr<ISalaryRecordDao> createSalaryRecordDao(const AppConfig& config);
    static std::shared_ptr<ILoginDao> createLoginDao(const AppConfig& config);

    // (Optional) Clean up static resources if needed, e.g., upon application shutdown
    static void cleanup();
};

#endif // DAOFACTORY_H