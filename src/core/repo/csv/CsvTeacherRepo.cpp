#include "CsvTeacherRepo.h"
#include "../../utils/Logger.h"
#include <stdexcept>
#include <algorithm> // For std::find_if, std::remove_if, std::copy_if
#include <charconv> // For std::from_chars

// --- Constructor ---
CsvTeacherRepo::CsvTeacherRepo(std::string csvFilePath) : _filePath(std::move(csvFilePath)) {
    // Optional: Initialize file/header if needed
}

// --- Helper Functions ---
std::vector<Teacher> CsvTeacherRepo::loadFromFile() const {
    std::vector<Teacher> teachers;
     std::vector<std::vector<std::string>> csvData;
     try {
         csvData = CsvFileHandler::readAllLines(_filePath, DELIMITER);
     } catch (const std::runtime_error& e) {
         LOG_ERROR("Error reading teacher data: " + std::string(e.what()));
         return teachers;
     }

    bool isFirstRow = true;
    for (const auto& row : csvData) {
        if (isFirstRow) {
            isFirstRow = false; continue; // Skip header
        }
        try {
            teachers.push_back(parseTeacher(row));
        } catch (const std::exception& e) {
             LOG_ERROR("Skipping invalid teacher row: " + CsvFileHandler::join(row, DELIMITER) + " Error: " + e.what());
        }
    }
    return teachers;
}

bool CsvTeacherRepo::saveToFile(const std::vector<Teacher>& teachers) {
    std::vector<std::vector<std::string>> csvData;
    // Add header
     csvData.push_back({"ID","FirstName","LastName","CitizenID","FacultyID","Qualification","Specialization","Designation","Experience","Email","Phone","Address","Birthday"});

    for (const auto& teacher : teachers) {
         try {
             csvData.push_back(formatTeacher(teacher));
         } catch (const std::exception& e) {
             LOG_ERROR("Error formatting teacher for saving: " + teacher.id() + " Error: " + e.what());
             return false;
         }
    }

    try {
        return CsvFileHandler::writeAllLines(_filePath, csvData, DELIMITER);
    } catch (const std::runtime_error& e) {
        LOG_ERROR("Error writing teacher data: " + std::string(e.what()));
        return false;
    }
}

Teacher CsvTeacherRepo::parseTeacher(const std::vector<std::string>& csvRow) const {
    if (csvRow.size() < NUM_COLUMNS) {
        throw std::runtime_error("Incorrect number of columns in teacher CSV row.");
    }

    int experience = 0;
    // Safer integer parsing
    auto result = std::from_chars(csvRow[COL_EXPERIENCE].data(), csvRow[COL_EXPERIENCE].data() + csvRow[COL_EXPERIENCE].size(), experience);
    if (result.ec != std::errc()) { // Check for conversion errors
         LOG_WARN("Invalid experience value '" + csvRow[COL_EXPERIENCE] + "' for teacher ID: " + csvRow[COL_ID] + ". Defaulting to 0.");
         experience = 0;
    }

    std::string id = csvRow[COL_ID];
    std::string fname = csvRow[COL_FNAME];
    std::string lname = csvRow[COL_LNAME];
    std::string citizenId = csvRow[COL_CITIZEN_ID];
    std::string facultyId = csvRow[COL_FACULTY_ID];
    std::string email = csvRow[COL_EMAIL];
    std::string phone = csvRow[COL_PHONE];
    std::string address = csvRow[COL_ADDRESS];
    std::string bdayStr = csvRow[COL_BIRTHDAY];
    std::string qualification = csvRow[COL_QUALIFICATION];
    std::string specialization = csvRow[COL_SPECIALIZATION];
    std::string designation = csvRow[COL_DESIGNATION];

    Teacher teacher(id, fname, lname, address, 
        citizenId, email, phone,
        facultyId, std::string(), 
        qualification, specialization, designation, experience);
    
    teacher.setBirthday(bdayStr); // Set birthday if valid
    // // Get birthday string for processing
    // std::string bdayStr = csvRow[COL_BIRTHDAY];
    // if (!teacher.setBirthday(bdayStr) && !bdayStr.empty() && bdayStr != "N/A") {
    //      LOG_WARN("Could not parse birthday string '" + bdayStr + "' for teacher ID: " + teacher.id());
    // }

    return teacher;
}

std::vector<std::string> CsvTeacherRepo::formatTeacher(const Teacher& teacher) const {
    std::vector<std::string> csvRow;
    csvRow.resize(NUM_COLUMNS);

    csvRow[COL_ID] = teacher.id();
    csvRow[COL_FNAME] = teacher.firstName();
    csvRow[COL_LNAME] = teacher.lastName();
    csvRow[COL_CITIZEN_ID] = teacher.citizenID();
    csvRow[COL_FACULTY_ID] = teacher.facultyId();
    csvRow[COL_QUALIFICATION] = teacher.qualification();
    csvRow[COL_SPECIALIZATION] = teacher.specializationSubjects();
    csvRow[COL_DESIGNATION] = teacher.designation();
    csvRow[COL_EXPERIENCE] = std::to_string(teacher.experienceYears());
    csvRow[COL_EMAIL] = teacher.email();
    csvRow[COL_PHONE] = teacher.phoneNumber();
    csvRow[COL_ADDRESS] = teacher.address();
    csvRow[COL_BIRTHDAY] = teacher.Birthday();

    return csvRow;
}


// --- ITeacherRepository Implementation ---
std::optional<Teacher> CsvTeacherRepo::findById(const std::string& teacherId) const {
    auto teachers = loadFromFile();
    auto it = std::find_if(teachers.begin(), teachers.end(), [&](const Teacher& t){ return t.id() == teacherId; });
    return (it != teachers.end()) ? std::optional<Teacher>(*it) : std::nullopt;
}

std::vector<Teacher> CsvTeacherRepo::findAll() const {
    return loadFromFile();
}

std::vector<Teacher> CsvTeacherRepo::findByFacultyId(const std::string& facultyId) const {
    auto all = loadFromFile();
    std::vector<Teacher> filtered;
    std::copy_if(all.begin(), all.end(), std::back_inserter(filtered),
                 [&](const Teacher& t){ return t.facultyId() == facultyId; });
    return filtered;
}

std::vector<Teacher> CsvTeacherRepo::findByDesignation(const std::string& designation) const {
     auto all = loadFromFile();
     std::vector<Teacher> filtered;
     std::copy_if(all.begin(), all.end(), std::back_inserter(filtered),
                  [&](const Teacher& t){ return t.designation() == designation; }); // Case-sensitive match
    // For case-insensitive: convert both t.designation() and designation to lower/upper before comparing
     return filtered;
}

bool CsvTeacherRepo::add(const Teacher& teacher) {
    if (exists(teacher.id())) return false;
    auto teachers = loadFromFile();
    teachers.push_back(teacher);
    return saveToFile(teachers);
}

bool CsvTeacherRepo::update(const Teacher& teacher) {
    auto teachers = loadFromFile();
    auto it = std::find_if(teachers.begin(), teachers.end(), [&](const Teacher& t){ return t.id() == teacher.id(); });
    if (it != teachers.end()) {
        *it = teacher;
        return saveToFile(teachers);
    }
    return false; // Not found
}

bool CsvTeacherRepo::remove(const std::string& teacherId) {
    auto teachers = loadFromFile();
    auto it = std::remove_if(teachers.begin(), teachers.end(), [&](const Teacher& t){ return t.id() == teacherId; });
    if (it != teachers.end()) {
        teachers.erase(it, teachers.end());
        return saveToFile(teachers);
    }
    return false; // Not found
}

bool CsvTeacherRepo::exists(const std::string& teacherId) const {
    return findById(teacherId).has_value();
}