#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <memory>
#include <mutex> // (➕) Đã có từ trước

// Singleton Logger class
class Logger {    
public: // (➕) Chuyển enum Level ra public để bên ngoài có thể dùng Logger::Level
    enum class Level {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        CRITICAL
    };
private:
    Logger(); 
    ~Logger();

    static std::shared_ptr<Logger> _instance; 
    static std::mutex _mutex; 
    
    std::unique_ptr<std::ofstream> _logFile;
    std::string _logFilename;
    Level _logLevel; // Sử dụng Logger::Level ở đây

    std::string getCurrentTimestamp() const;
    // std::string levelToString(Level level) const; // (➖) Chuyển sang public

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance();
    static void releaseInstance(); 


    void configure(Logger::Level level, const std::string& filename); // (➕) Dùng Logger::Level
    void setLogLevel(Logger::Level level); // (➕) Dùng Logger::Level

    // (➕) Chuyển levelToString sang public
    std::string levelToString(Logger::Level level) const; 

    void log(Logger::Level level, const std::string& message); // (➕) Dùng Logger::Level
    void debug(const std::string& message);
    void info(const std::string& message);
    void warn(const std::string& message);
    void error(const std::string& message);
    void critical(const std::string& message);
};

// Macro tiện ích để ghi log
#define LOG_DEBUG(message) Logger::getInstance().debug(message)
#define LOG_INFO(message)  Logger::getInstance().info(message)
#define LOG_WARN(message)  Logger::getInstance().warn(message)
#define LOG_ERROR(message) Logger::getInstance().error(message)
#define LOG_CRITICAL(message) Logger::getInstance().critical(message)

#endif