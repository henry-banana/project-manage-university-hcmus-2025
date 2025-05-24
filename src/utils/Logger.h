/**
 * @file Logger.h
 * @brief Cung cấp chức năng ghi log cho ứng dụng với mẫu thiết kế Singleton
 * 
 * Lớp Logger cho phép ghi nhật ký theo các cấp độ khác nhau (DEBUG, INFO, WARN, ERROR, CRITICAL).
 * Nó được thiết kế là một singleton để đảm bảo duy nhất một thể hiện trong toàn bộ ứng dụng.
 * Các log được ghi vào file với timestamp và có khả năng tự quản lý số lượng file log.
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <memory>
#include <mutex> 
#include <vector>     
#include <filesystem> 

/**
 * @class Logger
 * @brief Lớp singleton cung cấp chức năng ghi log với nhiều cấp độ khác nhau
 * 
 * Logger sử dụng mẫu thiết kế singleton để đảm bảo duy nhất một thể hiện trong ứng dụng.
 * Nó hỗ trợ ghi log theo các cấp độ, quản lý file log, và an toàn đa luồng.
 */
class Logger {    
public: // (QUAN TRỌNG) Enum Level phải public để bên ngoài (main.cpp) có thể dùng Logger::Level
    /**
     * @enum Level
     * @brief Các cấp độ log từ thấp đến cao
     */
    enum class Level {
        DEBUG,    /**< Thông tin chi tiết nhất, chỉ dùng khi debug */
        INFO,     /**< Thông tin thông thường về hoạt động của ứng dụng */
        WARN,     /**< Cảnh báo có thể dẫn đến lỗi */
        ERROR,    /**< Lỗi đã xảy ra nhưng ứng dụng vẫn có thể tiếp tục */
        CRITICAL  /**< Lỗi nghiêm trọng có thể dẫn đến việc dừng ứng dụng */
    };
private:
    /**
     * @brief Constructor riêng tư để đảm bảo mẫu thiết kế singleton
     */
    Logger(); 
    
    /**
     * @brief Destructor sẽ đóng file log nếu đang mở
     */
    ~Logger();

    static std::shared_ptr<Logger> _instance; /**< Thể hiện duy nhất của Logger */
    static std::mutex _mutex; /**< Mutex để đảm bảo an toàn đa luồng */
    
    std::unique_ptr<std::ofstream> _logFile; /**< File stream để ghi log */
    std::filesystem::path _logDirectory; /**< Thư mục chứa file log */    
    std::string _logFileBaseName; /**< Tên cơ sở của file log */     
    std::string _logFileExtension; /**< Phần mở rộng của file log */   
    std::string _currentActualLogFilePath; /**< Đường dẫn đầy đủ của file log hiện tại */

    Logger::Level _logLevel; /**< Cấp độ log hiện tại */

    /**
     * @brief Lấy timestamp hiện tại dưới dạng chuỗi
     * @param forFilename Nếu true, định dạng timestamp phù hợp để dùng trong tên file
     * @return Chuỗi timestamp
     */
    std::string getCurrentTimestamp(bool forFilename = false) const;

    /**
     * @brief Quản lý số lượng file log, xóa các file cũ nếu vượt quá giới hạn
     */
    void manageLogFiles() const; 
    const int MAX_LOG_FILES = 20; /**< Số lượng file log tối đa được giữ lại */

public:
    Logger(const Logger&) = delete; /**< Ngăn chặn copy constructor */
    Logger& operator=(const Logger&) = delete; /**< Ngăn chặn copy assignment */

    /**
     * @brief Lấy thể hiện duy nhất của Logger
     * @return Tham chiếu đến thể hiện Logger
     */
    static Logger& getInstance();
    
    /**
     * @brief Giải phóng thể hiện Logger
     */
    static void releaseInstance(); 

    /**
     * @brief Cấu hình Logger
     * @param level Cấp độ log
     * @param logDirectory Thư mục lưu file log
     * @param fileBaseName Tên cơ sở của file log
     * @param fileExtension Phần mở rộng của file log
     */
    void configure(Logger::Level level, const std::filesystem::path& logDirectory, 
                   const std::string& fileBaseName = "app", 
                   const std::string& fileExtension = ".log"); 
    
    /**
     * @brief Đặt cấp độ log
     * @param level Cấp độ log mới
     */
    void setLogLevel(Logger::Level level); 

    /**
     * @brief Chuyển đổi cấp độ log sang chuỗi
     * @param level Cấp độ log cần chuyển đổi
     * @return Chuỗi biểu diễn cấp độ log
     */
    std::string levelToString(Logger::Level level) const; // (QUAN TRỌNG) Đảm bảo là public

    /**
     * @brief Ghi log với cấp độ chỉ định
     * @param level Cấp độ của thông điệp log
     * @param message Nội dung thông điệp
     */
    void log(Logger::Level level, const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warn(const std::string& message);
    void error(const std::string& message);
    void critical(const std::string& message);
};

#define LOG_DEBUG(message) Logger::getInstance().debug(message)
#define LOG_INFO(message)  Logger::getInstance().info(message)
#define LOG_WARN(message)  Logger::getInstance().warn(message)
#define LOG_ERROR(message) Logger::getInstance().error(message)
#define LOG_CRITICAL(message) Logger::getInstance().critical(message)

#endif