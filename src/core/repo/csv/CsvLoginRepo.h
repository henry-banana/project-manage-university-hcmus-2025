#ifndef CSV_LOGIN_REPO_H
#define CSV_LOGIN_REPO_H

#include "../interface/ILoginRepository.h"
#include "../../../utils/FileHandler.h"
#include "../../entities/User.h"
#include "../../../utils/PasswordInput.h"
#include <string>
#include <map>
#include <optional>

// Implementation of ILoginRepository with CSV
class CsvLoginRepo : public ILoginRepository {
private:
    std::string _filePath;
    const char DELIMITER = ',';

    // Column indices
    enum ColumnIndex {
        COL_USER_ID = 0,
        COL_PASSWORD_HASH,
        COL_SALT,
        COL_ROLE, // Store role as string or int? String is more readable.
        NUM_COLUMNS
    };

    // Helper to load all credentials (less efficient but simple)
    std::vector<UserCredentials> loadFromFile() const;
    // Helper to save all credentials
    bool saveToFile(const std::vector<UserCredentials>& credentials);
    // Helper to parse one row
    UserCredentials parseCredentials(const std::vector<std::string>& csvRow) const;
    // Helper to format one row
    std::vector<std::string> formatCredentials(const UserCredentials& creds) const;
        // Helper to convert string to UserRole
    UserRole stringToRole(const std::string& roleStr) const;
    // Helper to convert UserRole to string
    std::string roleToString(UserRole role) const;


public:
    explicit CsvLoginRepo(std::string csvFilePath);

    // ILoginRepository implementation
    std::optional<UserRole> validateCredentials(const std::string& userId, const std::string& plainPassword) const override;
    std::optional<UserCredentials> findCredentialsById(const std::string& userId) const override;
    bool addUserCredentials(const std::string& userId, const std::string& passwordHash, const std::string& salt, UserRole userRole) override;
    bool updatePassword(const std::string& userId, const std::string& newPasswordHash, const std::string& newSalt) override;
    bool removeUserCredentials(const std::string& userId) override;
    std::optional<UserRole> getUserRole(const std::string& userId) const override;
};

#endif // CSV_LOGIN_REPO_H