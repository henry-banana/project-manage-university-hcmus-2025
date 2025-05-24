// src/core/entities/AdminUser.h
#ifndef ADMINUSER_H
#define ADMINUSER_H

#include "User.h" // Kế thừa từ User

class AdminUser : public User {
public:
    // Constructor (có thể có thêm tham số nếu Admin có các thuộc tính riêng)
    AdminUser(const std::string& id,
              const std::string& firstName,
              const std::string& lastName,
              LoginStatus status = LoginStatus::ACTIVE); // Role sẽ tự đặt là ADMIN

    // Override các phương thức thuần ảo từ User (nếu User vẫn là abstract)
    // hoặc từ IEntity (nếu User đã override rồi thì AdminUser có thể dùng lại hoặc override tiếp)
    std::string display() const override;
    ValidationResult validateBasic() const override;
    
    // Thêm các phương thức hoặc thuộc tính đặc thù cho AdminUser nếu có
    // ví dụ: std::string getAdminLevel() const; 
};

#endif // ADMINUSER_H