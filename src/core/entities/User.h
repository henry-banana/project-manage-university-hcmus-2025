#ifndef USER_H
#define USER_H

#include <string>

// Lớp cơ sở trừu tượng cho các loại người dùng
class User {
private:
    std::string _id;
    std::string _firstName;
    std::string _lastName;

protected: // Cho phép lớp con truy cập trực tiếp nếu cần, hoặc dùng getter

public:
    // Constructor ảo thuần túy -> làm cho lớp này trừu tượng
    User(std::string id, std::string firstName, std::string lastName)
        : _id(std::move(id)), _firstName(std::move(firstName)), _lastName(std::move(lastName)) {}

    // Destructor ảo để đảm bảo dọn dẹp đúng cách trong kế thừa
    virtual ~User() = default;

    // Các hàm getter chung
    const std::string& getId() const { return _id; }
    const std::string& getFirstName() const { return _firstName; }
    const std::string& getLastName() const { return _lastName; }
    std::string getFullName() const { return _firstName + " " + _lastName; }

    // Có thể thêm các phương thức ảo thuần túy khác nếu cần
    // virtual void displayDetails() const = 0;
};

#endif // USER_H