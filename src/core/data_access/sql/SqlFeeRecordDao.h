#ifndef SQLFEERECORDDAO_H
#define SQLFEERECORDDAO_H

/**
 * @file SqlFeeRecordDao.h
 * @brief SQL implementation of Fee Record data access object
 */

#include "../interface/IFeeRecordDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h"

/**
 * @class SqlFeeRecordDao
 * @brief SQL implementation of the fee record data access object
 * 
 * This class provides SQL-based implementations for all fee record data access operations
 * defined in the IFeeRecordDao interface. It uses a database adapter to execute SQL queries
 * and an entity parser to convert database results into FeeRecord objects.
 */
class SqlFeeRecordDao : public IFeeRecordDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter; ///< Database adapter for executing SQL queries
    std::shared_ptr<IEntityParser<FeeRecord, DbQueryResultRow>> _parser; ///< Parser for converting DB results to FeeRecord objects

public:
    /**
     * @brief Constructor for SqlFeeRecordDao
     * @param dbAdapter Database adapter for executing SQL queries
     * @param parser Entity parser for converting database results to FeeRecord objects
     */
    SqlFeeRecordDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                    std::shared_ptr<IEntityParser<FeeRecord, DbQueryResultRow>> parser);
    
    /**
     * @brief Virtual destructor
     */
    ~SqlFeeRecordDao() override = default;

    /**
     * @brief Retrieves a fee record by the student ID
     * @param studentId The ID of the student whose fee record to retrieve
     * @return The fee record if found, or an error if not found or on database error
     */
    std::expected<FeeRecord, Error> getById(const std::string& studentId) const override;
    
    /**
     * @brief Retrieves all fee records from the database
     * @return A vector of all fee records or an error on database failure
     */
    std::expected<std::vector<FeeRecord>, Error> getAll() const override;
    
    /**
     * @brief Adds a new fee record to the database
     * @param feeRecord The fee record object to add
     * @return The added fee record with any database-generated fields or an error on failure
     */
    std::expected<FeeRecord, Error> add(const FeeRecord& feeRecord) override;
    
    /**
     * @brief Updates an existing fee record in the database
     * @param feeRecord The fee record object with updated values
     * @return True if update succeeded, or an error on failure
     */
    std::expected<bool, Error> update(const FeeRecord& feeRecord) override;
    
    /**
     * @brief Removes a fee record from the database
     * @param studentId The ID of the student whose fee record to remove
     * @return True if removal succeeded, or an error on failure
     */
    std::expected<bool, Error> remove(const std::string& studentId) override;
    
    /**
     * @brief Checks if a fee record with the given student ID exists
     * @param studentId The student ID to check
     * @return True if the fee record exists, false if not, or an error on database failure
     */
    std::expected<bool, Error> exists(const std::string& studentId) const override;
};

#endif // SQLFEERECORDDAO_H