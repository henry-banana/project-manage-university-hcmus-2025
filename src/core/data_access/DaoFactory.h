#ifndef DAOFACTORY_H
#define DAOFACTORY_H

#include <memory>
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
// (➕) Interface cho file handler để inject vào CsvDaos
#include "../../utils/IFileHandler.h"
// (➕) Interfaces cho parsers để inject vào CsvDaos
#include "../parsing/interface/IEntityParser.h"
#include "../../core/entities/Student.h" // ví dụ
// ... và các entity khác
// (➕) Interface cho DB Adapter để inject vào SqlDaos
#include "../database_adapter/interface/IDatabaseAdapter.h"


class DaoFactory {
private:
    // (➕) Tạo file handler một lần, có thể là static hoặc member của factory
    static std::shared_ptr<IFileHandler> getFileHandlerInstance();

    // (➕) Tạo các parser một lần, hoặc mỗi lần tạo DAO tùy logic
    // Ví dụ:
    template<typename TEntity>
    static std::shared_ptr<IEntityParser<TEntity, std::vector<std::string>>> getCsvRowParserInstance();
    
    template<typename TEntity, typename TDbRow>
    static std::shared_ptr<IEntityParser<TEntity, TDbRow>> getSqlRowParserInstance();


    // (➕) Tạo DB Adapter một lần (nếu dùng SQL)
    static std::shared_ptr<IDatabaseAdapter> getDatabaseAdapterInstance(const AppConfig& config);


public:
    // Các factory method sẽ sử dụng getFileHandler, getDbAdapter, getCsvEntityParser, getSqlEntityParser
    static std::shared_ptr<IStudentDao> createStudentDao(const AppConfig& config);
    static std::shared_ptr<ITeacherDao> createTeacherDao(const AppConfig& config);
    static std::shared_ptr<IFacultyDao> createFacultyDao(const AppConfig& config);
    static std::shared_ptr<ICourseDao> createCourseDao(const AppConfig& config);
    static std::shared_ptr<IEnrollmentDao> createEnrollmentDao(const AppConfig& config);
    static std::shared_ptr<ICourseResultDao> createCourseResultDao(const AppConfig& config);
    static std::shared_ptr<IFeeRecordDao> createFeeRecordDao(const AppConfig& config);
    static std::shared_ptr<ISalaryRecordDao> createSalaryRecordDao(const AppConfig& config);
    static std::shared_ptr<ILoginDao> createLoginDao(const AppConfig& config);
};

#endif // DAOFACTORY_H