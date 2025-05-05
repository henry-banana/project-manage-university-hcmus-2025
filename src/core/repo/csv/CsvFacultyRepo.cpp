#include "CsvFacultyRepo.h"
#include "../../../utils/Logger.h"
#include <stdexcept>
#include <algorithm>

CsvFacultyRepo::CsvFacultyRepo(const std::string& csvFilePath) 
    : _filePath(std::move(csvFilePath)) {
    // Optional: Initialize file/header
}

std::vector<Faculty> CsvFacultyRepo::loadFromFile() const {
    std::vector<Faculty> faculties;
    std::vector<std::vector<std::string>> csvData;
    try {
        csvData = CsvFileHandler::readAllLines(_filePath, CSV_DELIMITER);
    } catch (const std::runtime_error& e) {
        LOG_ERROR("Error reading faculty data: " + std::string(e.what()));
        return faculties;
    }

    bool first = true;
    for (const auto& row : csvData) {
        if (first) { first = false; continue; } // Skip header
        try {
             faculties.push_back(parseFaculty(row));
        } catch (const std::exception& e) {
            LOG_ERROR("Skipping invalid faculty row: " + CsvFileHandler::join(row, CSV_DELIMITER) + " Error: " + e.what());
        }
    }
    return faculties;
}

bool CsvFacultyRepo::saveToFile(const std::vector<Faculty>& faculties) {
    std::vector<std::vector<std::string>> csvData;
    csvData.push_back({"ID", "Name"}); // Header
    for (const auto& faculty : faculties) {
        csvData.push_back(formatFaculty(faculty));
    }
    try {
        return CsvFileHandler::writeAllLines(_filePath, csvData, CSV_DELIMITER);
    } catch (const std::runtime_error& e) {
        LOG_ERROR("Error writing faculty data: " + std::string(e.what()));
        return false;
    }
}

Faculty CsvFacultyRepo::parseFaculty(const std::vector<std::string>& csvRow) const {
    if (csvRow.size() < NUM_COLUMNS) {
        throw std::runtime_error("Incorrect columns in faculty CSV row.");
    }
    return Faculty(csvRow[COL_ID], csvRow[COL_NAME]);
}

std::vector<std::string> CsvFacultyRepo::formatFaculty(const Faculty& faculty) const {
    return {faculty.id(), faculty.name()};
}

// IFacultyRepository implementation
std::optional<Faculty> CsvFacultyRepo::findById(const std::string& facultyId) const {
    auto faculties = loadFromFile();
    auto it = std::find_if(faculties.begin(), faculties.end(), [&](const Faculty& f){ return f.id() == facultyId; });
    return (it != faculties.end()) ? std::optional<Faculty>(*it) : std::nullopt;
}

std::optional<Faculty> CsvFacultyRepo::findByName(const std::string& name) const {
     auto faculties = loadFromFile();
     auto it = std::find_if(faculties.begin(), faculties.end(), [&](const Faculty& f){ return f.name() == name; }); // Case-sensitive
     return (it != faculties.end()) ? std::optional<Faculty>(*it) : std::nullopt;
}

std::vector<Faculty> CsvFacultyRepo::findAll() const {
    return loadFromFile();
}

bool CsvFacultyRepo::add(const Faculty& faculty) {
    if (exists(faculty.id()) || findByName(faculty.name())) return false; // Check ID and Name uniqueness
    auto faculties = loadFromFile();
    faculties.push_back(faculty);
    return saveToFile(faculties);
}

bool CsvFacultyRepo::update(const Faculty& faculty) {
    auto faculties = loadFromFile();
    auto it = std::find_if(faculties.begin(), faculties.end(), [&](const Faculty& f){ return f.id() == faculty.id(); });
    if (it != faculties.end()) {
        // Check if new name conflicts with another existing faculty (excluding itself)
        auto conflictIt = std::find_if(faculties.begin(), faculties.end(), [&](const Faculty& f){
            return f.id() != faculty.id() && f.name() == faculty.name();
        });
        if (conflictIt != faculties.end()) {
             LOG_WARN("Update failed: Faculty name '" + faculty.name() + "' already exists.");
             return false;
        }
        *it = faculty;
        return saveToFile(faculties);
    }
    return false; // Not found
}

bool CsvFacultyRepo::remove(const std::string& facultyId) {
    auto faculties = loadFromFile();
    auto it = std::remove_if(faculties.begin(), faculties.end(), [&](const Faculty& f){ return f.id() == facultyId; });
    if (it != faculties.end()) {
        faculties.erase(it, faculties.end());
        return saveToFile(faculties);
    }
    return false; // Not found
}

bool CsvFacultyRepo::exists(const std::string& facultyId) const {
    return findById(facultyId).has_value();
}