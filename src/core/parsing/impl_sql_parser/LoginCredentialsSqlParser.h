#ifndef LOGINCREDENTIALSSQLPARSER_H
#define LOGINCREDENTIALSSQLPARSER_H

#include "../interface/IEntityParser.h" // Mặc dù LoginCredentials không phải là IEntity, nhưng parser pattern có thể tái sử dụng
#include "../../data_access/interface/ILoginDao.h" // Để dùng struct LoginCredentials
#include "../../../common/UserRole.h"
#include "../../../common/LoginStatus.h"
#include "StudentSqlParser.h" // Để dùng SqlParserUtils

// LoginCredentials không phải IEntity, nên IEntityParser có thể không hoàn toàn phù hợp.
// Tuy nhiên, chúng ta vẫn cần parse nó từ DbQueryResultRow và serialize thành params.
// Do đó, ta vẫn dùng IEntityParser template, coi LoginCredentials như "TEntity".
class LoginCredentialsSqlParser : public IEntityParser<LoginCredentials, DbQueryResultRow> {
public:
    LoginCredentialsSqlParser() = default;

    // Parse từ row của bảng Logins JOIN Users (để lấy role, status)
    std::expected<LoginCredentials, Error> parse(const DbQueryResultRow& row) const override;
    
    // Serialize LoginCredentials to DbQueryResultRow (ít dùng hơn cho SQL)
    std::expected<DbQueryResultRow, Error> serialize(const LoginCredentials& creds) const override;

    // userId, passwordHash, salt (for INSERT into Logins table)
    std::expected<std::vector<std::any>, Error> toQueryInsertParams(const LoginCredentials& creds) const override;
    
    // passwordHash, salt, userId (for UPDATE Logins table)
    std::expected<std::vector<std::any>, Error> toQueryUpdateParams(const LoginCredentials& creds) const override;
};

#endif // LOGINCREDENTIALSSQLPARSER_H