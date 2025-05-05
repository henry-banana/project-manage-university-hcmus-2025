#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <memory>

// Singleton Logger class
class Logger {    
public:
    enum class Level {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        CRITICAL
    };
private:


    // Private constructor
    Logger();
    ~Logger(); // Đóng file khi hủy

    std::unique_ptr<std::ofstream> _logFile;
    std::string _logFilename = "../logs/university_app.log"; // Default log file
    Level _logLevel = Level::INFO; // Default logging level

    // Helper để lấy timestamp
    std::string getCurrentTimestamp() const;
    std::string levelToString(Level level) const;

protected:

public:

    // Xóa copy constructor và assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Lấy instance duy nhất của Logger
    static Logger& getInstance();

    // Ghi log message
    void log(const std::string& message);
    void error(const std::string& message);
    void info(const std::string& message);
    void debug(const std::string& message); // Có thể cấu hình level

    // Đặt file log (tùy chọn)
    void setLogFile(const std::string& filename);
    void setLogLevel(Level level);

    // Log messages
    void log(Level level, const std::string& message);
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

#endif // LOGGER_H