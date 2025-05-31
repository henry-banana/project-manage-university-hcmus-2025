// --- START OF MODIFIED FILE src/core/data_access/DaoFactory.h ---
#ifndef DAOFACTORY_H
#define DAOFACTORY_H

/**
 * @file DaoFactory.h
 * @brief Định nghĩa factory tạo các đối tượng DAO (Data Access Object)
 */

#include <memory>
#include <stdexcept> 
#include <mutex> // (➕) Thêm mutex include vì được sử dụng
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
#include "../database_adapter/sql/SQLiteAdapter.h" 

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


// Các Parser SQL cụ thể 
#include "../parsing/impl_sql_parser/StudentSqlParser.h"
#include "../parsing/impl_sql_parser/TeacherSqlParser.h"
#include "../parsing/impl_sql_parser/FacultySqlParser.h"
#include "../parsing/impl_sql_parser/CourseSqlParser.h"
#include "../parsing/impl_sql_parser/LoginCredentialsSqlParser.h"
#include "../parsing/impl_sql_parser/EnrollmentRecordSqlParser.h"
#include "../parsing/impl_sql_parser/CourseResultSqlParser.h"
#include "../parsing/impl_sql_parser/FeeRecordSqlParser.h"
#include "../parsing/impl_sql_parser/SalaryRecordSqlParser.h"

// Các Mock DAO 
#include "mock/MockStudentDao.h"
#include "mock/MockTeacherDao.h"
#include "mock/MockFacultyDao.h"
#include "mock/MockCourseDao.h"
#include "mock/MockLoginDao.h"
#include "mock/MockEnrollmentDao.h"
#include "mock/MockCourseResultDao.h"
#include "mock/MockFeeRecordDao.h"
#include "mock/MockSalaryRecordDao.h"


/**
 * @class DaoFactory
 * @brief Lớp factory tạo các đối tượng DAO (Data Access Object)
 * 
 * Lớp này cung cấp các phương thức tĩnh để tạo các đối tượng DAO
 * tương ứng với loại nguồn dữ liệu được cấu hình (SQL, Mock).
 */
class DaoFactory {
private:
    static std::shared_ptr<IDatabaseAdapter> _dbAdapterInstance; 
    static std::mutex _dbAdapterMutex; 
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


public: // (➕) CHUYỂN CÁC HÀM HELPER SANG PUBLIC ĐỂ TEST CÓ THỂ GỌI
    /**
     * @brief Lấy hoặc tạo mới database adapter
     * @param config Cấu hình ứng dụng
     * @return Con trỏ thông minh đến database adapter
     */
    static std::shared_ptr<IDatabaseAdapter> getDatabaseAdapter(const AppConfig& config);

    /**
     * @brief Lấy hoặc tạo mới parser cho Student
     * @return Con trỏ thông minh đến parser Student
     */
    static std::shared_ptr<IEntityParser<Student, DbQueryResultRow>> getStudentSqlParser();
    
    /**
     * @brief Lấy hoặc tạo mới parser cho Teacher
     * @return Con trỏ thông minh đến parser Teacher
     */
    static std::shared_ptr<IEntityParser<Teacher, DbQueryResultRow>> getTeacherSqlParser();
    
    /**
     * @brief Lấy hoặc tạo mới parser cho Faculty
     * @return Con trỏ thông minh đến parser Faculty
     */
    static std::shared_ptr<IEntityParser<Faculty, DbQueryResultRow>> getFacultySqlParser();
    
    /**
     * @brief Lấy hoặc tạo mới parser cho Course
     * @return Con trỏ thông minh đến parser Course
     */
    static std::shared_ptr<IEntityParser<Course, DbQueryResultRow>> getCourseSqlParser();
    
    /**
     * @brief Lấy hoặc tạo mới parser cho LoginCredentials
     * @return Con trỏ thông minh đến parser LoginCredentials
     */
    static std::shared_ptr<IEntityParser<LoginCredentials, DbQueryResultRow>> getLoginCredentialsSqlParser();
    
    /**
     * @brief Lấy hoặc tạo mới parser cho EnrollmentRecord
     * @return Con trỏ thông minh đến parser EnrollmentRecord
     */
    static std::shared_ptr<IEntityParser<EnrollmentRecord, DbQueryResultRow>> getEnrollmentRecordSqlParser();
    
    /**
     * @brief Lấy hoặc tạo mới parser cho CourseResult
     * @return Con trỏ thông minh đến parser CourseResult
     */
    static std::shared_ptr<IEntityParser<CourseResult, DbQueryResultRow>> getCourseResultSqlParser();
    
    /**
     * @brief Lấy hoặc tạo mới parser cho FeeRecord
     * @return Con trỏ thông minh đến parser FeeRecord
     */
    static std::shared_ptr<IEntityParser<FeeRecord, DbQueryResultRow>> getFeeRecordSqlParser();
    
    /**
     * @brief Lấy hoặc tạo mới parser cho SalaryRecord
     * @return Con trỏ thông minh đến parser SalaryRecord
     */
    static std::shared_ptr<IEntityParser<SalaryRecord, DbQueryResultRow>> getSalaryRecordSqlParser();


//public: // Phần public API chính vẫn giữ nguyên
    /**
     * @brief Tạo đối tượng DAO cho sinh viên
     * @param config Cấu hình ứng dụng
     * @return Con trỏ đến đối tượng DAO của sinh viên
     */
    static std::shared_ptr<IStudentDao> createStudentDao(const AppConfig& config);
    
    /**
     * @brief Tạo đối tượng DAO cho giảng viên
     * @param config Cấu hình ứng dụng
     * @return Con trỏ đến đối tượng DAO của giảng viên
     */
    static std::shared_ptr<ITeacherDao> createTeacherDao(const AppConfig& config);
    
    /**
     * @brief Tạo đối tượng DAO cho khoa
     * @param config Cấu hình ứng dụng
     * @return Con trỏ đến đối tượng DAO của khoa
     */
    static std::shared_ptr<IFacultyDao> createFacultyDao(const AppConfig& config);
    
    /**
     * @brief Tạo đối tượng DAO cho khóa học
     * @param config Cấu hình ứng dụng
     * @return Con trỏ đến đối tượng DAO của khóa học
     */
    static std::shared_ptr<ICourseDao> createCourseDao(const AppConfig& config);
    
    /**
     * @brief Tạo đối tượng DAO cho đăng ký khóa học
     * @param config Cấu hình ứng dụng
     * @return Con trỏ đến đối tượng DAO của đăng ký khóa học
     */
    static std::shared_ptr<IEnrollmentDao> createEnrollmentDao(const AppConfig& config);
    
    /**
     * @brief Tạo đối tượng DAO cho kết quả khóa học
     * @param config Cấu hình ứng dụng
     * @return Con trỏ đến đối tượng DAO của kết quả khóa học
     */
    static std::shared_ptr<ICourseResultDao> createCourseResultDao(const AppConfig& config);
    
    /**
     * @brief Tạo đối tượng DAO cho hồ sơ học phí
     * @param config Cấu hình ứng dụng
     * @return Con trỏ đến đối tượng DAO của hồ sơ học phí
     */
    static std::shared_ptr<IFeeRecordDao> createFeeRecordDao(const AppConfig& config);
    
    /**
     * @brief Tạo đối tượng DAO cho hồ sơ lương
     * @param config Cấu hình ứng dụng
     * @return Con trỏ đến đối tượng DAO của hồ sơ lương
     */
    static std::shared_ptr<ISalaryRecordDao> createSalaryRecordDao(const AppConfig& config);
    
    /**
     * @brief Tạo đối tượng DAO cho đăng nhập
     * @param config Cấu hình ứng dụng
     * @return Con trỏ đến đối tượng DAO của đăng nhập
     */
    static std::shared_ptr<ILoginDao> createLoginDao(const AppConfig& config);

    /**
     * @brief Giải phóng tài nguyên tĩnh
     * 
     * Phương thức này được gọi khi ứng dụng kết thúc để giải phóng
     * các tài nguyên tĩnh được sử dụng bởi DaoFactory.
     */
    static void cleanup();
};

#endif // DAOFACTORY_H
// --- END OF MODIFIED FILE src/core/data_access/DaoFactory.h ---