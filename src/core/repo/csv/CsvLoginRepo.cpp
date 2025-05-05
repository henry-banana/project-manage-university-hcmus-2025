#include "CsvLoginRepo.h"
#include "../../../utils/Logger.h"

#include <stdexcept>
#include <algorithm>
#include <map> // For string to role mapping

// Define role strings used in the CSV
const std::map<std::string, UserRole> ROLE_MAP_FROM_STRING = {
    {"STUDENT", UserRole::STUDENT},
    {"TEACHER", UserRole::TEACHER},
    {"ADMIN", UserRole::ADMIN},
    {"FINANCE", UserRole::FINANCE}
};

const std::map<UserRole, std::string> ROLE_MAP_TO_STRING = {
    {UserRole::STUDENT, "STUDENT"},
    {UserRole::TEACHER, "TEACHER"},
    {UserRole::ADMIN, "ADMIN"},
    {UserRole::FINANCE, "FINANCE"}
};


CsvLoginRepo::CsvLoginRepo(std::string csvFilePath) : _filePath(std::move(csvFilePath)) {
    // Optional: Initialize file/header
}

UserRole CsvLoginRepo::stringToRole(const std::string& roleStr) const {
    auto it = ROLE_MAP_FROM_STRING.find(roleStr);
    if (it != ROLE_MAP_FROM_STRING.end()) {
        return it->second;
    }
    LOG_WARN("Unknown role string found in login file: " + roleStr + ". Defaulting to STUDENT.");
    return UserRole::STUDENT; // Or throw exception
}

std::string CsvLoginRepo::roleToString(UserRole role) const {
     auto it = ROLE_MAP_TO_STRING.find(role);
     if (it != ROLE_MAP_TO_STRING.end()) {
         return it->second;
     }
     LOG_ERROR("Invalid UserRole enum value encountered: " + std::to_string(static_cast<int>(role)));
     return "UNKNOWN"; // Should not happen if enum is used correctly
}


std::vector<ILoginRepository::UserCredentials> CsvLoginRepo::loadFromFile() const {
    std::vector<UserCredentials> credentials;
    std::vector<std::vector<std::string>> csvData;
    try {
        csvData = CsvFileHandler::readAllLines(_filePath, DELIMITER);
    } catch (const std::runtime_error& e) {
        LOG_ERROR("Error reading login data: " + std::string(e.what()));
        return credentials;
    }

    bool first = true;
    for (const auto& row : csvData) {
        if (first) { first = false; continue; } // Skip header
        try {
            credentials.push_back(parseCredentials(row));
        } catch (const std::exception& e) {
            LOG_ERROR("Skipping invalid login row: " + CsvFileHandler::join(row, DELIMITER) + " Error: " + e.what());
        }
    }
    return credentials;
}

bool CsvLoginRepo::saveToFile(const std::vector<UserCredentials>& credentials) {
    std::vector<std::vector<std::string>> csvData;
    csvData.push_back({"UserID", "PasswordHash", "Salt", "Role"}); // Header
    for (const auto& cred : credentials) {
        try {
        csvData.push_back(formatCredentials(cred));
        } catch (const std::exception& e) {
            LOG_ERROR("Error formatting credentials for saving: " + cred.userId + " Error: " + e.what());
            return false;
        }
    }
    try {
        return CsvFileHandler::writeAllLines(_filePath, csvData, DELIMITER);
    } catch (const std::runtime_error& e) {
        LOG_ERROR("Error writing login data: " + std::string(e.what()));
        return false;
    }
}

ILoginRepository::UserCredentials CsvLoginRepo::parseCredentials(const std::vector<std::string>& csvRow) const {
    return {
        csvRow[COL_USER_ID],
        csvRow[COL_PASSWORD_HASH],
        csvRow[COL_SALT],
        stringToRole(csvRow[COL_ROLE])
    };
}

std::vector<std::string> CsvLoginRepo::formatCredentials(const UserCredentials& creds) const {
    return {
        creds.userId,
        creds.passwordHash,
        creds.salt,
        roleToString(creds.role)
    };
}

// ILoginRepository implementation
std::optional<UserRole> CsvLoginRepo::validateCredentials(const std::string& userId, const std::string& plainPassword) const {
    auto credentialsOpt = findCredentialsById(userId);
    if (credentialsOpt) {
        const auto& creds = credentialsOpt.value();
        // Use a proper password verification function here
        if (verifyPassword(plainPassword, creds.passwordHash, creds.salt)) {
            return creds.role;
        } else {
             LOG_INFO("Password verification failed for user: " + userId);
        }
    } else {
         LOG_INFO("User ID not found during login attempt: " + userId);
    }
    return std::nullopt; // Validation failed or user not found
}


std::optional<ILoginRepository::UserCredentials> CsvLoginRepo::findCredentialsById(const std::string& userId) const {
    auto credentials = loadFromFile();
    auto it = std::find_if(credentials.begin(), credentials.end(),
                           [&](const UserCredentials& c){ return c.userId == userId; });
    if (it != credentials.end()) {
        return *it;
    }
    return std::nullopt;
}

bool CsvLoginRepo::addUserCredentials(const std::string& userId, const std::string& passwordHash, const std::string& salt, UserRole userRole) {
    if (findCredentialsById(userId)) return false; // User already exists
    auto credentials = loadFromFile();
    credentials.push_back({userId, passwordHash, salt, userRole});
    return saveToFile(credentials);
}

bool CsvLoginRepo::updatePassword(const std::string& userId, const std::string& newPasswordHash, const std::string& newSalt) {
    auto credentials = loadFromFile();
    auto it = std::find_if(credentials.begin(), credentials.end(),
                           [&](const UserCredentials& c){ return c.userId == userId; });
    if (it != credentials.end()) {
        it->passwordHash = newPasswordHash;
        it->salt = newSalt;
        return saveToFile(credentials);
    }
    return false; // User not found
}

bool CsvLoginRepo::removeUserCredentials(const std::string& userId) {
    auto credentials = loadFromFile();
    auto it = std::remove_if(credentials.begin(), credentials.end(),
                             [&](const UserCredentials& c){ return c.userId == userId; });
    if (it != credentials.end()) {
        credentials.erase(it, credentials.end());
        return saveToFile(credentials);
    }
    return false; // User not found
}

std::optional<UserRole> CsvLoginRepo::getUserRole(const std::string& userId) const {
     auto credentialsOpt = findCredentialsById(userId);
     if (credentialsOpt) {
         return credentialsOpt.value().role;
     }
     return std::nullopt;
}