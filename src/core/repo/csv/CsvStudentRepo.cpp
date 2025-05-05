#include "CsvStudentRepo.h"
#include "../../../utils/Logger.h"
#include <algorithm> // For std::find_if, std::remove_if

// --- Constructor ---
CsvStudentRepo::CsvStudentRepo(const std::string& csvFilePath)
    : _filePath(std::move(csvFilePath))
{
     // Optionally check if file exists or create header if new
     // std::ofstream file(_filePath, std::ios::app); // Try opening in append mode
     // if (!file.is_open()) { /* Handle error */ }
     // if (file.tellp() == 0) { // File is empty, write header
     //    file << "ID,FirstName,LastName,CitizenID,FacultyID,Email,Phone,Address,Birthday" << std::endl;
     // }
     // file.close();
}

// --- Helper Functions ---
std::vector<Student> CsvStudentRepo::loadFromFile() const {
    std::vector<Student> students;
    std::vector<std::vector<std::string>> csvData;
    try {
         // Lock mutex for reading
         csvData = CsvFileHandler::readAllLines(_filePath, DELIMITER);
    } catch (const std::runtime_error& e) {
         LOG_ERROR("Error reading student data: " + std::string(e.what()));
         return students; // Return empty vector on read error
    }

    // Skip header row if present (assuming first line is header)
    bool isFirstRow = true;
    for (const auto& row : csvData) {
        if (isFirstRow) {
            isFirstRow = false;
            // Optional: Validate header columns here
            continue;
        }
        try {
            students.push_back(parseStudent(row));
        } catch (const std::exception& e) {
             LOG_ERROR("Skipping invalid student row: " + CsvFileHandler::join(row, DELIMITER) + " Error: " + e.what());
        }
    }
    return students;
}

bool CsvStudentRepo::saveToFile(const std::vector<Student>& students) {
    std::vector<std::vector<std::string>> csvData;
     // Add header row
     csvData.push_back({"ID","FirstName","LastName","CitizenID","FacultyID","Email","Phone","Address","Birthday"});

    for (const auto& student : students) {
        try {
            csvData.push_back(formatStudent(student));
        } catch (const std::exception& e) {
            LOG_ERROR("Error formatting student for saving: " + student.id() + " Error: " + e.what());
            // Decide whether to skip or abort saving
            return false; // Abort on formatting error
        }
    }

    try {
        // Lock mutex for writing
        return CsvFileHandler::writeAllLines(_filePath, csvData, DELIMITER);
    } catch (const std::runtime_error& e) {
        LOG_ERROR("Error writing student data: " + std::string(e.what()));
        return false;
    }
}

Student CsvStudentRepo::parseStudent(const std::vector<std::string>& csvRow) const {
     // Basic validation: check number of columns
    if (csvRow.size() < NUM_COLUMNS) {
        throw std::runtime_error("Incorrect number of columns in student CSV row.");
    }

    // Extract data, handle potential conversion errors
    std::string id = csvRow[COL_ID];
    std::string fname = csvRow[COL_FNAME];
    std::string lname = csvRow[COL_LNAME];
    std::string citizenId = csvRow[COL_CITIZEN_ID];
    std::string facultyId = csvRow[COL_FACULTY_ID];
    std::string email = csvRow[COL_EMAIL];
    std::string phone = csvRow[COL_PHONE];
    std::string address = csvRow[COL_ADDRESS];
    std::string bdayStr = csvRow[COL_BIRTHDAY];

   // This is a guess based on common patterns - adjust parameters according to actual Student constructor
    Student student(id,
        fname,
        lname,
        address,
        citizenId,
        email, 
        phone,
        facultyId
    );

    student.setBirthday(bdayStr); // Assuming setBirthday handles validation internally
    // // Set birthday
    // if (!student.birthday().setBirthday(bdayStr) && !bdayStr.empty() && bdayStr != "N/A") {
    //      LOG_WARN("Could not parse birthday string '" + bdayStr + "' for student ID: " + id);
    //      // Keep default birthday (unset)
    // }

     // Parse password hash/salt if stored here (from corresponding columns)

    return student;
}


std::vector<std::string> CsvStudentRepo::formatStudent(const Student& student) const {
     std::vector<std::string> csvRow;
     csvRow.resize(NUM_COLUMNS); // Ensure correct size

     csvRow[COL_ID] = student.id();
     csvRow[COL_FNAME] = student.firstName();
     csvRow[COL_LNAME] = student.lastName();
     csvRow[COL_CITIZEN_ID] = student.citizenID();
     csvRow[COL_FACULTY_ID] = student.facultyId();
     csvRow[COL_EMAIL] = student.email();
     csvRow[COL_PHONE] = student.phoneNumber();
     csvRow[COL_ADDRESS] = student.address();
     csvRow[COL_BIRTHDAY] = student.Birthday(); // Use Birthday's format

     // Format password hash/salt if stored here

     return csvRow;
}


// --- IStudentRepository Implementation ---

std::optional<Student> CsvStudentRepo::findById(const std::string& studentId) const {
    std::vector<Student> students = loadFromFile();
    auto it = std::find_if(students.begin(), students.end(),
                           [&](const Student& s) { return s.id() == studentId; });
    if (it != students.end()) {
        return *it;
    }
    return std::nullopt;
}

std::vector<Student> CsvStudentRepo::findAll() const {
    return loadFromFile();
}

std::vector<Student> CsvStudentRepo::findByFacultyId(const std::string& facultyId) const {
    std::vector<Student> allStudents = loadFromFile();
    std::vector<Student> filteredStudents;
    std::copy_if(allStudents.begin(), allStudents.end(), std::back_inserter(filteredStudents),
                 [&](const Student& s) { return s.facultyId() == facultyId; });
    return filteredStudents;
}

bool CsvStudentRepo::add(const Student& student) {
    if (exists(student.id())) {
         LOG_WARN("Attempted to add existing student ID: " + student.id());
         return false; // Prevent duplicates
    }
    std::vector<Student> students = loadFromFile();
    students.push_back(student);
    return saveToFile(students);
}

bool CsvStudentRepo::update(const Student& student) {
    std::vector<Student> students = loadFromFile();
    auto it = std::find_if(students.begin(), students.end(),
                           [&](const Student& s) { return s.id() == student.id(); });
    if (it != students.end()) {
        *it = student; // Replace the existing student data
        return saveToFile(students);
    }
    LOG_WARN("Attempted to update non-existent student ID: " + student.id());
    return false; // Student not found
}

bool CsvStudentRepo::remove(const std::string& studentId) {
    std::vector<Student> students = loadFromFile();
    auto it = std::remove_if(students.begin(), students.end(),
                           [&](const Student& s) { return s.id() == studentId; });

    if (it != students.end()) {
        students.erase(it, students.end()); // Erase the removed elements
        return saveToFile(students);
    }
    LOG_WARN("Attempted to remove non-existent student ID: " + studentId);
    return false; // Student not found
}

bool CsvStudentRepo::exists(const std::string& studentId) const {
     // Optimization: Only read IDs or implement a faster lookup if file is large
     // For simplicity now, just use findById
    return findById(studentId).has_value();
}