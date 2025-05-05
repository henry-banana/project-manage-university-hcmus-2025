#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>
#include <fstream>

class CsvFileHandler {
public:
    // Reads all lines from a CSV file. Throws runtime_error on file open failure.
    // Each inner vector represents a row, split by the delimiter.
    static std::vector<std::vector<std::string>> readAllLines(const std::string& filename, char delimiter = ',');

    // Writes all data to a CSV file (overwrites existing content).
    // Returns true on success, false on file open failure.
    static bool writeAllLines(const std::string& filename, const std::vector<std::vector<std::string>>& data, char delimiter = ',');

    // Appends a single line to the end of a CSV file.
    // Returns true on success, false on file open failure.
    static bool appendLine(const std::string& filename, const std::vector<std::string>& line, char delimiter = ',');

    // Helper to split a string by a delimiter
    static std::vector<std::string> split(const std::string& s, char delimiter);

    // Helper to join elements into a delimited string
    static std::string join(const std::vector<std::string>& elements, char delimiter);
};

#endif // FILEHANDLER_H