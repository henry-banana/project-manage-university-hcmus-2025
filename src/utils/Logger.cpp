#include "Logger.h"
#include <iostream>  // Cho std::cerr nếu không mở được file log
#include <iomanip>   // Cho std::put_time, std::setfill, std::setw
#include <chrono>    // Cho std::chrono::system_clock, time_since_epoch, duration_cast, milliseconds
#include <filesystem> // Cho std::filesystem::path, create_directories (C++17)
#include <sstream>   // Cho std::ostringstream

// Khởi tạo các thành viên static
std::shared_ptr<Logger> Logger::_instance = nullptr; // (➕) Khởi tạo là nullptr
std::mutex Logger::_mutex;

Logger::Logger() : _logLevel(Level::INFO), _logFilename("app.log") {
    // Constructor không nên tự động mở file log ngay,
    // mà nên chờ hàm configure() được gọi.
    // Điều này cho phép AppConfig quyết định đường dẫn và level.
}

Logger::~Logger() {
    if (_logFile && _logFile->is_open()) {
        // Log một thông điệp cuối cùng trước khi đóng (nếu cần)
        // log(Level::INFO, "Logger shutting down."); // Có thể gây vấn đề nếu instance đã bị hủy
        _logFile->close();
    }
}

Logger& Logger::getInstance() {
    std::lock_guard<std::mutex> lock(_mutex); // Đảm bảo thread-safe khi tạo instance
    if (_instance == nullptr) {
        // Dùng make_shared để quản lý Logger instance, nhưng constructor là private
        // Giải pháp: Dùng một "friend class" hoặc một trick nhỏ với struct
        struct EnableMakeSharedLogger : public Logger {};
        _instance = std::make_shared<EnableMakeSharedLogger>();
        // Hoặc, nếu không muốn phức tạp:
        // _instance.reset(new Logger()); // Nhưng constructor phải là public hoặc friend của getInstance
        // Với constructor private, cách tốt nhất là getInstance không trả về tham chiếu mà là shared_ptr
        // và instance được quản lý bởi một static shared_ptr trong hàm này.
        // Tuy nhiên, để giữ API getInstance() trả về Logger&, cách trên là một lựa chọn.
        // Một cách khác đơn giản hơn là:
        // static Logger loggerInstance; // Tạo static instance trực tiếp, thread-safe từ C++11
        // return loggerInstance;
        // --> Nhưng cách này không cho phép gọi releaseInstance() một cách dễ dàng.
        // --> Chọn shared_ptr với private constructor là một pattern phổ biến.
    }
    return *_instance; // Trả về tham chiếu tới Logger object
}

void Logger::releaseInstance() {
    std::lock_guard<std::mutex> lock(_mutex);
    // if (_instance && _instance->_logFile && _instance->_logFile->is_open()) {
    //     _instance->log(Level::INFO, "Logger instance released."); // Cẩn thận khi log ở đây
    // }
    _instance.reset(); // Giải phóng shared_ptr
}

void Logger::configure(Level level, const std::string& filename) {
    std::lock_guard<std::mutex> lock(_mutex); // Đảm bảo thread-safe khi cấu hình
    _logLevel = level;
    _logFilename = filename;

    if (_logFile && _logFile->is_open()) {
        _logFile->close(); // Đóng file cũ nếu có
    }

    try {
        std::filesystem::path logPath(_logFilename);
        // Tạo thư mục nếu nó chưa tồn tại
        if (logPath.has_parent_path()) {
            std::filesystem::create_directories(logPath.parent_path());
        }
        _logFile = std::make_unique<std::ofstream>(_logFilename, std::ios_base::app); // Mở ở chế độ append
        if (!_logFile || !_logFile->is_open()) {
            std::cerr << "Logger Error: Failed to open log file: " << _logFilename << std::endl;
            _logFile.reset(); // Đảm bảo unique_ptr là null nếu mở thất bại
        } else {
            // Log thông điệp cấu hình (không dùng this->log để tránh đệ quy vô hạn nếu log này cũng thất bại)
             *_logFile << "[" << getCurrentTimestamp() << "] "
                       << "[" << levelToString(Level::INFO) << "] " // Luôn log thông điệp này ở INFO
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

void Logger::setLogLevel(Level level) {
    std::lock_guard<std::mutex> lock(_mutex);
    Level oldLevel = _logLevel;
    _logLevel = level;
    if (_logFile && _logFile->is_open()) { // Chỉ log nếu file đã mở
         *_logFile << "[" << getCurrentTimestamp() << "] "
                   << "[" << levelToString(Level::INFO) << "] "
                   << "Log level changed from " << levelToString(oldLevel)
                   << " to " << levelToString(_logLevel) << std::endl;
    }
}

std::string Logger::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = {}; // Khởi tạo zero

    // Sử dụng phiên bản an toàn cho thread (localtime_s trên Windows, localtime_r trên POSIX)
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&now_tm, &now_c);
    #else
        localtime_r(&now_c, &now_tm);
    #endif

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");

    // Thêm milliseconds
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

std::string Logger::levelToString(Level level) const {
    switch (level) {
        case Level::DEBUG:    return "DEBUG   "; // Thêm khoảng trắng để căn chỉnh
        case Level::INFO:     return "INFO    ";
        case Level::WARN:     return "WARNING ";
        case Level::ERROR:    return "ERROR   ";
        case Level::CRITICAL: return "CRITICAL";
        default:              return "UNKNOWN ";
    }
}

void Logger::log(Level level, const std::string& message) {
    if (level < _logLevel) { // Bỏ qua nếu level của message thấp hơn log level hiện tại
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex); // Đảm bảo ghi log thread-safe

    if (_logFile && _logFile->is_open()) {
        *_logFile << "[" << getCurrentTimestamp() << "] "
                  << "[" << levelToString(level) << "] "
                  << message << std::endl;
        // Có thể cân nhắc flush ở các level ERROR, CRITICAL để đảm bảo log được ghi ngay
        if (level >= Level::ERROR) {
            _logFile->flush();
        }
    } else {
        // Fallback: In ra cerr nếu không mở được file log
        // Thêm tiền tố [NO_LOG_FILE] để dễ nhận biết
        std::cerr << "[NO_LOG_FILE] [" << getCurrentTimestamp() << "] "
                  << "[" << levelToString(level) << "] "
                  << message << std::endl;
    }
}

// Các hàm tiện ích
void Logger::debug(const std::string& message) { log(Level::DEBUG, message); }
void Logger::info(const std::string& message) { log(Level::INFO, message); }
void Logger::warn(const std::string& message) { log(Level::WARN, message); }
void Logger::error(const std::string& message) { log(Level::ERROR, message); }
void Logger::critical(const std::string& message) { log(Level::CRITICAL, message); }