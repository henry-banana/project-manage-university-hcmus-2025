#include "Logger.h"
#include <iostream>  
#include <iomanip>   
#include <chrono>    
#include <filesystem> 
#include <sstream>   

std::shared_ptr<Logger> Logger::_instance = nullptr; 
std::mutex Logger::_mutex;

Logger::Logger() : _logLevel(Logger::Level::INFO), _logFilename("app.log") {} // (➕) Dùng Logger::Level

Logger::~Logger() {
    if (_logFile && _logFile->is_open()) {
        _logFile->close();
    }
}

Logger& Logger::getInstance() {
    std::lock_guard<std::mutex> lock(_mutex); 
    if (_instance == nullptr) {
        struct EnableMakeSharedLogger : public Logger {};
        _instance = std::make_shared<EnableMakeSharedLogger>();
    }
    return *_instance; 
}

void Logger::releaseInstance() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_instance && _instance->_logFile && _instance->_logFile->is_open()) {
         // Có thể log 1 dòng cuối trước khi reset, nhưng cẩn thận nếu instance sắp bị hủy
         _instance->log(Logger::Level::INFO, "Logger instance explicitly released and log file closed.");
    }
    _instance.reset(); 
}

void Logger::configure(Logger::Level level, const std::string& filename) { // (➕) Dùng Logger::Level
    std::lock_guard<std::mutex> lock(_mutex); 
    _logLevel = level;
    _logFilename = filename;

    if (_logFile && _logFile->is_open()) {
        _logFile->close(); 
    }

    try {
        std::filesystem::path logPath(_logFilename);
        if (logPath.has_parent_path() && !std::filesystem::exists(logPath.parent_path())) {
            std::filesystem::create_directories(logPath.parent_path());
        }
        _logFile = std::make_unique<std::ofstream>(_logFilename, std::ios_base::app); 
        if (!_logFile || !_logFile->is_open()) {
            std::cerr << "Logger Error: Failed to open log file: " << _logFilename << std::endl;
            _logFile.reset(); 
        } else {
             *_logFile << "[" << getCurrentTimestamp() << "] "
                       << "[" << levelToString(Logger::Level::INFO) << "] " 
                       << "Logger configured. Level: " << levelToString(_logLevel)
                       << ". File: " << _logFilename << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Logger Filesystem Error: " << e.what() << " while configuring log file: " << _logFilename << std::endl;
        _logFile.reset();
    } catch (const std::exception& e) {
        std::cerr << "Logger Exception: " << e.what() << " while configuring log file: " << _logFilename << std::endl;
        _logFile.reset();
    }
}

void Logger::setLogLevel(Logger::Level level) { // (➕) Dùng Logger::Level
    std::lock_guard<std::mutex> lock(_mutex);
    Logger::Level oldLevel = _logLevel; // (➕) Dùng Logger::Level
    _logLevel = level;
    if (_logFile && _logFile->is_open()) { 
         *_logFile << "[" << getCurrentTimestamp() << "] "
                   << "[" << levelToString(Logger::Level::INFO) << "] "
                   << "Log level changed from " << levelToString(oldLevel)
                   << " to " << levelToString(_logLevel) << std::endl;
    }
}

std::string Logger::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = {}; 

    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&now_tm, &now_c);
    #else
        localtime_r(&now_c, &now_tm);
    #endif

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

// (➕) Định nghĩa hàm public này
std::string Logger::levelToString(Logger::Level level) const { // (➕) Dùng Logger::Level
    switch (level) {
        case Logger::Level::DEBUG:    return "DEBUG   ";
        case Logger::Level::INFO:     return "INFO    ";
        case Logger::Level::WARN:     return "WARNING ";
        case Logger::Level::ERROR:    return "ERROR   ";
        case Logger::Level::CRITICAL: return "CRITICAL";
        default:                      return "UNKNOWN ";
    }
}

void Logger::log(Logger::Level level, const std::string& message) { // (➕) Dùng Logger::Level
    if (level < _logLevel) { 
        return;
    }
    std::lock_guard<std::mutex> lock(_mutex); 

    if (_logFile && _logFile->is_open()) {
        *_logFile << "[" << getCurrentTimestamp() << "] "
                  << "[" << levelToString(level) << "] "
                  << message << std::endl;
        if (level >= Logger::Level::ERROR) { // (➕) Dùng Logger::Level
            _logFile->flush();
        }
    } else {
        std::cerr << "[NO_LOG_FILE] [" << getCurrentTimestamp() << "] "
                  << "[" << levelToString(level) << "] "
                  << message << std::endl;
    }
}

void Logger::debug(const std::string& message) { log(Logger::Level::DEBUG, message); }
void Logger::info(const std::string& message) { log(Logger::Level::INFO, message); }
void Logger::warn(const std::string& message) { log(Logger::Level::WARN, message); }
void Logger::error(const std::string& message) { log(Logger::Level::ERROR, message); }
void Logger::critical(const std::string& message) { log(Logger::Level::CRITICAL, message); }