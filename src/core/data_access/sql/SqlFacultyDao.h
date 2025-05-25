#ifndef SQLFACULTYDAO_H
#define SQLFACULTYDAO_H

/**
 * @file SqlFacultyDao.h
 * @brief SQL implementation of Faculty data access object
 */

#include "../interface/IFacultyDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h" // Để inject parser

/**
 * @class SqlFacultyDao
 * @brief SQL implementation of the faculty data access object
 * 
 * This class provides SQL-based implementations for all faculty data access operations
 * defined in the IFacultyDao interface. It uses a database adapter to execute SQL queries
 * and an entity parser to convert database results into Faculty objects.
 */
class SqlFacultyDao : public IFacultyDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter; ///< Database adapter for executing SQL queries
    std::shared_ptr<IEntityParser<Faculty, DbQueryResultRow>> _parser; ///< Parser for converting DB results to Faculty objects

public:
    /**
     * @brief Constructor for SqlFacultyDao
     * @param dbAdapter Database adapter for executing SQL queries
     * @param parser Entity parser for converting database results to Faculty objects
     */
    SqlFacultyDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                  std::shared_ptr<IEntityParser<Faculty, DbQueryResultRow>> parser);

    /**
     * @brief Virtual destructor
     */
    ~SqlFacultyDao() override = default;

    /**
     * @brief Retrieves a faculty by its ID
     * @param id The ID of the faculty to retrieve
     * @return The faculty if found, or an error if not found or on database error
     */
    std::expected<Faculty, Error> getById(const std::string& id) const override;
    
    /**
     * @brief Retrieves all faculties from the database
     * @return A vector of all faculties or an error on database failure
     */
    std::expected<std::vector<Faculty>, Error> getAll() const override;
    
    /**
     * @brief Adds a new faculty to the database
     * @param faculty The faculty object to add
     * @return The added faculty with any database-generated fields or an error on failure
     */
    std::expected<Faculty, Error> add(const Faculty& faculty) override;
    
    /**
     * @brief Updates an existing faculty in the database
     * @param faculty The faculty object with updated values
     * @return True if update succeeded, or an error on failure
     */
    std::expected<bool, Error> update(const Faculty& faculty) override;
    
    /**
     * @brief Removes a faculty from the database
     * @param id The ID of the faculty to remove
     * @return True if removal succeeded, or an error on failure
     */
    std::expected<bool, Error> remove(const std::string& id) override;
    
    /**
     * @brief Checks if a faculty with the given ID exists
     * @param id The ID to check
     * @return True if the faculty exists, false if not, or an error on database failure
     */
    std::expected<bool, Error> exists(const std::string& id) const override;
    
    /**
     * @brief Finds a faculty by its name
     * @param name The name of the faculty to find
     * @return The faculty if found, or an error if not found or on database error
     */
    std::expected<Faculty, Error> findByName(const std::string& name) const override;
};

#endif // SQLFACULTYDAO_H