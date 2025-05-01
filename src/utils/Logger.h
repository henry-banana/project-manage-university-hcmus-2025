#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex> // Để đảm bảo thread-safe

// Singleton Logger class
class Logger {
private:
    // Private constructor
    Logger();
    ~Logger(); // Đóng file khi hủy

    std::ofstream _logFile;
    std::mutex _mutex; // Để lock khi ghi log
    std::string _logFilename = "app.log"; // Default log file

    // Helper để lấy timestamp
    std::string getCurrentTimestamp() const;

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
};

// Macro tiện ích để ghi log
#define LOG_INFO(message) Logger::getInstance().info(message)
#define LOG_ERROR(message) Logger::getInstance().error(message)
#define LOG_DEBUG(message) Logger::getInstance().debug(message) // Chỉ hoạt động nếu DEBUG được define

#endif // LOGGER_H