#include "Logger.h"
#include <iostream> // For initial errors if file fails=
#include <iomanip> // For std::put_time
#include <ctime>
#include <chrono> // For time functions

Logger::Logger() : _logLevel(Level::INFO) {
    // Constructor attempts to open the default file immediately
    // Using unique_ptr to manage the ofstream resource
    try {
        _logFile = std::make_unique<std::ofstream>(_logFilename, std::ios::app);
         if (!_logFile || !_logFile->is_open()) {
            std::cerr << "Error: Failed to open default log file: " << _logFilename << std::endl;
             _logFile.reset(); // Ensure unique_ptr is null if open failed
         } else {
             info("Logger initialized. Logging to: " + _logFilename); // Log initialization
         }
    } catch (const std::exception& e) {
        std::cerr << "Exception opening default log file '" << _logFilename << "': " << e.what() << std::endl;
        _logFile.reset();
    }
}

Logger::~Logger() {
    if (_logFile && _logFile->is_open()) {
        info("Logger shutting down."); // Log shutdown
        _logFile->close();
    }
}

Logger& Logger::getInstance() {
    // Static instance created on first call (thread-safe in C++11 and later)
    static Logger instance;
    return instance;
}

void Logger::setLogFile(const std::string& filename) {
    if (_logFile && _logFile->is_open()) {
        info("Closing previous log file: " + _logFilename);
        _logFile->close();
    }
    _logFilename = filename;
     try {
        _logFile = std::make_unique<std::ofstream>(_logFilename, std::ios::app);
         if (!_logFile || !_logFile->is_open()) {
            std::cerr << "Error: Failed to open new log file: " << _logFilename << std::endl;
            _logFile.reset();
         } else {
            info("Logger switched log file to: " + _logFilename);
         }
     } catch (const std::exception& e) {
         std::cerr << "Exception opening new log file '" << _logFilename << "': " << e.what() << std::endl;
         _logFile.reset();
     }
}

void Logger::setLogLevel(Level level) {
     _logLevel = level;
     // Log the level change itself (using INFO level)
     info("Log level set to: " + levelToString(level));
}


std::string Logger::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm;

    // Use platform-specific safe versions for converting time_t to tm
    #ifdef _WIN32
        localtime_s(&now_tm, &now_c);
    #else
        localtime_r(&now_c, &now_tm); // POSIX thread-safe version
    #endif

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    // Add milliseconds (optional)
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

std::string Logger::levelToString(Level level) const {
     switch (level) {
         case Level::DEBUG:    return "DEBUG";
         case Level::INFO:     return "INFO "; // Pad for alignment
         case Level::WARN:     return "WARN ";
         case Level::ERROR:    return "ERROR";
         case Level::CRITICAL: return "CRITICAL";
         default:             return "UNKNOWN";
     }
 }


void Logger::log(Level level, const std::string& message) {

     if (level < _logLevel) {
         return; // Skip logging if below current level
     }

     if (_logFile && _logFile->is_open()) {
         // Format: [TIMESTAMP] [LEVEL] message
         *_logFile << "[" << getCurrentTimestamp() << "] "
                   << "[" << levelToString(level) << "] "
                   << message << std::endl;
         // Optional: Flush immediately for critical logs?
         // if (level >= Level::ERROR) {
         //     _logFile->flush();
         // }
     } else {
         // Fallback to stderr if file isn't open
         std::cerr << "[" << getCurrentTimestamp() << "] "
                   << "[" << levelToString(level) << "] "
                   << "[NO LOG FILE] " << message << std::endl;
     }
 }

 // Convenience methods calling log()
 void Logger::debug(const std::string& message) { log(Level::DEBUG, message); }
 void Logger::info(const std::string& message) { log(Level::INFO, message); }
 void Logger::warn(const std::string& message) { log(Level::WARN, message); }
 void Logger::error(const std::string& message) { log(Level::ERROR, message); }
 void Logger::critical(const std::string& message) { log(Level::CRITICAL, message); }