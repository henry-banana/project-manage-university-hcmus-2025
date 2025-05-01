#ifndef CSV_LOGIN_REPO_H
#define CSV_LOGIN_REPO_H

#include "../interface/ILoginRepository.h"
#include "../../../utils/FileHandler.h"
#include <string>
#include <map>
#include <optional>

// Implementation of ILoginRepository with CSV
class CsvLoginRepo : public ILoginRepository {
private:
    std::string _dataFilePath;
    std::map<std::string, std::pair<std::string, UserType>> _credentialsCache; // Local cache <userId, <password, userType>>
    CsvFileHandler _fileHandler;

    // Helper to load data from file
    void loadCredentials();
    // Helper to save data to file (if changed)
    bool saveCredentials() const;

protected:

public:
    CsvLoginRepo(const std::string& dataFilePath);

    std::optional<UserType> validateCredentials(const std::string& userId, const std::string& password) override;
    bool addUser(const std::string& userId, const std::string& password, UserType userType) override;
    bool updatePassword(const std::string& userId, const std::string& newPassword) override;
    bool removeUser(const std::string& userId) override;
    std::optional<UserType> getUserType(const std::string& userId) override;
};

#endif // CSV_LOGIN_REPO_H