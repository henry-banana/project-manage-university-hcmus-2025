#ifndef SQLENROLLMENTDAO_H
#define SQLENROLLMENTDAO_H

/**
 * @file SqlEnrollmentDao.h
 * @brief SQL implementation of Enrollment data access object
 */

#include "../interface/IEnrollmentDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h" // Để inject EnrollmentRecordSqlParser
#include <memory>

/**
 * @class SqlEnrollmentDao
 * @brief SQL implementation of the enrollment data access object
 * 
 * This class provides SQL-based implementations for all enrollment-related data access operations
 * defined in the IEnrollmentDao interface. It uses a database adapter to execute SQL queries
 * and an entity parser to convert database results into EnrollmentRecord objects.
 */
class SqlEnrollmentDao : public IEnrollmentDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter; ///< Database adapter for executing SQL queries
    std::shared_ptr<IEntityParser<EnrollmentRecord, DbQueryResultRow>> _parser; ///< Parser for converting DB results to EnrollmentRecord objects

public:
    /**
     * @brief Constructor for SqlEnrollmentDao
     * @param dbAdapter Database adapter for executing SQL queries
     * @param parser Entity parser for converting database results to EnrollmentRecord objects
     */
    SqlEnrollmentDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                     std::shared_ptr<IEntityParser<EnrollmentRecord, DbQueryResultRow>> parser);
    
    /**
     * @brief Virtual destructor
     */
    ~SqlEnrollmentDao() override = default;

    /**
     * @brief Adds a new enrollment record
     * @param studentId The ID of the student
     * @param courseId The ID of the course
     * @return True if enrollment succeeded, or an error on failure
     */
    std::expected<bool, Error> addEnrollment(const std::string& studentId, const std::string& courseId) override;
    
    /**
     * @brief Removes a specific enrollment
     * @param studentId The ID of the student
     * @param courseId The ID of the course
     * @return True if removal succeeded, or an error on failure
     */
    std::expected<bool, Error> removeEnrollment(const std::string& studentId, const std::string& courseId) override;
    
    /**
     * @brief Removes all enrollments for a specific student
     * @param studentId The ID of the student
     * @return True if removal succeeded, or an error on failure
     */
    std::expected<bool, Error> removeEnrollmentsByStudent(const std::string& studentId) override;
    
    /**
     * @brief Removes all enrollments for a specific course
     * @param courseId The ID of the course
     * @return True if removal succeeded, or an error on failure
     */
    std::expected<bool, Error> removeEnrollmentsByCourse(const std::string& courseId) override;
    
    /**
     * @brief Checks if a student is enrolled in a specific course
     * @param studentId The ID of the student
     * @param courseId The ID of the course
     * @return True if enrolled, false if not, or an error on database failure
     */
    std::expected<bool, Error> isEnrolled(const std::string& studentId, const std::string& courseId) const override;
    
    /**
     * @brief Finds all course IDs for a specific student
     * @param studentId The ID of the student
     * @return A vector of course IDs or an error on database failure
     */
    std::expected<std::vector<std::string>, Error> findCourseIdsByStudentId(const std::string& studentId) const override;
    
    /**
     * @brief Finds all student IDs for a specific course
     * @param courseId The ID of the course
     * @return A vector of student IDs or an error on database failure
     */
    std::expected<std::vector<std::string>, Error> findStudentIdsByCourseId(const std::string& courseId) const override;
    
    /**
     * @brief Retrieves all enrollment records
     * @return A vector of all enrollment records or an error on database failure
     */
    std::expected<std::vector<EnrollmentRecord>, Error> getAllEnrollments() const override;
};

#endif // SQLENROLLMENTDAO_H