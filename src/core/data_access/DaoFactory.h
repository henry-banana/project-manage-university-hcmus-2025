#ifndef DAOFACTORY_H
#define DAOFACTORY_H

#include <memory>
#include "../../common/AppConfig.h"
#include "interface/IStudentDao.h"
#include "interface/ITeacherDao.h"
#include "interface/IFacultyDao.h"
#include "interface/ICourseDao.h"
#include "interface/IEnrollmentDao.h"
#include "interface/ICourseResultDao.h"
#include "interface/IFeeRecordDao.h"
#include "interface/ISalaryRecordDao.h"
#include "interface/ILoginDao.h"
#include "../../utils/IFileHandler.h"
#include "../parsing/interface/IEntityParser.h"
#include "../database_adapter/interface/IDatabaseAdapter.h"

// Forward declare entities nếu ParserFactory cần
class Student;
// ...

class DaoFactory {
private:
    // Các instance này có thể là static và được khởi tạo một lần, hoặc
    // DaoFactory có thể lưu trữ chúng như member nếu DaoFactory không phải là static class.
    // Đối với factory tĩnh, chúng ta thường tạo chúng bên trong các factory method.
    // std::shared_ptr<IFileHandler> _fileHandlerInstance;
    // std::shared_ptr<IDatabaseAdapter> _dbAdapterInstance;

    // Helper để lấy IFileHandler
    static std::shared_ptr<IFileHandler> getFileHandler(const AppConfig& config);
    // Helper để lấy IDatabaseAdapter
    static std::shared_ptr<IDatabaseAdapter> getDbAdapter(const AppConfig& config);

    // Helper để lấy IEntityParser cho CSV (TRawData là vector<string>)
    template<typename TEntity>
    static std::shared_ptr<IEntityParser<TEntity, std::vector<std::string>>> getCsvEntityParser();
    // Helper để lấy IEntityParser cho SQL (TRawData là DbQueryResultRow)
    template<typename TEntity>
    static std::shared_ptr<IEntityParser<TEntity, DbQueryResultRow>> getSqlEntityParser();


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