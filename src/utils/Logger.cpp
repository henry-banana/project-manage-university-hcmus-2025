/**
 * @file Logger.cpp
 * @brief Triển khai các phương thức cho lớp Logger
 * 
 * File này chứa định nghĩa của các phương thức trong lớp Logger,
 * bao gồm khởi tạo, cấu hình, ghi log và các chức năng quản lý file log.
 */
#include "Logger.h"
#include <iostream>  
#include <iomanip>   
#include <chrono>    
#include <sstream>   
#include <algorithm> // For std::sort, std::remove_if
#include <vector>    // Đã include trong .h nhưng để rõ ràng

std::shared_ptr<Logger> Logger::_instance = nullptr; 
std::mutex Logger::_mutex;

/**
 * @brief Constructor khởi tạo với giá trị mặc định
 */
Logger::Logger() : _logLevel(Logger::Level::INFO) {
    // Khởi tạo mặc định, configure sẽ set các giá trị thực tế
}

/**
 * @brief Destructor đóng file log nếu đang mở
 */
Logger::~Logger() {
    if (_logFile && _logFile->is_open()) {
        log(Level::INFO, "Logger shutting down. Closing log file: " + _currentActualLogFilePath);
        _logFile->close();
    }
}

/**
 * @brief Lấy thể hiện duy nhất của Logger (mẫu Singleton)
 * @return Tham chiếu đến thể hiện Logger
 * 
 * Phương thức này sử dụng lock để đảm bảo an toàn đa luồng khi tạo 
 * hoặc truy cập thể hiện Logger.
 */
Logger& Logger::getInstance() {
    std::lock_guard<std::mutex> lock(_mutex); 
    if (_instance == nullptr) {
        struct EnableMakeSharedLogger : public Logger {};
        _instance = std::make_shared<EnableMakeSharedLogger>();
    }
    return *_instance; 
}

/**
 * @brief Giải phóng thể hiện Logger
 * 
 * Phương thức này ghi một thông báo cuối cùng vào log và reset con trỏ singleton.
 * Việc đóng file log sẽ được thực hiện bởi destructor.
 */
void Logger::releaseInstance() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_instance) { // Kiểm tra _instance còn tồn tại không
        if (_instance->_logFile && _instance->_logFile->is_open()) {
            // Ghi trực tiếp, không dùng this->log()
            *(_instance->_logFile) << "[" << _instance->getCurrentTimestamp() << "] [" << _instance->levelToString(Logger::Level::INFO) << "] Logger instance explicitly released. Log file will be closed by destructor if not already." << std::endl;
            _instance->_logFile->flush(); 
            // Không close ở đây, để destructor của Logger làm việc đó khi _instance.reset()
        }
        _instance.reset(); 
    }
}

/**
 * @brief Cấu hình Logger với các thông số chỉ định
 * @param level Cấp độ log
 * @param logDirectory Thư mục lưu file log
 * @param fileBaseName Tên cơ sở của file log
 * @param fileExtension Phần mở rộng của file log
 * 
 * Phương thức này thiết lập cấp độ log, thư mục, tên file, và tạo file log mới.
 * Nó cũng quản lý số lượng file log để tránh tràn ổ đĩa.
 */
void Logger::configure(Level level, const std::filesystem::path& logDirectory, 
                       const std::string& fileBaseName, const std::string& fileExtension) {
    std::lock_guard<std::mutex> lock(_mutex); 
    _logLevel = level;
    _logDirectory = logDirectory;
    _logFileBaseName = fileBaseName;
    _logFileExtension = fileExtension;

    if (_logFile && _logFile->is_open()) {
        _logFile->close(); 
    }

    try {
        if (!std::filesystem::exists(_logDirectory)) {
            if (std::filesystem::create_directories(_logDirectory)) {
                // Không log ở đây vì file log chưa mở
                std::cout << "[Logger Initializing] Created log directory: " << _logDirectory.string() << std::endl;
            } else {
                std::cerr << "Logger Error: Failed to create log directory: " << _logDirectory.string() << std::endl;
                _logFile.reset();
                return; // Không thể tiếp tục nếu không tạo được thư mục log
            }
        }
        
        // Tạo tên file mới với timestamp
        std::string timestamp = getCurrentTimestamp(true); // true để lấy định dạng cho filename
        _currentActualLogFilePath = (_logDirectory / (_logFileBaseName + "_" + timestamp + _logFileExtension)).string();

        _logFile = std::make_unique<std::ofstream>(_currentActualLogFilePath, std::ios_base::out); // Mở file mới (không append)
        
        if (!_logFile || !_logFile->is_open()) {
            std::cerr << "Logger Error: Failed to open new log file: " << _currentActualLogFilePath << std::endl;
            _logFile.reset(); 
        } else {
             // Ghi thông tin vào file log mới
             *_logFile << "[" << getCurrentTimestamp() << "] "
                       << "[" << levelToString(Logger::Level::INFO) << "] " 
                       << "Logger configured. New log session started. Level: " << levelToString(_logLevel)
                       << ". File: " << _currentActualLogFilePath << std::endl;
             _logFile->flush();

             // Quản lý các file log cũ
             manageLogFiles(); 
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Logger Filesystem Error: " << e.what() << " while configuring log file in directory: " << _logDirectory.string() << std::endl;
        _logFile.reset();
    } catch (const std::exception& e) {
        std::cerr << "Logger Exception: " << e.what() << " while configuring log file in directory: " << _logDirectory.string() << std::endl;
        _logFile.reset();
    }
}

/**
 * @brief Thiết lập cấp độ log mới
 * @param level Cấp độ log mới
 * 
 * Phương thức này thay đổi cấp độ log và ghi lại thông báo thay đổi vào file log (nếu mở).
 */
void Logger::setLogLevel(Level level) {
    std::lock_guard<std::mutex> lock(_mutex);
    Level oldLevel = _logLevel; 
    _logLevel = level;
    if (_logFile && _logFile->is_open()) { 
         *_logFile << "[" << getCurrentTimestamp() << "] "
                   << "[" << levelToString(Level::INFO) << "] "
                   << "Log level changed from " << levelToString(oldLevel)
                   << " to " << levelToString(_logLevel) << std::endl;
    }
}

/**
 * @brief Lấy timestamp hiện tại dưới dạng chuỗi
 * @param forFilename Nếu true, định dạng cho tên file, ngược lại định dạng cho hiển thị
 * @return Chuỗi chứa timestamp
 * 
 * Phương thức này trả về timestamp hiện tại dưới dạng chuỗi với định dạng khác nhau
 * tùy thuộc vào tham số forFilename. Nếu là tên file, định dạng là YYYYMMDD_HHMMSS,
 * nếu không thì định dạng là YYYY-MM-DD HH:MM:SS.mmm
 */
std::string Logger::getCurrentTimestamp(bool forFilename) const {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = {}; 

    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&now_tm, &now_c);
    #else
        localtime_r(&now_c, &now_tm);
    #endif

    std::ostringstream oss;
    if (forFilename) {
        oss << std::put_time(&now_tm, "%Y%m%d_%H%M%S");
    } else {
        oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    }
    return oss.str();
}

/**
 * @brief Chuyển đổi cấp độ log thành chuỗi
 * @param level Cấp độ log
 * @return Chuỗi tương ứng với cấp độ log
 * 
 * Phương thức này chuyển đổi các giá trị cấp độ log thành chuỗi để dễ dàng hiển thị.
 */
std::string Logger::levelToString(Level level) const { // Đã public
    switch (level) {
        case Level::DEBUG:    return "DEBUG   ";
        case Level::INFO:     return "INFO    ";
        case Level::WARN:     return "WARNING ";
        case Level::ERROR:    return "ERROR   ";
        case Level::CRITICAL: return "CRITICAL";
        default:              return "UNKNOWN ";
    }
}

/**
 * @brief Ghi một thông điệp vào file log với cấp độ chỉ định
 * @param level Cấp độ của thông điệp
 * @param message Nội dung thông điệp
 * 
 * Phương thức này ghi thông điệp vào file log nếu cấp độ của nó lớn hơn hoặc bằng
 * cấp độ hiện tại của logger. Nó cũng đảm bảo an toàn đa luồng khi ghi log.
 */
void Logger::log(Level level, const std::string& message) {
    if (level < _logLevel) { 
        return;
    }
    std::lock_guard<std::mutex> lock(_mutex); 

    if (_logFile && _logFile->is_open()) {
        *_logFile << "[" << getCurrentTimestamp() << "] "
                  << "[" << levelToString(level) << "] "
                  << message << std::endl;
        if (level >= Level::ERROR) { 
            _logFile->flush();
        }
    } else {
        // Nếu file log chưa mở, ghi vào std::cerr
        std::cerr << "[" << getCurrentTimestamp() << "] "
                  << "[" << levelToString(level) << "] "
                  << message << std::endl;
    }
}

/**
 * @brief Hàm quản lý file log cũ, xóa bớt nếu số lượng vượt quá giới hạn
 * 
 * Hàm này sẽ kiểm tra thư mục log, và xóa các file log cũ nếu số lượng file
 * vượt quá giới hạn MAX_LOG_FILES. Việc xóa file sẽ không làm ảnh hưởng đến file log hiện tại.
 */
void Logger::manageLogFiles() const {
    if (_logDirectory.empty() || !std::filesystem::exists(_logDirectory)) {
        return; // Không có thư mục log để quản lý
    }

    std::vector<std::filesystem::path> logFiles;
    try {
        for (const auto& entry : std::filesystem::directory_iterator(_logDirectory)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                // Kiểm tra xem file có khớp với mẫu tên không (ví dụ: app_YYYYMMDD_HHMMSS.log)
                if (filename.rfind(_logFileBaseName + "_", 0) == 0 && 
                    filename.size() > _logFileBaseName.length() + 1 + 15 + _logFileExtension.length() -1 && // YYYYMMDD_HHMMSS là 15 chars
                    filename.substr(filename.length() - _logFileExtension.length()) == _logFileExtension) 
                {
                    logFiles.push_back(entry.path());
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        // Dùng std::cerr vì logger có thể đang trong quá trình configure
        std::cerr << "Logger Warning: Error iterating log directory: " << e.what() << std::endl;
        return;
    }

    if (logFiles.size() <= static_cast<size_t>(MAX_LOG_FILES)) {
        return; // Chưa vượt quá giới hạn
    }

    // Sắp xếp file theo tên (nếu tên chứa timestamp YYYYMMDD_HHMMSS thì sẽ đúng thứ tự thời gian)
    std::sort(logFiles.begin(), logFiles.end());

    int filesToDelete = static_cast<int>(logFiles.size()) - MAX_LOG_FILES;
    for (int i = 0; i < filesToDelete; ++i) {
        try {
            if (logFiles[i].string() != _currentActualLogFilePath) { // Không xóa file log hiện tại
                std::filesystem::remove(logFiles[i]);
                 // Ghi vào file log hiện tại (nếu đã mở) hoặc cerr
                std::string msg = "Logger: Removed old log file: " + logFiles[i].string();
                if (_logFile && _logFile->is_open()) {
                    *_logFile << "[" << getCurrentTimestamp() << "] [" << levelToString(Level::INFO) << "] " << msg << std::endl;
                } else {
                    std::cout << "[Logger Info] " << msg << std::endl;
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::string errMsg = "Logger Warning: Failed to remove old log file '" + logFiles[i].string() + "': " + e.what();
             if (_logFile && _logFile->is_open()) {
                 *_logFile << "[" << getCurrentTimestamp() << "] [" << levelToString(Level::WARN) << "] " << errMsg << std::endl;
             } else {
                std::cerr << errMsg << std::endl;
             }
        }
    }
}


void Logger::debug(const std::string& message) { log(Level::DEBUG, message); }
void Logger::info(const std::string& message) { log(Level::INFO, message); }
void Logger::warn(const std::string& message) { log(Level::WARN, message); }
void Logger::error(const std::string& message) { log(Level::ERROR, message); }
void Logger::critical(const std::string& message) { log(Level::CRITICAL, message); }