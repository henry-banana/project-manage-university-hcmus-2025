#ifndef SQLLOGINDAO_H
#define SQLLOGINDAO_H

/**
 * @file SqlLoginDao.h
 * @brief SQL implementation of Login data access object
 */

#include "../interface/ILoginDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h" // Để inject LoginCredentialsSqlParser
#include <memory>

/**
 * @class SqlLoginDao
 * @brief SQL implementation of the login data access object
 * 
 * This class provides SQL-based implementations for all login-related data access operations
 * defined in the ILoginDao interface. It uses a database adapter to execute SQL queries
 * and an entity parser to convert database results into LoginCredentials objects.
 */
class SqlLoginDao : public ILoginDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter; ///< Database adapter for executing SQL queries
    // LoginCredentials không phải là IEntity đầy đủ, nhưng parser vẫn hữu ích
    std::shared_ptr<IEntityParser<LoginCredentials, DbQueryResultRow>> _parser; ///< Parser for converting DB results to LoginCredentials objects

public:
    /**
     * @brief Constructor for SqlLoginDao
     * @param dbAdapter Database adapter for executing SQL queries
     * @param parser Entity parser for converting database results to LoginCredentials objects
     */
    SqlLoginDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                std::shared_ptr<IEntityParser<LoginCredentials, DbQueryResultRow>> parser);
    
    /**
     * @brief Virtual destructor
     */
    ~SqlLoginDao() override = default;

    /**
     * @brief Finds login credentials for a specific user ID
     * @param userId The ID of the user whose credentials to retrieve
     * @return The login credentials if found, or an error if not found or on database error
     */
    std::expected<LoginCredentials, Error> findCredentialsByUserId(const std::string& userId) const override;
    
    /**
     * @brief Adds new user credentials to the database
     * @param userId The ID of the user
     * @param passwordHash The hashed password
     * @param salt The salt used for password hashing
     * @param role The user's role
     * @param status The user's login status
     * @return True if adding succeeded, or an error on failure
     */
    std::expected<bool, Error> addUserCredentials(const std::string& userId, const std::string& passwordHash, const std::string& salt, UserRole role, LoginStatus status) override;
    
    /**
     * @brief Updates a user's password
     * @param userId The ID of the user
     * @param newPasswordHash The new hashed password
     * @param newSalt The new salt used for password hashing
     * @return True if update succeeded, or an error on failure
     */
    std::expected<bool, Error> updatePassword(const std::string& userId, const std::string& newPasswordHash, const std::string& newSalt) override;
    
    /**
     * @brief Removes a user's credentials from the database
     * @param userId The ID of the user whose credentials to remove
     * @return True if removal succeeded, or an error on failure
     */
    std::expected<bool, Error> removeUserCredentials(const std::string& userId) override;
    
    /**
     * @brief Gets a user's role
     * @param userId The ID of the user
     * @return The user's role if found, or an error if not found or on database error
     */
    std::expected<UserRole, Error> getUserRole(const std::string& userId) const override;
    
    /**
     * @brief Gets a user's login status
     * @param userId The ID of the user
     * @return The user's login status if found, or an error if not found or on database error
     */
    std::expected<LoginStatus, Error> getUserStatus(const std::string& userId) const override;
    
    /**
     * @brief Updates a user's role and status
     * @param userId The ID of the user
     * @param newRole The new role to assign
     * @param newStatus The new login status to assign
     * @return True if update succeeded, or an error on failure
     */
    std::expected<bool, Error> updateUserRoleAndStatus(const std::string& userId, UserRole newRole, LoginStatus newStatus) override;
    
    /**
     * @brief Finds all users with a specific login status
     * @param status The login status to search for
     * @return A vector of matching login credentials or an error on database failure
     */
    std::expected<std::vector<LoginCredentials>, Error> findByStatus(LoginStatus status) const override;
};

#endif // SQLLOGINDAO_H