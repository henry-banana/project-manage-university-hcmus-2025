#ifndef SQLLOGINDAO_H
#define SQLLOGINDAO_H

#include "../interface/ILoginDao.h"
#include "../../database_adapter/interface/IDatabaseAdapter.h"
#include "../../parsing/interface/IEntityParser.h" // Để inject LoginCredentialsSqlParser
#include <memory>

class SqlLoginDao : public ILoginDao {
private:
    std::shared_ptr<IDatabaseAdapter> _dbAdapter;
    // LoginCredentials không phải là IEntity đầy đủ, nhưng parser vẫn hữu ích
    std::shared_ptr<IEntityParser<LoginCredentials, DbQueryResultRow>> _parser;

public:
    SqlLoginDao(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                std::shared_ptr<IEntityParser<LoginCredentials, DbQueryResultRow>> parser);
    ~SqlLoginDao() override = default;

    std::expected<LoginCredentials, Error> findCredentialsByUserId(const std::string& userId) const override;
    std::expected<bool, Error> addUserCredentials(const std::string& userId, const std::string& passwordHash, const std::string& salt, UserRole role, LoginStatus status) override;
    std::expected<bool, Error> updatePassword(const std::string& userId, const std::string& newPasswordHash, const std::string& newSalt) override;
    std::expected<bool, Error> removeUserCredentials(const std::string& userId) override;
    std::expected<UserRole, Error> getUserRole(const std::string& userId) const override;
    std::expected<LoginStatus, Error> getUserStatus(const std::string& userId) const override;
    std::expected<bool, Error> updateUserRoleAndStatus(const std::string& userId, UserRole newRole, LoginStatus newStatus) override;
    std::expected<std::vector<LoginCredentials>, Error> findByStatus(LoginStatus status) const override;
};

#endif // SQLLOGINDAO_H