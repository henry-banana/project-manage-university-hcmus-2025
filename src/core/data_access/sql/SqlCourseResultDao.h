#ifndef SQLCOURSERESULTDAO_H
#define SQLCOURSERESULTDAO_H

/**
 * @file SqlCourseResultDao.h
 * @brief SQL implementation of Course Result data access object
 */

#include "../interface/ICourseResultDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h"
#include <memory>

/**
 * @class SqlCourseResultDao
 * @brief SQL implementation of the course result data access object
 * 
 * This class provides SQL-based implementations for all course result data access operations
 * defined in the ICourseResultDao interface. It uses a database adapter to execute SQL queries
 * and an entity parser to convert database results into CourseResult objects.
 */
class SqlCourseResultDao : public ICourseResultDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter; ///< Database adapter for executing SQL queries
    std::shared_ptr<IEntityParser<CourseResult, DbQueryResultRow>> _parser; ///< Parser for converting DB results to CourseResult objects

public:
    /**
     * @brief Constructor for SqlCourseResultDao
     * @param dbAdapter Database adapter for executing SQL queries
     * @param parser Entity parser for converting database results to CourseResult objects
     */
    SqlCourseResultDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                       std::shared_ptr<IEntityParser<CourseResult, DbQueryResultRow>> parser);
    
    /**
     * @brief Virtual destructor
     */
    ~SqlCourseResultDao() override = default;

    /**
     * @brief Finds a course result for a specific student and course
     * @param studentId The ID of the student
     * @param courseId The ID of the course
     * @return The course result if found, or an error if not found or on database error
     */
    std::expected<CourseResult, Error> find(const std::string& studentId, const std::string& courseId) const override;
    
    /**
     * @brief Finds all course results for a specific student
     * @param studentId The ID of the student
     * @return A vector of course results or an error on database failure
     */
    std::expected<std::vector<CourseResult>, Error> findByStudentId(const std::string& studentId) const override;
    
    /**
     * @brief Finds all course results for a specific course
     * @param courseId The ID of the course
     * @return A vector of course results or an error on database failure
     */
    std::expected<std::vector<CourseResult>, Error> findByCourseId(const std::string& courseId) const override;
    
    /**
     * @brief Adds a new course result or updates an existing one
     * @param result The course result object to add or update
     * @return True if the operation succeeded, or an error on failure
     */
    std::expected<bool, Error> addOrUpdate(const CourseResult& result) override;
    
    /**
     * @brief Removes a specific course result
     * @param studentId The ID of the student
     * @param courseId The ID of the course
     * @return True if removal succeeded, or an error on failure
     */
    std::expected<bool, Error> remove(const std::string& studentId, const std::string& courseId) override;
    
    /**
     * @brief Removes all course results for a specific student
     * @param studentId The ID of the student
     * @return True if removal succeeded, or an error on failure
     */
    std::expected<bool, Error> removeAllForStudent(const std::string& studentId) override;
    
    /**
     * @brief Removes all course results for a specific course
     * @param courseId The ID of the course
     * @return True if removal succeeded, or an error on failure
     */
    std::expected<bool, Error> removeAllForCourse(const std::string& courseId) override;
};

#endif // SQLCOURSERESULTDAO_H