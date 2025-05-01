#ifndef ILOGINREPOSITORY_H
#define ILOGINREPOSITORY_H

#include <optional>
#include <string>

// Enum để phân biệt loại người dùng
enum class UserType {
    STUDENT,
    FACULTY,
    ADMIN,
    FINANCE
};

// Interface cho Login Repository (quản lý authentication)
class ILoginRepository {
private:

protected:

public:
    virtual ~ILoginRepository() = default;

    // Xác thực thông tin đăng nhập
    virtual std::optional<UserType> validateCredentials(const std::string& userId, const std::string& password) = 0;
    
    // Thêm người dùng mới
    virtual bool addUser(const std::string& userId, const std::string& password, UserType userType) = 0;
    
    // Cập nhật mật khẩu
    virtual bool updatePassword(const std::string& userId, const std::string& newPassword) = 0;
    
    // Xóa người dùng
    virtual bool removeUser(const std::string& userId) = 0;
    
    // Lấy loại người dùng
    virtual std::optional<UserType> getUserType(const std::string& userId) = 0;
};

#endif // ILOGINREPOSITORY_H