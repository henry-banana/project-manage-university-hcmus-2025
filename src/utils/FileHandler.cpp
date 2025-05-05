#include "FileHandler.h"
#include "Logger.h" 
#include <sstream>

std::vector<std::string> CsvFileHandler::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        // Basic handling for quoted fields (simple version)
        // A more robust CSV parser would handle escaped quotes etc.
        if (!token.empty() && token.front() == '"' && token.back() == '"') {
            if (token.length() >= 2) {
                 token = token.substr(1, token.length() - 2);
            } else {
                 token = ""; // Empty quoted string
            }
        }
        tokens.push_back(token);
    }
    // Handle case where line ends with delimiter (adds an empty token)
    if (!s.empty() && s.back() == delimiter) {
        tokens.push_back("");
    }
    return tokens;
}

std::string CsvFileHandler::join(const std::vector<std::string>& elements, char delimiter) {
    std::ostringstream oss;
    for (size_t i = 0; i < elements.size(); ++i) {
        // Basic quoting if delimiter or quote is present
        std::string element = elements[i];
        bool needs_quoting = element.find(delimiter) != std::string::npos ||
                             element.find('"') != std::string::npos;
        if (needs_quoting) {
             // Escape existing quotes by doubling them (standard CSV)
             size_t pos = element.find('"');
             while(pos != std::string::npos) {
                 element.replace(pos, 1, "\"\"");
                 pos = element.find('"', pos + 2);
             }
             oss << '"' << element << '"';
        } else {
             oss << element;
        }

        if (i < elements.size() - 1) {
            oss << delimiter;
        }
    }
    return oss.str();
}


std::vector<std::vector<std::string>> CsvFileHandler::readAllLines(const std::string& filename, char delimiter) {
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open file for reading: " + filename);
        // Decide: throw or return empty? Throwing is often better.
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    // Skip header? Depends on repository logic. For now, read all.
    while (std::getline(file, line)) {
        if (!line.empty()) { // Avoid processing empty lines
             data.push_back(split(line, delimiter));
        }
    }
    file.close();
    return data;
}

bool CsvFileHandler::writeAllLines(const std::string& filename, const std::vector<std::vector<std::string>>& data, char delimiter) {
    // Use trunc to overwrite
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open file for writing (overwrite): " + filename);
        return false;
    }

    // Write header? Repository should decide. Assume data includes header if needed.
    for (const auto& row : data) {
        file << join(row, delimiter) << std::endl;
        if (!file.good()) { // Check for write errors after each line
            LOG_ERROR("Error writing line to file: " + filename);
            file.close();
            return false;
        }
    }

    file.close();
    // Check close status? Sometimes errors are only caught on close.
    // if (!file.good()) { /* Error occurred during close */ }
    return true;
}

bool CsvFileHandler::appendLine(const std::string& filename, const std::vector<std::string>& line, char delimiter) {
    // Use app to append
    std::ofstream file(filename, std::ios::out | std::ios::app);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open file for appending: " + filename);
        return false;
    }

    file << join(line, delimiter) << std::endl;
    if (!file.good()) {
        LOG_ERROR("Error appending line to file: " + filename);
        file.close();
        return false;
    }

    file.close();
    return true;
}