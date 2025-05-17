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
    Logger(); // Private constructor for Singleton
    ~Logger();

    static std::shared_ptr<Logger> _instance; // (Thay vì static Logger instance; để có thể delete)
    static std::mutex _mutex; // (➕)
    
    std::unique_ptr<std::ofstream> _logFile;
    std::string _logFilename;
    Level _logLevel;

    std::string getCurrentTimestamp() const;
    std::string levelToString(Level level) const;

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance();
    static void releaseInstance(); // (➕) Dọn dẹp singleton (nếu cần, hiếm khi)


    void configure(Level level, const std::string& filename); // (➕) Cấu hình từ AppConfig
    void setLogLevel(Level level); // Vẫn giữ lại nếu muốn thay đổi sau configure

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

#endif 