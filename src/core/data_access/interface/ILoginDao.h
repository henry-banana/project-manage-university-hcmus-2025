/**
 * @file ILoginDao.h
 * @brief Định nghĩa giao diện DAO cho truy cập dữ liệu đăng nhập
 * 
 * Giao diện ILoginDao định nghĩa các phương thức để truy cập và
 * thao tác với dữ liệu đăng nhập người dùng trong hệ thống.
 */
#ifndef ILOGINDAO_H
#define ILOGINDAO_H

#include <string>
#include <vector> // (➕) Cần cho findByStatus
#include <expected> // (➕)
#include "../../../common/UserRole.h"
#include "../../../common/LoginStatus.h"
#include "../../../common/ErrorType.h" // (➕)

// LoginCredentials đã được khai báo trong file header ILoginDao trước đó, giữ nguyên
/**
 * @struct LoginCredentials
 * @brief Cấu trúc dữ liệu chứa thông tin đăng nhập của người dùng
 * 
 * Cấu trúc này chứa ID người dùng, mật khẩu đã được hash, salt,
 * vai trò và trạng thái đăng nhập của người dùng.
 */
struct LoginCredentials {
    std::string userId;     ///< ID của người dùng
    std::string passwordHash; ///< Mật khẩu đã được hash
    std::string salt;       ///< Salt dùng để hash mật khẩu
    UserRole role;          ///< Vai trò của người dùng
    LoginStatus status;     ///< Trạng thái đăng nhập của người dùng
};

/**
 * @class ILoginDao
 * @brief Giao diện DAO cho truy cập dữ liệu đăng nhập
 * 
 * Giao diện này định nghĩa các phương thức để truy cập và thao tác
 * với dữ liệu đăng nhập người dùng trong hệ thống.
 */
class ILoginDao {
public:
    /**
     * @brief Hàm hủy ảo mặc định
     */
    virtual ~ILoginDao() = default;    /**
     * @brief Tìm thông tin đăng nhập theo ID người dùng
     * @param userId ID của người dùng
     * @return Đối tượng LoginCredentials nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<LoginCredentials, Error> findCredentialsByUserId(const std::string& userId) const = 0;
    
    /**
     * @brief Thêm thông tin đăng nhập mới cho người dùng
     * @param userId ID của người dùng
     * @param passwordHash Mật khẩu đã được hash
     * @param salt Salt dùng để hash mật khẩu
     * @param role Vai trò của người dùng
     * @param status Trạng thái đăng nhập của người dùng
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> addUserCredentials(const std::string& userId, const std::string& passwordHash, const std::string& salt, UserRole role, LoginStatus status) = 0;
    
    /**
     * @brief Cập nhật mật khẩu của người dùng
     * @param userId ID của người dùng
     * @param newPasswordHash Mật khẩu mới đã được hash
     * @param newSalt Salt mới dùng để hash mật khẩu
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> updatePassword(const std::string& userId, const std::string& newPasswordHash, const std::string& newSalt) = 0;
    
    /**
     * @brief Xóa thông tin đăng nhập của người dùng
     * @param userId ID của người dùng
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> removeUserCredentials(const std::string& userId) = 0;
    
    /**
     * @brief Lấy vai trò của người dùng
     * @param userId ID của người dùng
     * @return Vai trò của người dùng nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<UserRole, Error> getUserRole(const std::string& userId) const = 0; // Trả UserRole hoặc Error
    
    /**
     * @brief Lấy trạng thái đăng nhập của người dùng
     * @param userId ID của người dùng
     * @return Trạng thái đăng nhập nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<LoginStatus, Error> getUserStatus(const std::string& userId) const = 0; // Trả LoginStatus hoặc Error
    
    /**
     * @brief Cập nhật vai trò và trạng thái đăng nhập của người dùng
     * @param userId ID của người dùng
     * @param newRole Vai trò mới
     * @param newStatus Trạng thái đăng nhập mới
     * @return true nếu thành công, hoặc Error nếu thất bại
     */
    virtual std::expected<bool, Error> updateUserRoleAndStatus(const std::string& userId, UserRole newRole, LoginStatus newStatus) = 0;
    
    /**
     * @brief Tìm người dùng theo trạng thái đăng nhập
     * @param status Trạng thái đăng nhập cần tìm
     * @return Danh sách các đối tượng LoginCredentials có trạng thái tương ứng, hoặc Error nếu thất bại
     */
    virtual std::expected<std::vector<LoginCredentials>, Error> findByStatus(LoginStatus status) const = 0;
};

#endif // ILOGINDAO_H