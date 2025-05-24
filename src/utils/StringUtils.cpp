#include "StringUtils.h"
#include <sstream>  // for std::istringstream

namespace StringUtils {

    std::string trim(const std::string& str) {
        // Find the first non-whitespace character
        size_t start = str.find_first_not_of(" \t\n\r\f\v");
        
        // If the string is all whitespace
        if (start == std::string::npos) {
            return "";
        }
        
        // Find the last non-whitespace character
        size_t end = str.find_last_not_of(" \t\n\r\f\v");
        
        // Return the trimmed substring
        return str.substr(start, end - start + 1);
    }

    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::toupper(c); });
        return result;
    }

    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        
        return tokens;
    }

    std::string join(const std::vector<std::string>& elements, char delimiter) {
        std::string result;
        
        for (size_t i = 0; i < elements.size(); ++i) {
            result += elements[i];
            if (i < elements.size() - 1) {
                result += delimiter;
            }
        }
        
        return result;
    }

    bool startsWith(const std::string& str, const std::string& prefix) {
        if (str.length() < prefix.length()) {
            return false;
        }
        
        return str.compare(0, prefix.length(), prefix) == 0;
    }

    bool endsWith(const std::string& str, const std::string& suffix) {
        if (str.length() < suffix.length()) {
            return false;
        }
        
        return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
    }

    bool contains(const std::string& str, const std::string& substring) {
        return str.find(substring) != std::string::npos;
    }

}
